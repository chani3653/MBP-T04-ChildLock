/**
 * @file can_bus_if.c
 * @brief «boundary» CANBusIF — CAN 통신 인터페이스 구현
 *
 * @details
 * UC-001 시퀀스 다이어그램 step 5~9를 담당한다.
 *   step 5 : sendCommand(LOCK/UNLOCK, target=LEFT+RIGHT) → CANBusIF
 *   step 6a: actuateLatch(LOCK/UNLOCK, doorLeft)
 *   step 6b: actuateLatch(LOCK/UNLOCK, doorRight)
 *   step 8 : return ACK(left_OK, right_OK)
 *   step 9 : return doorACK(left=OK, right=OK) → ECLController
 *
 *   EF-001-2: DCU ACK 미수신 → ECL_ERR_ACK_FAIL 반환
 *   EF-001-3: 액추에이터 걸림 → ECL_ERR_ACTUATOR_JAM 반환
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 *
 * @assumption 실제 CAN 드라이버 대신 시뮬레이션 응답으로 동작한다.
 * @reason     임베디드 환경 없이 호스트 PC에서 컴파일·테스트 가능하게 하기 위함.
 */

#include "can_bus_if.h"
#include "door_actuator_if.h"
#include <stdio.h>

/* ============================================================
 * Internal state (오류 주입용)
 * ============================================================ */

/** @brief ACK 미수신 오류 주입 플래그 */
static bool s_injectAckFail = false;

/** @brief 액추에이터 걸림 오류 주입 플래그 */
static bool s_injectJamFail = false;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 오류 주입 설정 (시뮬레이션 전용)
 */
void CANBusIF_setErrorInject(bool injectAckFail, bool injectJamFail)
{
    s_injectAckFail = injectAckFail;
    s_injectJamFail = injectJamFail;
}

/**
 * @brief 잠금/해제 명령 CAN 전송 및 ACK 수신 (step 5~9)
 */
ECLResult_t CANBusIF_sendCommand(CANCommand_t  cmd,
                                 DoorTarget_t  target,
                                 DoorAck_t    *ack)
{
    ECLResult_t retLeft;
    ECLResult_t retRight;

    if (ack == NULL) {
        return ECL_ERR_INVALID_PARAM;
    }

    printf("[CANBusIF] sendCommand(%s, target=%s)\n",
           (cmd == CAN_CMD_LOCK) ? "LOCK" : "UNLOCK",
           (target == DOOR_TARGET_BOTH)  ? "LEFT+RIGHT" :
           (target == DOOR_TARGET_LEFT)  ? "LEFT" : "RIGHT");

    /* EF-001-3: 액추에이터 걸림 오류 주입 */
    if (s_injectJamFail) {
        printf("[CANBusIF] EF-001-3: 액추에이터 기계적 걸림 감지 (과전류)\n");
        ack->leftOK  = false;
        ack->rightOK = false;
        return ECL_ERR_ACTUATOR_JAM;
    }

    /* EF-001-2: ACK 미수신 오류 주입 */
    if (s_injectAckFail) {
        printf("[CANBusIF] EF-001-2: DCU ACK 미수신 (CAN 오류 / 액추에이터 고장)\n");
        ack->leftOK  = false;
        ack->rightOK = false;
        return ECL_ERR_ACK_FAIL;
    }

    /* step 6a: 좌측 DCU actuateLatch */
    retLeft = DoorActuatorIF_actuateLatch(cmd, DOOR_TARGET_LEFT);

    /* step 6b: 우측 DCU actuateLatch */
    retRight = DoorActuatorIF_actuateLatch(cmd, DOOR_TARGET_RIGHT);

    ack->leftOK  = (retLeft  == ECL_OK);
    ack->rightOK = (retRight == ECL_OK);

    printf("[CANBusIF] return doorACK(left=%s, right=%s)\n",
           ack->leftOK ? "OK" : "FAIL",
           ack->rightOK ? "OK" : "FAIL");

    if (!ack->leftOK || !ack->rightOK) {
        return ECL_ERR_ACK_FAIL;
    }

    return ECL_OK;
}
