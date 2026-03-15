/**
 * @file roa_controller.c
 * @brief «control» ROAController — BCM/IBU 후석 탑승자 경고 컨트롤러 구현
 *
 * @details
 * UC-003 시퀀스 다이어그램 전체 흐름을 구현한다.
 *
 * [Main Success Scenario]
 *   step 1 : startDetectionMode() → RadarSensorIF
 *   step 2 : activateRadar(57-66 GHz ISM band)
 *   loop   : 모니터링 (ICE 24h / EV 10min)
 *     step 3~5: 원시 신호 수신 → FFT 분석 → occupantDetected
 *   step 6 : storeDetection(occupantPresent=true, breathRate, timestamp)
 *   step 7 : 경고 단계 판단 → 1차 경고 준비
 *   step 8~9: displayWarning('뒷좌석 탑승자 확인', level=1) → ClusterIF
 *   [운전자 응답 대기]
 *   loop   : 2차 경고 (최대 8회 × 25초 사이클)
 *     step 10~10a: activateAlarm(horn=25sec, hazardLights=ON) → AlarmIF
 *     step 11~11a: sendAlert(SMS+PUSH) → MobileAppIF
 *   step 12 : 모니터링 지속 판단
 *
 * [Alternative Flow]
 *   AF-003-1: 도어 열림/잠금 해제 → 경고 시퀀스 즉시 중단
 *
 * [Exception Flows]
 *   EF-003-1: 센서 자가진단 실패 → DTC 기록, ROA 비활성화
 *   EF-003-2: 모바일 통신 불가   → 경적/비상등만 반복
 *   EF-003-3: EV 배터리 부족     → 마지막 1회 2차 경고 후 종료
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
 * Internal constants
 * ============================================================ */

/** @brief 1차 경고 대기 시간 (초, 시뮬레이션에서는 즉시 에스컬레이션) */
#define ROA_LV1_WAIT_SEC              (30u)

/** @brief 차량 위치 정보 (Assumption: 고정 시뮬레이션 값) */
#define ROA_VEHICLE_LOCATION          "37.123456, 127.654321"

/** @brief 모바일 알림 메시지 */
#define ROA_ALERT_MSG                 "뒷좌석 탑승자 감지"

/** @brief 1차 경고 메시지 */
#define ROA_WARNING_MSG_LV1           "뒷좌석 탑승자 확인"

/** @brief ROA 점검 요망 메시지 */
#define ROA_SENSOR_FAULT_MSG          "ROA 시스템 점검 요망"

/* ============================================================
 * Internal state
 * ============================================================ */

/** @brief 현재 ROA 상태 */
static ROAState_t s_state = ROA_STATE_IDLE;

/** @brief 초기화 파라미터 */
static ROAInitParams_t s_params;

/* ============================================================
 * Internal function declarations
 * ============================================================ */

static ROAResult_t runMonitoringLoop(void);
static ROAResult_t executeLevel1Warning(void);
static ROAResult_t executeLevel2Warning(void);
static uint32_t    getMaxMonitorSec(void);

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief ROA 컨트롤러 초기화
 */
void ROAController_init(const ROAInitParams_t *params)
{
    if (params == NULL) {
        return;
    }

    s_params = *params;
    s_state  = ROA_STATE_IDLE;

    OccupantData_init();
    printf("[ROAController] Initialized (vehicleType=%s, evBatteryLow=%s, doorOpened=%s)\n",
           (s_params.vehicleType == VEHICLE_TYPE_EV) ? "EV" : "ICE",
           s_params.evBatteryLow ? "true" : "false",
           s_params.doorOpened   ? "true" : "false");
}

/**
 * @brief ROA 감지 모드 시작 — 전체 시퀀스 진입점
 */
ROAResult_t ROAController_startDetectionMode(void)
{
    ROAResult_t ret;

    printf("\n[ROAController] ===== UC-003 ROA 시작 (트리거: 시동 OFF + 차량 잠금 완료) =====\n");

    /* EF-003-1: 센서 자가진단 */
    ret = RadarSensorIF_selfDiagnostic();
    if (ret != ROA_OK) {
        printf("[ROAController] [EF-003-1] 센서 자가진단 실패 → DTC 기록\n");
        (void)OccupantData_storeDTC(ROA_DTC_SENSOR_FAIL);
        (void)ClusterIF_displayStatus(ROA_SENSOR_FAULT_MSG, true);
        s_state = ROA_STATE_FAULT;
        return ROA_ERR_SENSOR_FAIL;
    }

    /* step 1: startDetectionMode → 레이더 활성화 */
    printf("[ROAController] step 1: startDetectionMode()\n");
    ret = RadarSensorIF_activateRadar();
    if (ret != ROA_OK) {
        s_state = ROA_STATE_FAULT;
        return ret;
    }

    s_state = ROA_STATE_MONITORING;

    /* EF-003-3: EV 배터리 부족 → 마지막 1회 2차 경고 후 종료 */
    if ((s_params.vehicleType == VEHICLE_TYPE_EV) && s_params.evBatteryLow) {
        printf("[ROAController] [EF-003-3] EV 보조 배터리 부족 → 마지막 1회 2차 경고 후 종료\n");
        (void)AlarmIF_activateAlarm(ROA_ALARM_HORN_SEC, true);
        s_state = ROA_STATE_TERMINATED;
        return ROA_ERR_BATTERY_LOW;
    }

    /* 모니터링 루프 실행 */
    ret = runMonitoringLoop();
    s_state = ROA_STATE_TERMINATED;
    printf("[ROAController] ===== ROA 시퀀스 종료 =====\n");
    return ret;
}

