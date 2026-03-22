/**
 * @file airbag_if.h
 * @brief «boundary» AirbagIF — 에어백 신호 수신 인터페이스 헤더
 *
 * @details
 * UC-002 시퀀스 다이어그램의 AirbagIF 경계 객체를 구현한다.
 * 에어백 ECU로부터 충돌 이벤트 신호를 수신하고
 * EmergencyController에 전달한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef AIRBAG_IF_H
#define AIRBAG_IF_H

#include "../include/eu_types.h"

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 충돌 이벤트 수신 (시퀀스 step 1~2)
 * @details 에어백 ECU가 broadcastCrashEvent를 전송하면
 *          AirbagIF가 수신하여 EmergencyController에 전달한다.
 * @param[out] event 수신된 충돌 이벤트 데이터
 * @return EU_OK on success
 */
EUResult_t AirbagIF_onCrashDetected(CrashEventData_t *event);

/**
 * @brief 시뮬레이션 충돌 이벤트 설정 (시뮬레이션 전용)
 * @param[in] severity       충돌 심각도
 * @param[in] airbagDeployed 에어백 전개 여부
 * @param[in] accelerationG  가속도 (G)
 */
void AirbagIF_setSimCrashEvent(CrashSeverity_t severity,
                               bool            airbagDeployed,
                               float           accelerationG);

#endif /* AIRBAG_IF_H */
