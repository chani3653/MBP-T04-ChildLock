/**
 * @file can_bus_if.h
 * @brief «boundary» CANBusIF — CAN 통신 인터페이스 헤더 (UC-002)
 *
 * @details
 * UC-002 시퀀스 다이어그램의 CANBusIF 경계 객체를 구현한다.
 * EmergencyController의 비상 해제 명령을 최고 우선순위 CAN 메시지로 전송하고
 * DoorActuatorIF(DCU)를 통해 forceUnlatch를 실행한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef EU_CAN_BUS_IF_H
#define EU_CAN_BUS_IF_H

#include "../include/eu_types.h"

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 비상 해제 명령 전송 (시퀀스 step 6~9)
 * @param[in]  priority 명령 우선순위 (EU_PRIORITY_HIGHEST)
 * @param[out] ack      DCU 응답 (allDoorsUnlocked)
 * @return EU_OK on success,
 *         EU_ERR_CAN_FAIL if CAN bus disconnected (AF-002-1),
 *         EU_ERR_ACTUATOR_FAIL if actuator physically damaged (EF-002-1)
 */
EUResult_t CANBusIF_sendEmergencyUnlock(uint8_t        priority,
                                        EmergencyAck_t *ack);

/**
 * @brief 시뮬레이션 오류 조건 주입 (시뮬레이션 전용)
 * @param[in] injectCanFail       true: CAN 버스 단선 오류 주입
 * @param[in] injectActuatorFail  true: 액추에이터 파손 오류 주입
 */
void CANBusIF_setErrorInject(bool injectCanFail, bool injectActuatorFail);

#endif /* EU_CAN_BUS_IF_H */