/**
 * @brief 현재 ROA 시스템 상태 조회
 */
ROAState_t ROAController_getState(void)
{
    return s_state;
}

/* ============================================================
 * Internal functions
 * ============================================================ */

/**
 * @brief 차량 타입에 따른 최대 모니터링 시간(초) 반환
 */
static uint32_t getMaxMonitorSec(void)
{
    return (s_params.vehicleType == VEHICLE_TYPE_EV) ?
           ROA_MAX_MONITOR_EV_SEC :
           ROA_MAX_MONITOR_ICE_SEC;
}

/**
 * @brief 모니터링 루프 — 탑승자 감지 시까지 반복 (step 3~7)
 *
 * @note 시뮬레이션에서는 1회 실행 후 탑승자 감지로 간주한다.
 * @assumption 실제 구현에서는 타이머 기반으로 getMaxMonitorSec() 동안 루프 반복.
 */
static ROAResult_t runMonitoringLoop(void)
{
    ROAResult_t           ret;
    OccupantDetectedMsg_t detected;
    uint32_t              maxSec;

    maxSec = getMaxMonitorSec();
    printf("[ROAController] loop [모니터링 지속: %u초 (%s)]\n",
           maxSec,
           (s_params.vehicleType == VEHICLE_TYPE_EV) ? "EV 10min" : "ICE 24h");

    /* step 3~5: 원시 신호 수신 및 분석 */
    ret = RadarSensorIF_startDetectionMode(&detected);
    if (ret != ROA_OK) {
        printf("[ROAController] [EF-003-1] 센서 오류 감지\n");
        (void)OccupantData_storeDTC(ROA_DTC_SENSOR_FAIL);
        s_state = ROA_STATE_FAULT;
        return ret;
    }

    /* confidence=LOW → 루프 계속 (시뮬레이션에서는 1회만) */
    if (detected.confidence == CONFIDENCE_LOW) {
        printf("[ROAController] [미감지] confidence=LOW → 경고 미발행\n");
        return ROA_OK;
    }

    /* step 6: storeDetection */
    printf("[ROAController] step 6: storeDetection\n");
    (void)OccupantData_storeDetection(true, detected.breathRate, detected.position);

    /* step 7: 경고 단계 판단 → 1차 경고 준비 */
    printf("[ROAController] step 7: occupantPresent=true → 1차 경고 실행 준비\n");

    return executeLevel1Warning();
}

/**
 * @brief 1차 경고 실행 (step 8~9, 이후 2차 경고로 에스컬레이션)
 */
static ROAResult_t executeLevel1Warning(void)
{
    ROAResult_t ret;

    s_state = ROA_STATE_WARNING_LV1;
    printf("\n[ROAController] ── 1차 경고 (클러스터) ──\n");

    /* step 8~9: displayWarning */
    ret = ClusterIF_displayWarning(ROA_WARNING_MSG_LV1, CLUSTER_WARNING_LEVEL_1);
    if (ret != ROA_OK) {
        return ret;
    }

    /* AF-003-1: 도어 열림 / 잠금 해제 이벤트 확인 */
    if (s_params.doorOpened) {
        printf("[ROAController] [AF-003-1] 도어 열림 감지 → 경고 시퀀스 즉시 중단, ROA 종료\n");
        s_state = ROA_STATE_TERMINATED;
        return ROA_OK;
    }

    printf("[ROAController] 운전자 응답 없음 → %u초 대기 후 2차 경고 에스컬레이션\n",
           ROA_LV1_WAIT_SEC);

    return executeLevel2Warning();
}

/**
 * @brief 2차 경고 실행 — 최대 8회 × 25초 사이클 (step 10~12)
 */
static ROAResult_t executeLevel2Warning(void)
{
    ROAResult_t            ret;
    DeliveryConfirmation_t confirm;
    uint8_t                cycle;

    s_state = ROA_STATE_WARNING_LV2;
    printf("\n[ROAController] ── 2차 경고 (에스컬레이션: 경적 + 비상등 + 모바일) ──\n");
    printf("[ROAController] loop [최대 %u회 반복 × %u초 사이클]\n",
           ROA_MAX_ALARM_CYCLE, ROA_ALARM_HORN_SEC);

    for (cycle = 1u; cycle <= ROA_MAX_ALARM_CYCLE; cycle++) {
        printf("\n[ROAController] --- 2차 경고 사이클 %u/%u ---\n",
               cycle, ROA_MAX_ALARM_CYCLE);

        /* step 10~10a: activateAlarm */
        ret = AlarmIF_activateAlarm(ROA_ALARM_HORN_SEC, true);
        if (ret != ROA_OK) {
            return ret;
        }

        /* step 11~11a: sendAlert (EF-003-2: 통신 불가 시 건너뜀) */
        ret = MobileAppIF_sendAlert(ROA_VEHICLE_LOCATION, ROA_ALERT_MSG, &confirm);
        if (ret == ROA_ERR_COMM_FAIL) {
            printf("[ROAController] [EF-003-2] 모바일 통신 불가 → 경적/비상등만 반복\n");
            /* 통신 오류는 치명적이지 않음 — 루프 계속 */
        } else if (ret != ROA_OK) {
            return ret;
        }
    }

    /* step 12: 모니터링 지속 판단 완료 */
    printf("\n[ROAController] step 12: 최대 사이클 완료 — ROA 모니터링 종료\n");
    (void)AlarmIF_deactivateAlarm();
    return ROA_OK;
}
