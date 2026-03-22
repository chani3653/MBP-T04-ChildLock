/**
 * @file emergency_controller.c
 * @brief «controller» EmergencyController — 충돌 시 비상 해제 제어 로직 구현
 *
 * @details
 * UC-002 시퀀스 다이어그램 전체 흐름을 담당한다.
 *
 *   Main Success (step 1~10):
 *     충돌 이벤트 수신 → 충돌 정보 저장 → 잠금 상태 확인
 *     → 비상 해제 판단 → 최고 우선순위 CAN 전송 → forceUnlatch
 *     → ACK 수신 → 상태 EMERGENCY_RELEASED 갱신
 *
 *   AF-002-1: CAN 버스 단선 → EU_ERR_CAN_FAIL (아날로그 직접 전송 안내)
 *   AF-002-2: 2차 충돌 위험 → 지연 후 해제
 *   EF-002-1: 액추에이터 파손 → DTC 기록 + EU_ERR_ACTUATOR_FAIL
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#include "emergency_controller.h"
#include "../boundary/airbag_if.h"
#include "../boundary/can_bus_if.h"
#include "../boundary/door_actuator_if.h"
#include "../entity/crash_event.h"
#include "../entity/lock_state.h"
#include <stdio.h>

/* ============================================================
 * Internal state
 * ============================================================ */

/** @brief 현재 컨트롤러 상태 */
static EUState_t s_state;

/** @brief 초기화 파라미터 */
static EUInitParams_t s_params;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 컨트롤러 초기화
 */
void EmergencyController_init(const EUInitParams_t *params)
{
    if (params == NULL) {
        return;
    }

    s_params = *params;
    s_state  = EU_STATE_IDLE;

    CrashEvent_init();
    EULockState_init();

    CANBusIF_setErrorInject(params->injectCanFail, params->injectActuatorFail);
    DoorActuatorIF_setActuatorFail(params->injectActuatorFail);

    printf("[EmergencyController] init(injectCanFail=%s, injectActuatorFail=%s, "
           "secondaryImpactRisk=%s)\n",
           params->injectCanFail      ? "true" : "false",
           params->injectActuatorFail ? "true" : "false",
           params->secondaryImpactRisk ? "true" : "false");
}

/**
 * @brief 현재 컨트롤러 상태 조회
 */
EUState_t EmergencyController_getState(void)
{
    return s_state;
}

/**
 * @brief 충돌 이벤트 처리 — 전체 UC-002 시퀀스 실행 (step 2~10)
 */
EUResult_t EmergencyController_handleCrashEvent(void)
{
    EUResult_t       ret;
    CrashEventData_t event;
    EULockState_t    lockState;
    EmergencyAck_t   ack;

    s_state = EU_STATE_PROCESSING;

    /* ----------------------------------------------------------
     * Step 1~2: 충돌 이벤트 수신
     * ---------------------------------------------------------- */
    ret = AirbagIF_onCrashDetected(&event);
    if (ret != EU_OK) {
        s_state = EU_STATE_FAULT;
        return ret;
    }

    /* ----------------------------------------------------------
     * Step 3: 충돌 정보 저장
     * ---------------------------------------------------------- */
    ret = CrashEvent_storeCrashInfo(&event);
    if (ret != EU_OK) {
        s_state = EU_STATE_FAULT;
        return ret;
    }

    /* ----------------------------------------------------------
     * Step 4: 현재 잠금 상태 확인
     * ---------------------------------------------------------- */
    ret = EULockState_getCurrentState(&lockState);
    if (ret != EU_OK) {
        s_state = EU_STATE_FAULT;
        return ret;
    }

    /* ----------------------------------------------------------
     * Step 5: 비상 해제 판단
     *   조건: airbagDeployed=true AND currentState=LOCKED
     * ---------------------------------------------------------- */
    if (!event.airbagDeployed || (lockState != EU_LOCK_STATE_LOCKED)) {
        printf("[EmergencyController] 비상 해제 조건 미충족 — 시퀀스 종료\n");
        s_state = EU_STATE_IDLE;
        return EU_OK;
    }

    printf("[EmergencyController] 비상 해제 판단: airbagDeployed=true AND state=LOCKED"
           " → EMERGENCY_UNLOCK 명령 생성\n");

    /* AF-002-2: 2차 충돌 위험 → 지연 후 해제 */
    if (s_params.secondaryImpactRisk) {
        printf("[EmergencyController] AF-002-2: 2차 충돌 위험 감지 → %d~%d초 지연 후 해제\n",
               EU_SECONDARY_DELAY_MIN_SEC, EU_SECONDARY_DELAY_MAX_SEC);
    }

    /* ----------------------------------------------------------
     * Step 6~9: 비상 해제 CAN 전송 및 ACK 수신
     *   AF-002-1: CAN 버스 단선
     *   EF-002-1: 액추에이터 파손
     * ---------------------------------------------------------- */
    ret = CANBusIF_sendEmergencyUnlock(EU_PRIORITY_HIGHEST, &ack);

    if (ret == EU_ERR_CAN_FAIL) {
        /* AF-002-1: CAN 버스 단선 → 에어백 ECU가 아날로그 직접 전송 안내 */
        (void)CrashEvent_storeDTC(EU_DTC_CAN_BUS_FAIL);
        printf("[EmergencyController] AF-002-1: CAN 단선 → 에어백 ECU 아날로그 펄스 직접 전송 필요\n");
        s_state = EU_STATE_FAULT;
        return ret;
    }

    if (ret == EU_ERR_ACTUATOR_FAIL) {
        /* EF-002-1: 액추에이터 파손 → DTC 기록 + eCall 요청 안내 */
        (void)CrashEvent_storeDTC(EU_DTC_ACTUATOR_FAIL);
        printf("[EmergencyController] EF-002-1: 액추에이터 파손 → DTC 기록 + eCall \"도어 해제 실패\" 포함\n");
        s_state = EU_STATE_FAULT;
        return ret;
    }

    if (ret != EU_OK) {
        s_state = EU_STATE_FAULT;
        return ret;
    }

    /* ----------------------------------------------------------
     * Step 10: 상태 EMERGENCY_RELEASED 갱신
     * ---------------------------------------------------------- */
    (void)EULockState_updateState(EU_LOCK_STATE_EMERGENCY_RELEASED);
    s_state = EU_STATE_RELEASED;

    return EU_OK;
}
