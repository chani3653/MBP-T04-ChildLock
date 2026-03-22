/**
 * @file crash_event.c
 * @brief «entity» CrashEvent — 충돌 이벤트 정보 데이터 모델 구현
 *
 * @details
 * UC-002 시퀀스 다이어그램 step 3, 3a를 담당한다.
 *   step 3 : storeCrashInfo(severity, timestamp, airbagStatus)
 *   step 3a: return OK (저장 완료)
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#include "crash_event.h"
#include <stdio.h>
#include <string.h>

/* ============================================================
 * Internal state
 * ============================================================ */

/** @brief 마지막 충돌 이벤트 레코드 */
static CrashRecord_t s_record;

/** @brief 엔티티 초기화 여부 */
static bool s_initialized = false;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 엔티티 초기화
 */
void CrashEvent_init(void)
{
    (void)memset(&s_record, 0, sizeof(CrashRecord_t));
    s_initialized = true;
    printf("[CrashEvent] Entity initialized\n");
}

/**
 * @brief 충돌 이벤트 정보 저장 (step 3)
 */
EUResult_t CrashEvent_storeCrashInfo(const CrashEventData_t *data)
{
    if (data == NULL) {
        return EU_ERR_INVALID_PARAM;
    }

    if (!s_initialized) {
        CrashEvent_init();
    }

    s_record.severity       = data->severity;
    s_record.airbagDeployed = data->airbagDeployed;
    s_record.accelerationG  = data->accelerationG;
    s_record.timestamp      = data->timestamp;
    s_record.dtcCode        = 0u;

    printf("[CrashEvent] storeCrashInfo(severity=%s, airbagDeployed=%s, "
           "accel=%.1fG, timestamp=%ld)\n",
           (data->severity == CRASH_SEVERITY_HIGH) ? "HIGH" : "LOW",
           data->airbagDeployed ? "true" : "false",
           data->accelerationG,
           (long)data->timestamp);
    printf("[CrashEvent] return OK (저장 완료)\n");

    return EU_OK;
}

/**
 * @brief DTC 코드 기록
 */
EUResult_t CrashEvent_storeDTC(uint32_t dtcCode)
{
    if (!s_initialized) {
        CrashEvent_init();
    }

    s_record.dtcCode = dtcCode;
    printf("[CrashEvent] storeDTC(0x%05X)\n", dtcCode);
    return EU_OK;
}

/**
 * @brief 마지막 충돌 이벤트 레코드 조회
 */
EUResult_t CrashEvent_getLastRecord(CrashRecord_t *record)
{
    if (record == NULL) {
        return EU_ERR_INVALID_PARAM;
    }

    if (!s_initialized) {
        return EU_ERR_INVALID_PARAM;
    }

    *record = s_record;
    return EU_OK;
}
