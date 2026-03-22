/**
 * @file cluster_if.c
 * @brief «boundary» ClusterIF — 클러스터 / HUD 인터페이스 구현
 *
 * @details
 * UC-001 시퀀스 다이어그램 step 12~13을 담당한다.
 *   step 12: displayStatus(state, indicator=ON/OFF)
 *   step 13: 표시등 갱신 + 확인음 출력
 *
 *   EF-001-1: "조작 불가" 메시지 3초간 표시
 *   EF-001-2: "차일드락 시스템 점검 요망" 경고 표시
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 *
 * @assumption 실제 클러스터 CAN 프레임 대신 printf 출력으로 시뮬레이션한다.
 * @reason     임베디드 환경 없이 호스트 PC에서 컴파일·테스트 가능하게 하기 위함.
 */

#include "cluster_if.h"
#include <stdio.h>

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 잠금 상태 표시등 갱신 및 확인음 출력 (step 12~13)
 */
ECLResult_t ClusterIF_displayStatus(LockStateValue_t state, bool soundBeep)
{
    const char *stateStr     = (state == LOCK_STATE_LOCKED) ? "LOCKED" : "UNLOCKED";
    const char *indicatorStr = (state == LOCK_STATE_LOCKED) ? "ON" : "OFF";

    printf("[ClusterIF] displayStatus(state=%s, indicator=%s)\n",
           stateStr, indicatorStr);
    printf("[ClusterIF] 표시등 갱신 완료");

    if (soundBeep) {
        printf(" + 확인음 출력");
    }
    printf("\n");

    return ECL_OK;
}

/**
 * @brief 오류 경고 메시지 표시
 */
ECLResult_t ClusterIF_displayWarning(const char *msg, uint32_t durationSec)
{
    if (msg == NULL) {
        return ECL_ERR_INVALID_PARAM;
    }

    printf("[ClusterIF] displayWarning(\"%s\", %u초)\n", msg, durationSec);
    return ECL_OK;
}
