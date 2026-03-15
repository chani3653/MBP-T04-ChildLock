/**
 * @file cluster_if.c
 * @brief «boundary» ClusterIF — 클러스터 / HUD 인터페이스 구현
 *
 * @details
 * UC-003 시퀀스 다이어그램 step 8~9를 담당한다.
 *   step 8: displayWarning(msg='뒷좌석 탑승자 확인', level=1)
 *   step 9: 경고 메시지 표시 + 경고음 출력
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 *
 * @assumption CAN 버스를 통한 실제 클러스터 통신 대신 콘솔 출력으로 시뮬레이션.
 * @reason     호스트 환경에서 동작 검증이 가능하게 하기 위함.
 */

#include "cluster_if.h"
#include <stdio.h>

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 클러스터에 경고 메시지 표시 및 경고음 출력
 */
ROAResult_t ClusterIF_displayWarning(const char *msg, uint8_t level)
{
    if (msg == NULL) {
        return ROA_ERR_INVALID_PARAM;
    }

    printf("[ClusterIF] displayWarning(msg='%s', level=%u)\n", msg, level);
    printf("[ClusterIF] >>> 경고 메시지 표시 + 경고음 출력 (1차 경고)\n");
    return ROA_OK;
}

/**
 * @brief 클러스터 시스템 상태 표시 (ROA 점검 요망)
 */
ROAResult_t ClusterIF_displayStatus(const char *statusMsg, bool nextIgnOn)
{
    if (statusMsg == NULL) {
        return ROA_ERR_INVALID_PARAM;
    }

    printf("[ClusterIF] displayStatus(\"%s\", nextIgnOn=%s)\n",
           statusMsg, nextIgnOn ? "true" : "false");
    printf("[ClusterIF] >>> DTC 기록 완료, 다음 시동 시 경고 표시 예약\n");
    return ROA_OK;
}
