/**
 * @file roa_controller.c
 * @brief «controller» ROAController — 후석 탑승자 감지 경고 제어 로직 구현
 *
 * @details
 * UC-003 시퀀스 다이어그램 전체 흐름을 담당한다.
 *
 *   Main Success (step 1~12):
 *     자가진단 → 레이더 활성화 → 탑승자 감지 → 데이터 저장
 *     → 1차 경고 → 2차 경고 8사이클(경적+모바일) → 종료
 *
 *   AF-003-1: 1차 경고 후 도어 열림 → 즉시 종료
 *   EF-003-1: 레이더 자가진단 실패 → DTC 기록 → 클러스터 상태 표시
 *   EF-003-2: 모바일 통신 불가 → 경보 계속(비치명적)
 *   EF-003-3: EV 보조 배터리 부족 → 마지막 1회 2차 경고 후 종료
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 */

#include "roa_controller.h"
#include "../boundary/radar_sensor_if.h"
#include "../boundary/cluster_if.h"
#include "../boundary/alarm_if.h"
#include "../boundary/mobile_app_if.h"
#include "../entity/occupant_data.h"
#include <stdio.h>

/* ============================================================
 * Internal state
 * ============================================================ */

/** @brief 현재 컨트롤러 상태 */
static ROAState_t      s_state;

/** @brief 초기화 파라미터 */
static ROAInitParams_t s_params;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 컨트롤러 초기화
 */
void ROAController_init(const ROAInitParams_t *params)
{
    if (params == NULL) {
        return;
    }

    s_params = *params;
    s_state  = ROA_STATE_IDLE;
    OccupantData_init();

    printf("[ROAController] init(vehicleType=%d, evBatteryLow=%s, doorOpened=%s)\n",
           (int)params->vehicleType,
           params->evBatteryLow ? "true" : "false",
           params->doorOpened   ? "true" : "false");
}

/**
 * @brief 현재 컨트롤러 상태 조회
 */
ROAState_t ROAController_getState(void)
{
    return s_state;
}

/**
 * @brief 감지 모드 시작 — 전체 UC-003 시퀀스 실행
 */
ROAResult_t ROAController_startDetectionMode(void)
{
    ROAResult_t            ret;
    OccupantDetectedMsg_t  detectedMsg;
    DeliveryConfirmation_t confirm;
    uint8_t                alarmCycle;
    uint8_t                maxCycles;

    /* ----------------------------------------------------------
     * Step 1: 레이더 센서 자가진단 (EF-003-1 대응)
     * ---------------------------------------------------------- */
    ret = RadarSensorIF_selfDiagnostic();
    if (ret != ROA_OK) {
        s_state = ROA_STATE_FAULT;
        (void)OccupantData_storeDTC(ROA_DTC_SENSOR_FAIL);
        (void)ClusterIF_displayStatus("ROA 점검 요망", true);
        printf("[ROAController] EF-003-1: sensor fail → state=FAULT\n");
        return ret;
    }

    /* ----------------------------------------------------------
     * Step 2: 레이더 활성화 (57-66 GHz ISM band)
     * ---------------------------------------------------------- */
    s_state = ROA_STATE_MONITORING;
    ret = RadarSensorIF_activateRadar();
    if (ret != ROA_OK) {
        s_state = ROA_STATE_FAULT;
        return ret;
    }

    /* ----------------------------------------------------------
     * Step 3~5: 탑승자 감지 1사이클 실행
     * ---------------------------------------------------------- */
    ret = RadarSensorIF_startDetectionMode(&detectedMsg);
    if (ret != ROA_OK) {
        s_state = ROA_STATE_FAULT;
        (void)OccupantData_storeDTC(ROA_DTC_SENSOR_FAIL);
        (void)ClusterIF_displayStatus("ROA 점검 요망", true);
        return ret;
    }

    if ((detectedMsg.confidence != CONFIDENCE_HIGH) ||
        (detectedMsg.position   != POSITION_REAR_SEAT)) {
        s_state = ROA_STATE_TERMINATED;
        printf("[ROAController] No occupant detected — monitoring complete\n");
        return ROA_OK;
    }

    /* ----------------------------------------------------------
     * Step 6: 탑승자 감지 결과 저장
     * ---------------------------------------------------------- */
    (void)OccupantData_storeDetection(true, detectedMsg.breathRate, detectedMsg.position);

    /* ----------------------------------------------------------
     * Step 7~9: 1차 경고 — 클러스터 경고 메시지 + 경고음
     * ---------------------------------------------------------- */
    s_state = ROA_STATE_WARNING_LV1;
    (void)ClusterIF_displayWarning("뒷좌석 탑승자 확인", CLUSTER_WARNING_LEVEL_1);

    /* AF-003-1: 1차 경고 후 도어 열림 → 즉시 종료 */
    if (s_params.doorOpened) {
        printf("[ROAController] AF-003-1: door opened → terminate immediately\n");
        s_state = ROA_STATE_TERMINATED;
        return ROA_OK;
    }

    /* EF-003-3: EV 보조 배터리 부족 → 마지막 1회 2차 경고만 수행 */
    if (s_params.evBatteryLow) {
        maxCycles = 1u;
        printf("[ROAController] EF-003-3: EV battery low → last 1 alarm cycle\n");
    } else {
        maxCycles = ROA_MAX_ALARM_CYCLE;
    }

    /* ----------------------------------------------------------
     * Step 10~11: 2차 경고 루프 (경적 + 모바일 알림)
     * ---------------------------------------------------------- */
    s_state = ROA_STATE_WARNING_LV2;

    for (alarmCycle = 0u; alarmCycle < maxCycles; alarmCycle++) {
        printf("[ROAController] 2차 경고 cycle %u/%u\n",
               (unsigned)(alarmCycle + 1u), (unsigned)maxCycles);

        /* step 10: 경적 + 비상등 활성화 */
        (void)AlarmIF_activateAlarm(ROA_ALARM_HORN_SEC, true);

        /* step 11: SMS + 푸시 알림 (EF-003-2: 실패 시 비치명적) */
        ret = MobileAppIF_sendAlert("위치 정보 N/A", "뒷좌석 탑승자 감지", &confirm);
        if (ret == ROA_ERR_COMM_FAIL) {
            printf("[ROAController] EF-003-2: mobile alert failed — alarm continues\n");
        }

        (void)AlarmIF_deactivateAlarm();
    }

    s_state = ROA_STATE_TERMINATED;

    /* EF-003-3: 배터리 부족으로 종료 시 해당 오류 코드 반환 */
    if (s_params.evBatteryLow) {
        return ROA_ERR_BATTERY_LOW;
    }

    return ROA_OK;
}
