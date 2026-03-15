/**
 * @file main.c
 * @brief UC-003 Rear Occupant Alert (ROA) 시뮬레이션 진입점
 *
 * @details
 * 세 가지 시나리오를 순서대로 실행하여 ROA 기능을 검증한다.
 *
 *   Scenario 1: Main Success (탑승자 감지 → 1차 → 2차 경고)
 *   Scenario 2: AF-003-1    (1차 경고 후 도어 열림 → 즉시 종료)
 *   Scenario 3: EF-003-3    (EV 배터리 부족 → 마지막 1회 경고)
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 */

#include "controller/roa_controller.h"
#include <stdio.h>

/* ============================================================
 * Internal helpers
 * ============================================================ */

/**
 * @brief 시나리오 헤더 출력
 * @param[in] title 시나리오 제목
 */
static void printScenarioHeader(const char *title)
{
    printf("\n");
    printf("============================================================\n");
    printf("  %s\n", title);
    printf("============================================================\n");
}

/**
 * @brief 시나리오 결과 출력
 * @param[in] result 실행 결과 코드
 */
static void printScenarioResult(ROAResult_t result)
{
    printf("\n[RESULT] ROAController state=%d, return=%d (%s)\n",
           (int)ROAController_getState(),
           (int)result,
           (result == ROA_OK || result == ROA_ERR_BATTERY_LOW || result == ROA_ERR_COMM_FAIL)
               ? "EXPECTED" : "UNEXPECTED");
}

/* ============================================================
 * main
 * ============================================================ */

/**
 * @brief 프로그램 진입점
 * @return 0 on success
 */
int main(void)
{
    ROAInitParams_t params;
    ROAResult_t     ret;

    /* ----------------------------------------------------------
     * Scenario 1: Main Success Scenario
     *   - ICE 차량, 도어 닫힘, 배터리 정상
     *   - 탑승자 감지 → 1차 경고 → 2차 경고 8사이클
     * ---------------------------------------------------------- */
    printScenarioHeader("Scenario 1: Main Success — 탑승자 감지 → 1차 → 2차 경고");

    params.vehicleType  = VEHICLE_TYPE_ICE;
    params.evBatteryLow = false;
    params.doorOpened   = false;

    ROAController_init(&params);
    ret = ROAController_startDetectionMode();
    printScenarioResult(ret);

    /* ----------------------------------------------------------
     * Scenario 2: AF-003-1 — 1차 경고 후 도어 열림
     *   - ICE 차량, 도어 열림 시뮬레이션
     *   - 1차 경고 직후 시퀀스 종료 확인
     * ---------------------------------------------------------- */
    printScenarioHeader("Scenario 2: AF-003-1 — 1차 경고 후 도어 열림 → 즉시 종료");

    params.vehicleType  = VEHICLE_TYPE_ICE;
    params.evBatteryLow = false;
    params.doorOpened   = true;

    ROAController_init(&params);
    ret = ROAController_startDetectionMode();
    printScenarioResult(ret);

    /* ----------------------------------------------------------
     * Scenario 3: EF-003-3 — EV 보조 배터리 부족
     *   - EV 차량, 배터리 부족
     *   - 마지막 1회 2차 경고 후 종료 확인
     * ---------------------------------------------------------- */
    printScenarioHeader("Scenario 3: EF-003-3 — EV 보조 배터리 부족 → 마지막 1회 경고 후 종료");

    params.vehicleType  = VEHICLE_TYPE_EV;
    params.evBatteryLow = true;
    params.doorOpened   = false;

    ROAController_init(&params);
    ret = ROAController_startDetectionMode();
    printScenarioResult(ret);

    printf("\n[SIMULATION] 모든 시나리오 완료\n");
    return 0;
}
