/**
 * @file occupant_data.h
 * @brief «entity» OccupantData — 탑승자 감지 결과 데이터 모델 헤더
 *
 * @details
 * UC-003 시퀀스 다이어그램의 OccupantData 엔티티 객체를 구현한다.
 * 탑승자 감지 결과를 저장하고 조회하는 데이터 모델 역할을 한다.
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef OCCUPANT_DATA_H
#define OCCUPANT_DATA_H

#include "../include/roa_types.h"
#include <time.h>

/* ============================================================
 * Structures
 * ============================================================ */

/**
 * @brief 탑승자 감지 결과 레코드 (step 6: storeDetection)
 */
typedef struct {
    bool               occupantPresent; /**< 탑승자 존재 여부 */
    uint8_t            breathRate;      /**< 호흡수 (회/분) */
    OccupantPosition_t position;        /**< 감지 위치 */
    time_t             timestamp;       /**< 감지 시각 (Unix 타임스탬프) */
    uint32_t           dtcCode;         /**< DTC 코드 (0이면 정상) */
} OccupantRecord_t;

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 엔티티 초기화
 */
void OccupantData_init(void);

/**
 * @brief 탑승자 감지 결과 저장 (시퀀스 step 6)
 * @param[in] occupantPresent 탑승자 존재 여부
 * @param[in] breathRate      호흡수 (회/분)
 * @param[in] position        감지 위치
 * @return ROA_OK on success
 */
ROAResult_t OccupantData_storeDetection(bool               occupantPresent,
                                        uint8_t            breathRate,
                                        OccupantPosition_t position);

/**
 * @brief DTC 코드 기록
 * @param[in] dtcCode DTC 코드
 * @return ROA_OK on success
 */
ROAResult_t OccupantData_storeDTC(uint32_t dtcCode);

/**
 * @brief 마지막 감지 결과 조회
 * @param[out] record 감지 결과 레코드
 * @return ROA_OK on success
 */
ROAResult_t OccupantData_getLastRecord(OccupantRecord_t *record);

#endif /* OCCUPANT_DATA_H */
