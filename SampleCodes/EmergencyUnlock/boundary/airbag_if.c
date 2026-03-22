/**
 * @file airbag_if.c
 * @brief «boundary» AirbagIF — 에어백 신호 수신 인터페이스 구현
 *
 * @details
 * UC-002 시퀀스 다이어그램 step 1~2를 담당한다.
 *   step 1: broadcastCrashEvent(accelerationData, airbagDeployed=true)
 *           에어백 ECU → AirbagIF
 *   step 2: onCrashDetected(severity=HIGH, airbagDeployed=true)
 *           AirbagIF → EmergencyController
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 *
 * @assumption 실제 에어백 ECU CAN 메시지 대신 시뮬레이션 값으로 동작한다.
 * @reason     임베디드 환경 없이 호스트 PC에서 컴파일·테스트 가능하게 하기 위함.
 */

#include "airbag_if.h"
#include <stdio.h>

/* ============================================================
 * Internal state (시뮬레이션용)
 * ============================================================ */

/** @brief 시뮬레이션 충돌 심각도 */
static CrashSeverity_t s_simSeverity       = CRASH_SEVERITY_HIGH;

/** @brief 시뮬레이션 에어백 전개 여부 */
static bool            s_simAirbagDeployed = true;

/** @brief 시뮬레이션 가속도 (G) */
static float           s_simAccelerationG  = 25.0f;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 시뮬레이션 충돌 이벤트 설정
 */
void AirbagIF_setSimCrashEvent(CrashSeverity_t severity,
                               bool            airbagDeployed,
                               float           accelerationG)
{
    s_simSeverity       = severity;
    s_simAirbagDeployed = airbagDeployed;
    s_simAccelerationG  = accelerationG;
}

/**
 * @brief 충돌 이벤트 수신 (step 1~2)
 */
EUResult_t AirbagIF_onCrashDetected(CrashEventData_t *event)
{
    if (event == NULL) {
        return EU_ERR_INVALID_PARAM;
    }

    printf("[AirbagIF] broadcastCrashEvent(accelerationData=%.1fG, airbagDeployed=%s) 수신\n",
           s_simAccelerationG,
           s_simAirbagDeployed ? "true" : "false");

    event->severity       = s_simSeverity;
    event->airbagDeployed = s_simAirbagDeployed;
    event->accelerationG  = s_simAccelerationG;
    event->timestamp      = time(NULL);

    printf("[AirbagIF] onCrashDetected(severity=%s, airbagDeployed=%s) → EmergencyController\n",
           (event->severity == CRASH_SEVERITY_HIGH) ? "HIGH" : "LOW",
           event->airbagDeployed ? "true" : "false");

    return EU_OK;
}
