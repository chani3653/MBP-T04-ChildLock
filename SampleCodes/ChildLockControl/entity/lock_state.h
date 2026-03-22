/**
 * @file lock_state.h
 * @brief «entity» LockState — 잠금 상태 정보 데이터 모델 헤더
 *
 * @details
 * UC-001 시퀀스 다이어그램의 LockState 엔티티 객체를 구현한다.
 * 현재 도어 잠금 상태를 저장하고 조회하는 데이터 모델 역할을 한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef LOCK_STATE_H
#define LOCK_STATE_H

#include "../include/ecl_types.h"

/* ============================================================
 * Structures
 * ============================================================ */

/**
 * @brief 잠금 상태 레코드
 */
typedef struct {
    LockStateValue_t currentState;  /**< 현재 잠금 상태 */
    uint32_t         dtcCode;       /**< DTC 코드 (0이면 정상) */
    time_t           lastUpdated;   /**< 마지막 갱신 시각 */
} LockRecord_t;

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 엔티티 초기화 (기본 상태: UNLOCKED)
 */
void LockState_init(void);

/**
 * @brief 현재 잠금 상태 조회 (시퀀스 step 3)
 * @param[out] state 현재 잠금 상태
 * @return ECL_OK on success
 */
ECLResult_t LockState_getCurrentState(LockStateValue_t *state);

/**
 * @brief 잠금 상태 갱신 (시퀀스 step 11)
 * @param[in] newState 갱신할 상태
 * @return ECL_OK on success
 */
ECLResult_t LockState_updateState(LockStateValue_t newState);

/**
 * @brief DTC 코드 기록 (EF-001-2, EF-001-3 대응)
 * @param[in] dtcCode DTC 코드
 * @return ECL_OK on success
 */
ECLResult_t LockState_storeDTC(uint32_t dtcCode);

/**
 * @brief 마지막 잠금 레코드 조회
 * @param[out] record 잠금 상태 레코드
 * @return ECL_OK on success
 */
ECLResult_t LockState_getLastRecord(LockRecord_t *record);

#endif /* LOCK_STATE_H */
