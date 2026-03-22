/**
 * @file lock_state.c
 * @brief «entity» LockState — 잠금 상태 정보 데이터 모델 구현
 *
 * @details
 * UC-001 시퀀스 다이어그램 step 3, 11을 담당한다.
 *   step 3 : getCurrentState() → return currentState (UNLOCKED / LOCKED)
 *   step 11: updateState(newState) → return OK
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#include "lock_state.h"
#include <stdio.h>
#include <string.h>

/* ============================================================
 * Internal state
 * ============================================================ */

/** @brief 현재 잠금 레코드 */
static LockRecord_t s_record;

/** @brief 엔티티 초기화 여부 */
static bool s_initialized = false;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 엔티티 초기화
 */
void LockState_init(void)
{
    (void)memset(&s_record, 0, sizeof(LockRecord_t));
    s_record.currentState = LOCK_STATE_UNLOCKED;
    s_record.dtcCode      = 0u;
    s_record.lastUpdated  = time(NULL);
    s_initialized         = true;
    printf("[LockState] Entity initialized (state=UNLOCKED)\n");
}

/**
 * @brief 현재 잠금 상태 조회 (step 3)
 */
ECLResult_t LockState_getCurrentState(LockStateValue_t *state)
{
    if (state == NULL) {
        return ECL_ERR_INVALID_PARAM;
    }

    if (!s_initialized) {
        LockState_init();
    }

    *state = s_record.currentState;
    printf("[LockState] getCurrentState() → %s\n",
           (*state == LOCK_STATE_LOCKED) ? "LOCKED" : "UNLOCKED");
    return ECL_OK;
}

/**
 * @brief 잠금 상태 갱신 (step 11)
 */
ECLResult_t LockState_updateState(LockStateValue_t newState)
{
    if (!s_initialized) {
        LockState_init();
    }

    s_record.currentState = newState;
    s_record.lastUpdated  = time(NULL);

    printf("[LockState] updateState(%s) → OK\n",
           (newState == LOCK_STATE_LOCKED) ? "LOCKED" : "UNLOCKED");
    return ECL_OK;
}

/**
 * @brief DTC 코드 기록
 */
ECLResult_t LockState_storeDTC(uint32_t dtcCode)
{
    if (!s_initialized) {
        LockState_init();
    }

    s_record.dtcCode = dtcCode;
    printf("[LockState] storeDTC(0x%05X)\n", dtcCode);
    return ECL_OK;
}

/**
 * @brief 마지막 잠금 레코드 조회
 */
ECLResult_t LockState_getLastRecord(LockRecord_t *record)
{
    if (record == NULL) {
        return ECL_ERR_INVALID_PARAM;
    }

    if (!s_initialized) {
        return ECL_ERR_INVALID_PARAM;
    }

    *record = s_record;
    return ECL_OK;
}
