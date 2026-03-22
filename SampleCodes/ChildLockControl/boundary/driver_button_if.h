/**
 * @file driver_button_if.h
 * @brief «boundary» DriverButtonIF — 버튼 입력부 인터페이스 헤더
 *
 * @details
 * UC-001 시퀀스 다이어그램의 DriverButtonIF 경계 객체를 구현한다.
 * 운전자의 차일드락 버튼 입력을 수신하고 컨트롤러에 전달한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef DRIVER_BUTTON_IF_H
#define DRIVER_BUTTON_IF_H

#include "../include/ecl_types.h"

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 차일드락 버튼 입력 이벤트 수신 (시퀀스 step 1~2)
 * @details 운전자가 버튼을 1회 누르면 토글 신호를 생성하여 반환한다.
 * @param[out] signal 생성된 토글 신호
 * @return ECL_OK on success, ECL_ERR_TIMEOUT if engine-off timeout exceeded
 */
ECLResult_t DriverButtonIF_buttonPressed(ToggleSignal_t *signal);

/**
 * @brief 시동 OFF 이후 경과 시간 설정 (시뮬레이션 전용)
 * @param[in] elapsedSec 시동 OFF 후 경과 시간 (초)
 */
void DriverButtonIF_setEngineOffElapsed(uint32_t elapsedSec);

#endif /* DRIVER_BUTTON_IF_H */
