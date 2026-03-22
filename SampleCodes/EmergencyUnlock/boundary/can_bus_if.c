/**
 * @file can_bus_if.c
 * @brief «boundary» CANBusIF — CAN 통신 인터페이스 구현 (UC-002)
 *
 * @details
 * UC-002 시퀀스 다이어그램 step 6~9를 담당한다.
 *   step 6 : sendEmergencyUnlock(PRIORITY=HIGHEST, target=ALL_REAR) → CANBusIF
 *   step 7a: forceUnlatch(doorLeft)
 *   step 7b: forceUnlatch(doorRight)
 *   step 9 : return ACK(left=UNLOCKED, right=UNLOCKED)
 *   step 9a: return emergencyACK(allDoorsUnlocked=true) → EmergencyController
 *
 *   AF-002-1: CAN 버스 단선 → EU_ERR_CAN_FAIL 반환
 *   EF-002-1: 액추에이터 물리적 파손 → EU_ERR_ACTUATOR_FAIL 반환
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

/** @brief CAN 버스 단선 오류 주입 플래그 */
static bool s_injectCanFail      = false;

/** @brief 액추에이터 파손 오류 주입 플래그 */
static bool s_injectActuatorFail = false;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 오류 주입 설정 (시뮬레이션 전용)
 */
void CANBusIF_setErrorInject(bool injectCanFail, bool injectActuatorFail)
{
    s_injectCanFail      = injectCanFail;
    s_injectActuatorFail = injectActuatorFail;
}

/**
 * @brief 비상 해제 명령 전송 (step 6~9)
 */
EUResult_t CANBusIF_sendEmergencyUnlock(uint8_t        priority,
                                        EmergencyAck_t *ack)
{
    EUResult_t retLeft;
    EUResult_t retRight;

    if (ack == NULL) {
        return EU_ERR_INVALID_PARAM;
    }

    printf("[CANBusIF] sendEmergencyUnlock(PRIORITY=0x%02X, target=ALL_REAR)\n",
           priority);

    /* AF-002-1: CAN 버스 단선 오류 주입 */
    if (s_injectCanFail) {
        printf("[CANBusIF] AF-002-1: CAN 버스 단선 감지 → EU_ERR_CAN_FAIL\n");
        ack->leftUnlocked  = false;
        ack->rightUnlocked = false;
        return EU_ERR_CAN_FAIL;
    }

    /* step 7a: 좌측 DCU forceUnlatch */
    retLeft = DoorActuatorIF_forceUnlatch(DOOR_TARGET_LEFT);

    /* step 7b: 우측 DCU forceUnlatch */
    retRight = DoorActuatorIF_forceUnlatch(DOOR_TARGET_RIGHT);

    /* EF-002-1: 액추에이터 파손 */
    if ((retLeft == EU_ERR_ACTUATOR_FAIL) || (retRight == EU_ERR_ACTUATOR_FAIL)) {
        printf("[CANBusIF] EF-002-1: 액추에이터 물리적 파손 → DCU NACK\n");
        ack->leftUnlocked  = false;
        ack->rightUnlocked = false;
        return EU_ERR_ACTUATOR_FAIL;
    }

    ack->leftUnlocked  = (retLeft  == EU_OK);
    ack->rightUnlocked = (retRight == EU_OK);

    printf("[CANBusIF] return ACK(left=%s, right=%s)\n",
           ack->leftUnlocked  ? "UNLOCKED" : "FAIL",
           ack->rightUnlocked ? "UNLOCKED" : "FAIL");
    printf("[CANBusIF] return emergencyACK(allDoorsUnlocked=%s)\n",
           (ack->leftUnlocked && ack->rightUnlocked) ? "true" : "false");

    if (!ack->leftUnlocked || !ack->rightUnlocked) {
        return EU_ERR_ACTUATOR_FAIL;
    }

    return EU_OK;
}
