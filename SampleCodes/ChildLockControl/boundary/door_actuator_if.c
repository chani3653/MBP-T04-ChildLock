/**
 * @file door_actuator_if.c
 * @brief «boundary» DoorActuatorIF — 도어 DCU(좌/우) 인터페이스 구현
 *
 * @details
 * UC-001 시퀀스 다이어그램 step 6a, 6b, 7, 8을 담당한다.
 *   step 6a: actuateLatch(LOCK/UNLOCK, doorLeft)
 *   step 6b: actuateLatch(LOCK/UNLOCK, doorRight)
 *   step 7 : 래치 내 액추에이터 기계적 구동
 *   step 8 : return ACK(left_OK, right_OK)
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 *
 * @assumption 실제 하드웨어 액추에이터 드라이버 대신 시뮬레이션 값으로 동작한다.
 * @reason     임베디드 환경 없이 호스트 PC에서 컴파일·테스트 가능하게 하기 위함.
 */

#include "door_actuator_if.h"
#include <stdio.h>

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 도어 래치 액추에이터 구동 (step 6a / 6b)
 */
ECLResult_t DoorActuatorIF_actuateLatch(CANCommand_t cmd, DoorTarget_t target)
{
    const char *doorName = (target == DOOR_TARGET_LEFT) ? "doorLeft" : "doorRight";
    const char *cmdName  = (cmd == CAN_CMD_LOCK) ? "LOCK" : "UNLOCK";

    printf("[DoorActuatorIF] actuateLatch(%s, %s)\n", cmdName, doorName);

    /*
     * Assumption: 실제 환경에서는 모터 드라이버 IC를 제어하여 래치를 구동한다.
     * Reason: 호스트 시뮬레이션에서는 항상 정상 ACK를 반환한다.
     */
    printf("[DoorActuatorIF] 래치 내 액추에이터 기계적 구동 완료 (%s)\n", doorName);
    printf("[DoorActuatorIF] return ACK(%s=OK)\n", doorName);

    return ECL_OK;
}
