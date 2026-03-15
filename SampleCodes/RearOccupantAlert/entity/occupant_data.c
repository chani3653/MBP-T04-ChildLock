/**
 * @file occupant_data.c
 * @brief «entity» OccupantData — 탑승자 감지 결과 데이터 모델 구현
 *
 * @details
 * UC-003 시퀀스 다이어그램 step 6을 담당한다.
 *   step 6 : storeDetection(occupantPresent=true, breathRate, timestamp)
 *   step 6a: return OK
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 */

#include "occupant_data.h"
#include <stdio.h>
#include <string.h>

/* ============================================================
 * Internal state
 * ============================================================ */

/** @brief 마지막 탑승자 감지 레코드 */
static OccupantRecord_t s_lastRecord;

/** @brief 엔티티 초기화 여부 */
static bool s_initialized = false;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 엔티티 초기화
 */
void OccupantData_init(void)
{
    (void)memset(&s_lastRecord, 0, sizeof(OccupantRecord_t));
    s_initialized = true;
    printf("[OccupantData] Entity initialized\n");
}

/**
 * @brief 탑승자 감지 결과 저장
 */
ROAResult_t OccupantData_storeDetection(bool               occupantPresent,
                                        uint8_t            breathRate,
                                        OccupantPosition_t position)
{
    if (!s_initialized) {
        OccupantData_init();
    }

    s_lastRecord.occupantPresent = occupantPresent;
    s_lastRecord.breathRate      = breathRate;
    s_lastRecord.position        = position;
    s_lastRecord.timestamp       = time(NULL);
    s_lastRecord.dtcCode         = 0u;

    printf("[OccupantData] storeDetection(occupantPresent=%s, breathRate=%u/min, timestamp=%ld)\n",
           occupantPresent ? "true" : "false",
           breathRate,
           (long)s_lastRecord.timestamp);
    printf("[OccupantData] return OK\n");

    return ROA_OK;
}

/**
 * @brief DTC 코드 기록
 */
ROAResult_t OccupantData_storeDTC(uint32_t dtcCode)
{
    if (!s_initialized) {
        OccupantData_init();
    }

    s_lastRecord.dtcCode = dtcCode;
    printf("[OccupantData] storeDTC(0x%05X)\n", dtcCode);
    return ROA_OK;
}

/**
 * @brief 마지막 감지 결과 조회
 */
ROAResult_t OccupantData_getLastRecord(OccupantRecord_t *record)
{
    if (record == NULL) {
        return ROA_ERR_INVALID_PARAM;
    }

    if (!s_initialized) {
        return ROA_ERR_INVALID_PARAM;
    }

    *record = s_lastRecord;
    return ROA_OK;
}
