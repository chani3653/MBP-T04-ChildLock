/**
 * @file ecl_controller.h
 * @brief «controller» ECLController — 도어 잠금/해제 제어 로직 헤더
 *
 * @details
 * UC-001 시퀀스 다이어그램의 ECLController 제어 객체를 구현한다.
 * BCM/IBU 역할을 담당하며 Boundary / Entity 객체를 조율하여
 * 버튼 입력 → 토글 판단 → CAN 명령 → 상태 저장 → 표시 흐름을 실행한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef ECL_CONTROLLER_H
#define ECL_CONTROLLER_H

#include "../include/ecl_types.h"

/* ============================================================
 * Structures
 * ============================================================ */

/**
 * @brief ECLController 초기화 파라미터
 */
typedef struct {
    LockStateValue_t initialState;       /**< 초기 잠금 상태 */
    uint32_t         engineOffElapsedSec;/**< 시동 OFF 후 경과 시간 (EF-001-1) */
    bool             injectAckFail;      /**< ACK 미수신 오류 주입 (EF-001-2) */
    bool             injectActuatorJam;  /**< 액추에이터 걸림 오류 주입 (EF-001-3) */
} ECLInitParams_t;

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 컨트롤러 초기화
 * @param[in] params 초기화 파라미터
 */
void ECLController_init(const ECLInitParams_t *params);

/**
 * @brief 버튼 입력 처리 — 전체 UC-001 시퀀스 실행 (step 2~13)
 * @return ECL_OK on success,
 *         ECL_ERR_TIMEOUT if engine-off timeout (EF-001-1),
 *         ECL_ERR_ACK_FAIL if DCU ACK not received (EF-001-2),
 *         ECL_ERR_ACTUATOR_JAM if actuator jam (EF-001-3)
 */
ECLResult_t ECLController_handleButtonPress(void);

/**
 * @brief 현재 컨트롤러 상태 조회
 * @return 현재 ECLState_t
 */
ECLState_t ECLController_getState(void);

#endif /* ECL_CONTROLLER_H */
