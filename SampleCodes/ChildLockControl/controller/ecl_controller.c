/**
 * @file ecl_controller.c
 * @brief «controller» ECLController — 도어 잠금/해제 제어 로직 구현
 *
 * @details
 * UC-001 시퀀스 다이어그램 전체 흐름을 담당한다.
 *
 *   Main Success (step 1~13):
 *     버튼 입력 수신 → 현재 상태 조회 → 토글 판단
 *     → CAN 명령 전송 → 양쪽 DCU ACK 검증
 *     → 상태 저장 → 클러스터 표시등 갱신
 *
 *   EF-001-1: 시동 OFF 3분 초과 → 입력 Discard + "조작 불가" 표시
 *   EF-001-2: DCU ACK 미수신 → DTC 기록 + "시스템 점검 요망" 경고
 *   EF-001-3: 액추에이터 걸림 → 모터 정지 + NACK + DTC 기록
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#include "ecl_controller.h"
#include "../boundary/driver_button_if.h"
#include "../boundary/can_bus_if.h"
#include "../boundary/door_actuator_if.h"
#include "../boundary/cluster_if.h"
#include "../entity/lock_state.h"
#include <stdio.h>

/* ============================================================
 * Internal state
 * ============================================================ */

/** @brief 현재 컨트롤러 상태 */
static ECLState_t s_state;

/** @brief 초기화 파라미터 */
static ECLInitParams_t s_params;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 컨트롤러 초기화
 */
void ECLController_init(const ECLInitParams_t *params)
{
    if (params == NULL) {
        return;
    }

    s_params = *params;
    s_state  = ECL_STATE_IDLE;

    LockState_init();
    (void)LockState_updateState(params->initialState);

    DriverButtonIF_setEngineOffElapsed(params->engineOffElapsedSec);
    CANBusIF_setErrorInject(params->injectAckFail, params->injectActuatorJam);

    printf("[ECLController] init(initialState=%s, engineOffElapsed=%us, "
           "injectAckFail=%s, injectJam=%s)\n",
           (params->initialState == LOCK_STATE_LOCKED) ? "LOCKED" : "UNLOCKED",
           params->engineOffElapsedSec,
           params->injectAckFail     ? "true" : "false",
           params->injectActuatorJam ? "true" : "false");
}

/**
 * @brief 현재 컨트롤러 상태 조회
 */
ECLState_t ECLController_getState(void)
{
    return s_state;
}

/**
 * @brief 버튼 입력 처리 — 전체 UC-001 시퀀스 실행 (step 2~13)
 */
ECLResult_t ECLController_handleButtonPress(void)
{
    ECLResult_t      ret;
    ToggleSignal_t   toggleSignal;
    LockStateValue_t currentState;
    LockStateValue_t newState;
    CANCommand_t     cmd;
    DoorAck_t        ack;

    s_state = ECL_STATE_PROCESSING;

    /* ----------------------------------------------------------
     * Step 1~2: 버튼 입력 수신
     *   EF-001-1: 시동 OFF 3분 초과 → Discard
     * ---------------------------------------------------------- */
    ret = DriverButtonIF_buttonPressed(&toggleSignal);
    if (ret == ECL_ERR_TIMEOUT) {
        /* EF-001-1: 조작 불가 메시지 3초 표시 */
        (void)ClusterIF_displayWarning("조작 불가", ECL_CLUSTER_NOTICE_SEC);
        printf("[ECLController] EF-001-1: 버튼 입력 Discard → state=FAULT\n");
        s_state = ECL_STATE_FAULT;
        return ret;
    }
    if (ret != ECL_OK) {
        s_state = ECL_STATE_FAULT;
        return ret;
    }

    /* ----------------------------------------------------------
     * Step 3: 현재 잠금 상태 조회
     * ---------------------------------------------------------- */
    ret = LockState_getCurrentState(&currentState);
    if (ret != ECL_OK) {
        s_state = ECL_STATE_FAULT;
        return ret;
    }

    /* ----------------------------------------------------------
     * Step 4: 토글 판단 (현재 상태 반전)
     * ---------------------------------------------------------- */
    if (currentState == LOCK_STATE_UNLOCKED) {
        newState = LOCK_STATE_LOCKED;
        cmd      = CAN_CMD_LOCK;
        printf("[ECLController] 토글 판단: UNLOCKED → LOCK 명령 생성\n");
    } else {
        newState = LOCK_STATE_UNLOCKED;
        cmd      = CAN_CMD_UNLOCK;
        printf("[ECLController] 토글 판단: LOCKED → UNLOCK 명령 생성\n");
    }

    /* ----------------------------------------------------------
     * Step 5~9: CAN 명령 전송 및 양쪽 DCU ACK 수신
     *   EF-001-2: ACK 미수신
     *   EF-001-3: 액추에이터 걸림
     * ---------------------------------------------------------- */
    ret = CANBusIF_sendCommand(cmd, DOOR_TARGET_BOTH, &ack);
    if (ret == ECL_ERR_ACTUATOR_JAM) {
        /* EF-001-3: DCU NACK + DTC 기록 */
        (void)LockState_storeDTC(ECL_DTC_ACTUATOR_JAM);
        printf("[ECLController] EF-001-3: 액추에이터 걸림 → DTC 기록\n");
        s_state = ECL_STATE_FAULT;
        return ret;
    }
    if (ret == ECL_ERR_ACK_FAIL) {
        /* EF-001-2: DTC 기록 + "시스템 점검 요망" */
        (void)LockState_storeDTC(ECL_DTC_ACK_FAIL);
        (void)ClusterIF_displayWarning("차일드락 시스템 점검 요망", ECL_CLUSTER_NOTICE_SEC);
        printf("[ECLController] EF-001-2: ACK 미수신 → DTC 기록 + 경고. 잠금 상태 변경 없음\n");
        s_state = ECL_STATE_FAULT;
        return ret;
    }
    if (ret != ECL_OK) {
        s_state = ECL_STATE_FAULT;
        return ret;
    }

    /* ----------------------------------------------------------
     * Step 10: 양쪽 DCU ACK 검증 (left AND right)
     * ---------------------------------------------------------- */
    printf("[ECLController] ACK 검증: left=%s, right=%s\n",
           ack.leftOK ? "OK" : "FAIL",
           ack.rightOK ? "OK" : "FAIL");

    if (!ack.leftOK || !ack.rightOK) {
        (void)LockState_storeDTC(ECL_DTC_ACK_FAIL);
        (void)ClusterIF_displayWarning("차일드락 시스템 점검 요망", ECL_CLUSTER_NOTICE_SEC);
        s_state = ECL_STATE_FAULT;
        return ECL_ERR_ACK_FAIL;
    }

    /* ----------------------------------------------------------
     * Step 11: 상태 저장
     * ---------------------------------------------------------- */
    (void)LockState_updateState(newState);

    if (newState == LOCK_STATE_LOCKED) {
        s_state = ECL_STATE_LOCKED;
    } else {
        s_state = ECL_STATE_UNLOCKED;
    }

    /* ----------------------------------------------------------
     * Step 12~13: 클러스터 표시등 갱신 + 확인음
     * ---------------------------------------------------------- */
    (void)ClusterIF_displayStatus(newState, true);

    return ECL_OK;
}
