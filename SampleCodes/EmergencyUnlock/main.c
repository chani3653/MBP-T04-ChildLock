/**
 * @file main.c
 * @brief UC-002 Emergency Unlock (EU) 시뮬레이션 진입점
 *
 * @details
 * 세 가지 시나리오를 순서대로 실행하여 비상 해제 기능을 검증한다.
 *
 *   Scenario 1: Main Success  (충돌 감지 → 비상 해제 → EMERGENCY_RELEASED)
 *   Scenario 2: AF-002-1      (CAN 버스 단선 → EU_ERR_CAN_FAIL)
 *   Scenario 3: EF-002-1      (액추에이터 파손 → DTC 기록 + eCall 요청)
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#include "controller/emergency_controller.h"
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
static void printScenarioResult(EUResult_t result)
{
    const char *expected = "UNEXPECTED";

    if ((result == EU_OK) ||
        (result == EU_ERR_CAN_FAIL) ||
        (result == EU_ERR_ACTUATOR_FAIL)) {
        expected = "EXPECTED";
    }

    printf("\n[RESULT] EmergencyController state=%d, return=%d (%s)\n",
           (int)EmergencyController_getState(),
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
    EUInitParams_t params;
    EUResult_t     ret;

    /* ----------------------------------------------------------
     * Scenario 1: Main Success Scenario
     *   - 에어백 전개 (severity=HIGH, airbagDeployed=true)
     *   - 오류 주입: 없음
     *   - 기대 결과: 비상 해제 → EMERGENCY_RELEASED
     * ---------------------------------------------------------- */
    printScenarioHeader("Scenario 1: Main Success — 충돌 감지 → 비상 해제 → EMERGENCY_RELEASED");

    params.injectCanFail       = false;
    params.injectActuatorFail  = false;
    params.secondaryImpactRisk = false;

    EmergencyController_init(&params);
    ret = EmergencyController_handleCrashEvent();
    printScenarioResult(ret);

    /* ----------------------------------------------------------
     * Scenario 2: AF-002-1 — CAN 버스 단선
     *   - 오류 주입: injectCanFail=true
     *   - 기대 결과: EU_ERR_CAN_FAIL + 아날로그 직접 전송 안내
     * ---------------------------------------------------------- */
    printScenarioHeader("Scenario 2: AF-002-1 — CAN 버스 단선 → EU_ERR_CAN_FAIL");

    params.injectCanFail       = true;
    params.injectActuatorFail  = false;
    params.secondaryImpactRisk = false;

    EmergencyController_init(&params);
    ret = EmergencyController_handleCrashEvent();
    printScenarioResult(ret);

    /* ----------------------------------------------------------
     * Scenario 3: EF-002-1 — 액추에이터 물리적 파손
     *   - 오류 주입: injectActuatorFail=true
     *   - 기대 결과: DTC 기록 + eCall "도어 해제 실패" 포함
     * ---------------------------------------------------------- */
    printScenarioHeader("Scenario 3: EF-002-1 — 액추에이터 파손 → DTC 기록 + eCall");

    params.injectCanFail       = false;
    params.injectActuatorFail  = true;
    params.secondaryImpactRisk = false;

    EmergencyController_init(&params);
    ret = EmergencyController_handleCrashEvent();
    printScenarioResult(ret);

    printf("\n[SIMULATION] 모든 시나리오 완료\n");
    return 0;
}
