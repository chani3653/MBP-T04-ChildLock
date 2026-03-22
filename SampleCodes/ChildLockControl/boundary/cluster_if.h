/**
 * @file cluster_if.h
 * @brief «boundary» ClusterIF — 클러스터 / HUD 인터페이스 헤더
 *
 * @details
 * UC-001 시퀀스 다이어그램의 ClusterIF 경계 객체를 구현한다.
 * 잠금/해제 상태 표시등 갱신 및 확인음 출력,
 * 오류 상황의 경고 메시지 표시를 담당한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef CLUSTER_IF_ECL_H
#define CLUSTER_IF_ECL_H

#include "../include/ecl_types.h"

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 잠금 상태 표시등 갱신 및 확인음 출력 (시퀀스 step 12~13)
 * @param[in] state      현재 잠금 상태 (표시등 ON/OFF 결정)
 * @param[in] soundBeep  확인음 출력 여부
 * @return ECL_OK on success
 */
ECLResult_t ClusterIF_displayStatus(LockStateValue_t state, bool soundBeep);

/**
 * @brief 오류 경고 메시지 표시 (EF-001-1, EF-001-2 대응)
 * @param[in] msg         표시할 경고 메시지 문자열
 * @param[in] durationSec 메시지 표시 시간 (초)
 * @return ECL_OK on success
 */
ECLResult_t ClusterIF_displayWarning(const char *msg, uint32_t durationSec);

#endif /* CLUSTER_IF_ECL_H */
