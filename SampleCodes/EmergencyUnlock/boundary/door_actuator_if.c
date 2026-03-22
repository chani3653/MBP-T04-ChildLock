/**
 * @file door_actuator_if.c
 * @brief «boundary» DoorActuatorIF — 도어 DCU(좌/우) 인터페이스 구현 (UC-002)
 *
 * @details
 * UC-002 시퀀스 다이어그램 step 7a, 7b, 8, 9를 담당한다.
 *   step 7a: forceUnlatch(doorLeft)
 *   step 7b: forceUnlatch(doorRight)
 *   step 8 : 액추에이터 해제 방향 구동 / 내부 핸들 연결 기구 해제 위치 이동
 *   step 9 : return ACK(left=UNLOCKED, right=UNLOCKED)
 *
 *   EF-002-1: 액추에이터 물리적 파손 → NACK + EU_ERR_ACTUATOR_FAIL
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
 * Internal state (오류 주입용)
 * ============================================================ */

/** @brief 액추에이터 파손 오류 주입 플래그 */
static bool s_injectFail = false;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 액추에이터 파손 오류 주입 설정
 */
void DoorActuatorIF_setActuatorFail(bool inject)
{
    s_injectFail = inject;
}

/**
 * @brief 도어 래치 강제 해제 구동 (step 7a / 7b)
 */
EUResult_t DoorActuatorIF_forceUnlatch(EUDoorTarget_t target)
{
    const char *doorName = (target == DOOR_TARGET_LEFT) ? "doorLeft" : "doorRight";

    printf("[DoorActuatorIF] forceUnlatch(%s)\n", doorName);

    /* EF-002-1: 액추에이터 물리적 파손 오류 주입 */
    if (s_injectFail) {
        printf("[DoorActuatorIF] EF-002-1: 액추에이터 물리적 파손 → NACK+오류코드 (%s)\n",
               doorName);
        return EU_ERR_ACTUATOR_FAIL;
    }

    /*
     * Assumption: 실제 환경에서는 모터 드라이버 IC를 해제 방향으로 제어한다.
     * Reason: 호스트 시뮬레이션에서는 항상 정상 ACK를 반환한다.
     */
    printf("[DoorActuatorIF] 액추에이터 해제 방향 구동 완료 (%s)\n", doorName);
    printf("[DoorActuatorIF] return ACK(%s=UNLOCKED)\n", doorName);

    return EU_OK;
}
