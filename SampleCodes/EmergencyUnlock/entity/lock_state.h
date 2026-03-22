/**
 * @file lock_state.h
 * @brief «entity» LockState — 잠금 상태 정보 데이터 모델 헤더 (UC-002)
 *
 * @details
 * UC-002 시퀀스 다이어그램의 LockState 엔티티 객체를 구현한다.
 * 비상 해제 전 잠금 상태를 조회하고, 해제 후 상태를 EMERGENCY_RELEASED로 갱신한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef EU_LOCK_STATE_H
#define EU_LOCK_STATE_H

#include "../include/eu_types.h"

/* ============================================================
 * Structures
 * ============================================================ */

/**
 * @brief 잠금 상태 레코드 (UC-002)
 */
typedef struct {
    EULockState_t currentState;  /**< 현재 잠금 상태 */
    time_t        lastUpdated;   /**< 마지막 갱신 시각 */
} EULockRecord_t;

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 엔티티 초기화 (기본 상태: LOCKED — 충돌 전 잠금 상태)
 */
void EULockState_init(void);

/**
 * @brief 현재 잠금 상태 조회 (시퀀스 step 4)
 * @param[out] state 현재 잠금 상태
 * @return EU_OK on success
 */
EUResult_t EULockState_getCurrentState(EULockState_t *state);

/**
 * @brief 잠금 상태 갱신 (시퀀스 step 10)
 * @param[in] newState 갱신할 상태
 * @return EU_OK on success
 */
EUResult_t EULockState_updateState(EULockState_t newState);

#endif /* EU_LOCK_STATE_H */
