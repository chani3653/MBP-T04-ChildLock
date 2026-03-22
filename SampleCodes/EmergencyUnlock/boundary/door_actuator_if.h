/**
 * @file door_actuator_if.h
 * @brief «boundary» DoorActuatorIF — 도어 DCU(좌/우) 인터페이스 헤더 (UC-002)
 *
 * @details
 * UC-002 시퀀스 다이어그램의 DoorActuatorIF 경계 객체를 구현한다.
 * 비상 해제 명령을 받아 도어 래치를 강제 해제 방향으로 구동한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef EU_DOOR_ACTUATOR_IF_H
#define EU_DOOR_ACTUATOR_IF_H

#include "../include/eu_types.h"

/* ============================================================
 * Enumerations
 * ============================================================ */

/**
 * @brief 도어 대상 (좌/우)
 */
typedef enum {
    DOOR_TARGET_LEFT  = 0,
    DOOR_TARGET_RIGHT = 1
} EUDoorTarget_t;

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 도어 래치 강제 해제 구동 (시퀀스 step 7a / 7b)
 * @details 액추에이터를 해제 방향으로 강제 구동하여
 *          내부 핸들 연결 기구를 해제 위치로 이동시킨다.
 * @param[in] target 대상 도어 (LEFT 또는 RIGHT)
 * @return EU_OK on success (ACK 정상),
 *         EU_ERR_ACTUATOR_FAIL if physically damaged (NACK)
 */
EUResult_t DoorActuatorIF_forceUnlatch(EUDoorTarget_t target);

/**
 * @brief 액추에이터 파손 오류 주입 (시뮬레이션 전용)
 * @param[in] inject true: 파손 오류 주입
 */
void DoorActuatorIF_setActuatorFail(bool inject);

#endif /* EU_DOOR_ACTUATOR_IF_H */
