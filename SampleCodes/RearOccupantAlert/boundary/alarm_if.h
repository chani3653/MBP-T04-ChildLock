/**
 * @file alarm_if.h
 * @brief «boundary» AlarmIF — 경적 / 비상등 인터페이스 헤더
 *
 * @details
 * UC-003 시퀀스 다이어그램의 AlarmIF 경계 객체를 구현한다.
 * 2차 경고 시 경적(25초)과 비상등 점멸을 제어한다.
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef ALARM_IF_H
#define ALARM_IF_H

#include "../include/roa_types.h"

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 경적 및 비상등 활성화 (시퀀스 step 10)
 * @param[in] hornDurationSec 경적 지속 시간 (초)
 * @param[in] hazardLightsOn  비상등 활성화 여부
 * @return ROA_OK on success
 */
ROAResult_t AlarmIF_activateAlarm(uint32_t hornDurationSec, bool hazardLightsOn);

/**
 * @brief 경적 및 비상등 비활성화
 * @return ROA_OK on success
 */
ROAResult_t AlarmIF_deactivateAlarm(void);

#endif /* ALARM_IF_H */
