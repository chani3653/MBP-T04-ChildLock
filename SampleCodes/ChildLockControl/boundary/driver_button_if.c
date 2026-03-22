/**
 * @file driver_button_if.c
 * @brief «boundary» DriverButtonIF — 버튼 입력부 인터페이스 구현
 *
 * @details
 * UC-001 시퀀스 다이어그램 step 1~2를 담당한다.
 *   step 1: 차일드락 버튼 1회 누름 (운전자 → DriverButtonIF)
 *   step 2: buttonPressed(toggle_signal) → ECLController
 *
 *   EF-001-1: 시동 OFF 후 3분(180초) 초과 시 버튼 입력 Discard
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 *
 * @assumption 실제 하드웨어 GPIO 인터럽트 대신 시뮬레이션 값으로 동작한다.
 * @reason     임베디드 환경 없이 호스트 PC에서 컴파일·테스트 가능하게 하기 위함.
 */

#include "driver_button_if.h"
#include <stdio.h>

/* ============================================================
 * Internal state
 * ============================================================ */

/** @brief 시동 OFF 후 경과 시간 (시뮬레이션용) */
static uint32_t s_engineOffElapsedSec = 0u;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 시동 OFF 경과 시간 설정 (시뮬레이션 전용)
 */
void DriverButtonIF_setEngineOffElapsed(uint32_t elapsedSec)
{
    s_engineOffElapsedSec = elapsedSec;
}

/**
 * @brief 차일드락 버튼 입력 이벤트 수신 (step 1~2)
 */
ECLResult_t DriverButtonIF_buttonPressed(ToggleSignal_t *signal)
{
    if (signal == NULL) {
        return ECL_ERR_INVALID_PARAM;
    }

    printf("[DriverButtonIF] 차일드락 버튼 1회 누름 감지\n");

    /* EF-001-1: 시동 OFF 후 3분 초과 → 입력 Discard */
    if (s_engineOffElapsedSec > ECL_ENGINE_OFF_TIMEOUT_SEC) {
        printf("[DriverButtonIF] EF-001-1: 시동 OFF %u초 초과 → 입력 Discard\n",
               ECL_ENGINE_OFF_TIMEOUT_SEC);
        return ECL_ERR_TIMEOUT;
    }

    /*
     * Assumption: 버튼 1회 누름은 항상 토글 신호를 생성한다.
     * Reason: 호스트 시뮬레이션에서 실제 GPIO 상태 대신 고정 신호를 사용.
     */
    *signal = TOGGLE_SIGNAL_LOCK;   /* 컨트롤러가 현재 상태 기반으로 토글 결정 */

    printf("[DriverButtonIF] buttonPressed(toggle_signal) → ECLController\n");
    return ECL_OK;
}
