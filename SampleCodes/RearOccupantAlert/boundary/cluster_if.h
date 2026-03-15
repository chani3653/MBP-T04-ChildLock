/**
 * @file cluster_if.h
 * @brief «boundary» ClusterIF — 클러스터 / HUD 인터페이스 헤더
 *
 * @details
 * UC-003 시퀀스 다이어그램의 ClusterIF 경계 객체를 구현한다.
 * 1차 경고 메시지를 클러스터(계기판) 및 HUD에 출력한다.
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef CLUSTER_IF_H
#define CLUSTER_IF_H

#include "../include/roa_types.h"

/* ============================================================
 * Constants
 * ============================================================ */

/** @brief 1차 경고 레벨 */
#define CLUSTER_WARNING_LEVEL_1 (1u)

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 클러스터에 경고 메시지 표시 (시퀀스 step 8)
 * @param[in] msg   표시할 경고 메시지 문자열
 * @param[in] level 경고 레벨 (1 = 1차 경고)
 * @return ROA_OK on success
 */
ROAResult_t ClusterIF_displayWarning(const char *msg, uint8_t level);

/**
 * @brief 클러스터 시스템 상태 표시 (EF-003-1: ROA 점검 요망 알림)
 * @param[in] statusMsg   상태 메시지
 * @param[in] nextIgnOn   다음 시동 시 표시 여부
 * @return ROA_OK on success
 */
ROAResult_t ClusterIF_displayStatus(const char *statusMsg, bool nextIgnOn);

#endif /* CLUSTER_IF_H */
