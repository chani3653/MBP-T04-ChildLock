/**
 * @file crash_event.h
 * @brief «entity» CrashEvent — 충돌 이벤트 정보 데이터 모델 헤더
 *
 * @details
 * UC-002 시퀀스 다이어그램의 CrashEvent 엔티티 객체를 구현한다.
 * 충돌 이벤트 정보를 저장하고 조회하는 데이터 모델 역할을 한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef CRASH_EVENT_H
#define CRASH_EVENT_H

#include "../include/eu_types.h"

/* ============================================================
 * Structures
 * ============================================================ */

/**
 * @brief 충돌 이벤트 저장 레코드
 */
typedef struct {
    CrashSeverity_t severity;        /**< 충돌 심각도 */
    bool            airbagDeployed;  /**< 에어백 전개 여부 */
    float           accelerationG;   /**< 가속도 (G) */
    time_t          timestamp;       /**< 충돌 발생 시각 */
    uint32_t        dtcCode;         /**< DTC 코드 (0이면 정상) */
} CrashRecord_t;

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 엔티티 초기화
 */
void CrashEvent_init(void);

/**
 * @brief 충돌 이벤트 정보 저장 (시퀀스 step 3)
 * @param[in] data 충돌 이벤트 데이터
 * @return EU_OK on success
 */
EUResult_t CrashEvent_storeCrashInfo(const CrashEventData_t *data);

/**
 * @brief DTC 코드 기록
 * @param[in] dtcCode DTC 코드
 * @return EU_OK on success
 */
EUResult_t CrashEvent_storeDTC(uint32_t dtcCode);

/**
 * @brief 마지막 충돌 이벤트 레코드 조회
 * @param[out] record 충돌 이벤트 레코드
 * @return EU_OK on success
 */
EUResult_t CrashEvent_getLastRecord(CrashRecord_t *record);

#endif /* CRASH_EVENT_H */
