/**
 * @file main.c
 * @brief UC-001 Electronic Child Lock (ECL) 시뮬레이션 진입점
 *
 * @details
 * 세 가지 시나리오를 순서대로 실행하여 ECL 기능을 검증한다.
 *
 *   Scenario 1: Main Success  (버튼 누름 → UNLOCKED → LOCKED → ACK → 표시등)
 *   Scenario 2: EF-001-1      (시동 OFF 3분 초과 → 버튼 입력 Discard)
 *   Scenario 3: EF-001-2      (DCU ACK 미수신 → DTC 기록 + 경고)
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#include "controller/ecl_controller.h"
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
static void printScenarioResult(ECLResult_t result)
{
    const char *expected = "UNEXPECTED";

    if ((result == ECL_OK) ||
        (result == ECL_ERR_TIMEOUT) ||
        (result == ECL_ERR_ACK_FAIL) ||
        (result == ECL_ERR_ACTUATOR_JAM)) {
        expected = "EXPECTED";
    }

    printf("\n[RESULT] ECLController state=%d, return=%d (%s)\n",
           (int)ECLController_getState(),
           (int)result,
           expected);
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
    ECLInitParams_t params;
    ECLResult_t     ret;

    /* ----------------------------------------------------------
     * Scenario 1: Main Success Scenario
     *   - 초기 상태: UNLOCKED
     *   - 시동 OFF 경과: 0초 (정상 범위)
     *   - 오류 주입: 없음
     *   - 기대 결과: UNLOCKED → LOCKED, 클러스터 표시등 ON
     * ---------------------------------------------------------- */
    printScenarioHeader("Scenario 1: Main Success — 버튼 누름 → UNLOCKED→LOCKED");

    params.initialState        = LOCK_STATE_UNLOCKED;
    params.engineOffElapsedSec = 0u;
    params.injectAckFail       = false;
    params.injectActuatorJam   = false;

    ECLController_init(&params);
    ret = ECLController_handleButtonPress();
    printScenarioResult(ret);

    /* ----------------------------------------------------------
     * Scenario 2: EF-001-1 — 시동 OFF 3분 초과
     *   - 시동 OFF 경과: 200초 (3분 = 180초 초과)
     *   - 기대 결과: 버튼 입력 Discard + "조작 불가" 표시
     * ---------------------------------------------------------- */
    printScenarioHeader("Scenario 2: EF-001-1 — 시동 OFF 3분 초과 → 입력 Discard");

    params.initialState        = LOCK_STATE_UNLOCKED;
    params.engineOffElapsedSec = 200u;
    params.injectAckFail       = false;
    params.injectActuatorJam   = false;

    ECLController_init(&params);
    ret = ECLController_handleButtonPress();
    printScenarioResult(ret);

    /* ----------------------------------------------------------
     * Scenario 3: EF-001-2 — DCU ACK 미수신
     *   - 오류 주입: injectAckFail=true
     *   - 기대 결과: DTC 기록 + "시스템 점검 요망". 상태 변경 없음
     * ---------------------------------------------------------- */
    printScenarioHeader("Scenario 3: EF-001-2 — DCU ACK 미수신 → DTC 기록 + 경고");

    params.initialState        = LOCK_STATE_UNLOCKED;
    params.engineOffElapsedSec = 0u;
    params.injectAckFail       = true;
    params.injectActuatorJam   = false;

    ECLController_init(&params);
    ret = ECLController_handleButtonPress();
    printScenarioResult(ret);

    printf("\n[SIMULATION] 모든 시나리오 완료\n");
    return 0;
}
