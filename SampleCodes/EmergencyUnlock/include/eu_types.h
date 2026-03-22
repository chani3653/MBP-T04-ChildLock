/**
 * @file eu_types.h
 * @brief Shared type definitions for UC-002 Emergency Unlock (EU)
 *
 * @details
 * UC-002 충돌 시 비상 해제 시스템에서 사용되는
 * 공통 타입, 상수, 열거형을 정의한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef EU_TYPES_H
#define EU_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

/* ============================================================
 * Constants
 * ============================================================ */

/** @brief 비상 해제 명령 우선순위 — 최고 등급 */
#define EU_PRIORITY_HIGHEST           (0xFFu)

/** @brief 2차 충돌 위험 시 해제 지연 최소 시간 (초) — AF-002-2 */
#define EU_SECONDARY_DELAY_MIN_SEC    (5u)

/** @brief 2차 충돌 위험 시 해제 지연 최대 시간 (초) — AF-002-2 */
#define EU_SECONDARY_DELAY_MAX_SEC    (20u)

/** @brief DTC 코드: 액추에이터 물리적 파손 (EF-002-1) */
#define EU_DTC_ACTUATOR_FAIL          (0xC0021u)

/** @brief DTC 코드: CAN 버스 단선 (AF-002-1) */
#define EU_DTC_CAN_BUS_FAIL           (0xC0022u)

/* ============================================================
 * Enumerations
 * ============================================================ */

/**
 * @brief 함수 반환 코드
 */
typedef enum {
    EU_OK                  = 0,
    EU_ERR_CAN_FAIL        = 1,   /**< AF-002-1: CAN 버스 단선 */
    EU_ERR_ACTUATOR_FAIL   = 2,   /**< EF-002-1: 액추에이터 물리적 파손 */
    EU_ERR_INVALID_PARAM   = 3
} EUResult_t;

/**
 * @brief 충돌 심각도
 */
typedef enum {
    CRASH_SEVERITY_LOW  = 0,
    CRASH_SEVERITY_HIGH = 1
} CrashSeverity_t;

/**
 * @brief 비상 해제 후 도어 잠금 상태
 */
typedef enum {
    EU_LOCK_STATE_LOCKED            = 0,
    EU_LOCK_STATE_UNLOCKED          = 1,
    EU_LOCK_STATE_EMERGENCY_RELEASED = 2
} EULockState_t;

/**
 * @brief EmergencyController 내부 상태
 */
typedef enum {
    EU_STATE_IDLE       = 0,
    EU_STATE_PROCESSING = 1,
    EU_STATE_RELEASED   = 2,
    EU_STATE_FAULT      = 3
} EUState_t;

/* ============================================================
 * Structures
 * ============================================================ */

/**
 * @brief 충돌 이벤트 데이터 (step 1: broadcastCrashEvent)
 */
typedef struct {
    CrashSeverity_t severity;        /**< 충돌 심각도 */
    bool            airbagDeployed;  /**< 에어백 전개 여부 */
    float           accelerationG;   /**< 가속도 (G) */
    time_t          timestamp;       /**< 충돌 발생 시각 */
} CrashEventData_t;

/**
 * @brief DCU forceUnlatch ACK 응답
 */
typedef struct {
    bool leftUnlocked;   /**< 좌측 DCU 해제 성공 여부 */
    bool rightUnlocked;  /**< 우측 DCU 해제 성공 여부 */
} EmergencyAck_t;

#endif /* EU_TYPES_H */
