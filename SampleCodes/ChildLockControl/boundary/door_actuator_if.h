/**
 * @file door_actuator_if.h
 * @brief «boundary» DoorActuatorIF — 도어 DCU(좌/우) 인터페이스 헤더
 *
 * @details
 * UC-001 시퀀스 다이어그램의 DoorActuatorIF 경계 객체를 구현한다.
 * CAN 명령을 받아 도어 래치 액추에이터를 기계적으로 구동한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef DOOR_ACTUATOR_IF_H
#define DOOR_ACTUATOR_IF_H

#include "../include/ecl_types.h"

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 도어 래치 액추에이터 구동 (시퀀스 step 6a / 6b)
 * @details 래치 내 액추에이터를 기계적으로 구동하여 잠금/해제를 수행한다.
 * @param[in] cmd    잠금 또는 해제 명령
 * @param[in] target 대상 도어 (LEFT 또는 RIGHT)
 * @return ECL_OK on success (ACK 정상),
 *         ECL_ERR_ACTUATOR_JAM if mechanical jam detected
 */
ECLResult_t DoorActuatorIF_actuateLatch(CANCommand_t cmd, DoorTarget_t target);

#endif /* DOOR_ACTUATOR_IF_H */
