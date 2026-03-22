/**
 * @file emergency_controller.h
 * @brief «controller» EmergencyController — 충돌 시 비상 해제 제어 로직 헤더
 *
 * @details
 * UC-002 시퀀스 다이어그램의 EmergencyController 제어 객체를 구현한다.
 * BCM/IBU 역할을 담당하며 Boundary / Entity 객체를 조율하여
 * 충돌 감지 → 비상 판단 → 강제 해제 → 상태 갱신 흐름을 실행한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef EMERGENCY_CONTROLLER_H
#define EMERGENCY_CONTROLLER_H

#include "../include/eu_types.h"

/* ============================================================
 * Structures
 * ============================================================ */

/**
 * @brief EmergencyController 초기화 파라미터
 */
typedef struct {
    bool injectCanFail;       /**< CAN 버스 단선 오류 주입 (AF-002-1) */
    bool injectActuatorFail;  /**< 액추에이터 파손 오류 주입 (EF-002-1) */
    bool secondaryImpactRisk; /**< 2차 충돌 위험 여부 (AF-002-2) */
} EUInitParams_t;

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 컨트롤러 초기화
 * @param[in] params 초기화 파라미터
 */
void EmergencyController_init(const EUInitParams_t *params);

/**
 * @brief 충돌 이벤트 처리 — 전체 UC-002 시퀀스 실행 (step 2~10)
 * @return EU_OK on success,
 *         EU_ERR_CAN_FAIL if CAN bus disconnected (AF-002-1),
 *         EU_ERR_ACTUATOR_FAIL if actuator physically damaged (EF-002-1)
 */
EUResult_t EmergencyController_handleCrashEvent(void);

/**
 * @brief 현재 컨트롤러 상태 조회
 * @return 현재 EUState_t
 */
EUState_t EmergencyController_getState(void);

#endif /* EMERGENCY_CONTROLLER_H */
