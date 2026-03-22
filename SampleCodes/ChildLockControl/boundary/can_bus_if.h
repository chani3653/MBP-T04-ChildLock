/**
 * @file can_bus_if.h
 * @brief «boundary» CANBusIF — CAN 통신 인터페이스 헤더
 *
 * @details
 * UC-001 시퀀스 다이어그램의 CANBusIF 경계 객체를 구현한다.
 * ECLController의 잠금/해제 명령을 CAN 메시지로 변환하여
 * DoorActuatorIF(DCU)에 전달하고 ACK를 수신한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef CAN_BUS_IF_H
#define CAN_BUS_IF_H

#include "../include/ecl_types.h"

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 잠금/해제 명령 CAN 전송 (시퀀스 step 5)
 * @param[in]  cmd    잠금 또는 해제 명령
 * @param[in]  target 대상 도어 (LEFT / RIGHT / BOTH)
 * @param[out] ack    DCU 응답 결과
 * @return ECL_OK on success,
 *         ECL_ERR_ACK_FAIL if DCU ACK not received (EF-001-2),
 *         ECL_ERR_ACTUATOR_JAM if actuator jam detected (EF-001-3)
 */
ECLResult_t CANBusIF_sendCommand(CANCommand_t  cmd,
                                 DoorTarget_t  target,
                                 DoorAck_t    *ack);

/**
 * @brief 시뮬레이션 오류 조건 주입 (시뮬레이션 전용)
 * @param[in] injectAckFail  true: ACK 미수신 오류 주입
 * @param[in] injectJamFail  true: 액추에이터 걸림 오류 주입
 */
void CANBusIF_setErrorInject(bool injectAckFail, bool injectJamFail);

#endif /* CAN_BUS_IF_H */
