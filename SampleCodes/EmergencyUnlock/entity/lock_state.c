/**
 * @file lock_state.c
 * @brief «entity» LockState — 잠금 상태 정보 데이터 모델 구현 (UC-002)
 *
 * @details
 * UC-002 시퀀스 다이어그램 step 4, 10을 담당한다.
 *   step 4 : getCurrentState() → return currentState = LOCKED
 *   step 10: updateState(EMERGENCY_RELEASED) → return OK
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
static EULockRecord_t s_record;

/** @brief 엔티티 초기화 여부 */
static bool s_initialized = false;

/* ============================================================
 * Internal helpers
 * ============================================================ */

static const char *stateToStr(EULockState_t state)
{
    switch (state) {
        case EU_LOCK_STATE_LOCKED:            return "LOCKED";
        case EU_LOCK_STATE_UNLOCKED:          return "UNLOCKED";
        case EU_LOCK_STATE_EMERGENCY_RELEASED: return "EMERGENCY_RELEASED";
        default:                              return "UNKNOWN";
    }
}

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 엔티티 초기화 (기본 상태: LOCKED)
 */
void EULockState_init(void)
{
    (void)memset(&s_record, 0, sizeof(EULockRecord_t));
    s_record.currentState = EU_LOCK_STATE_LOCKED;
    s_record.lastUpdated  = time(NULL);
    s_initialized         = true;
    printf("[EULockState] Entity initialized (state=LOCKED)\n");
}

/**
 * @brief 현재 잠금 상태 조회 (step 4)
 */
EUResult_t EULockState_getCurrentState(EULockState_t *state)
{
    if (state == NULL) {
        return EU_ERR_INVALID_PARAM;
    }

    if (!s_initialized) {
        EULockState_init();
    }

    *state = s_record.currentState;
    printf("[EULockState] getCurrentState() → %s\n", stateToStr(*state));
    return EU_OK;
}

/**
 * @brief 잠금 상태 갱신 (step 10)
 */
EUResult_t EULockState_updateState(EULockState_t newState)
{
    if (!s_initialized) {
        EULockState_init();
    }

    s_record.currentState = newState;
    s_record.lastUpdated  = time(NULL);

    printf("[EULockState] updateState(%s) → OK\n", stateToStr(newState));
    return EU_OK;
}
