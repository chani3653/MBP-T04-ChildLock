/**
 * @file roa_types.h
 * @brief Shared type definitions for UC-003 Rear Occupant Alert (ROA)
 *
 * @details
 * UC-003 후석 탑승자 감지 경고 시스템에서 사용되는
 * 공통 타입, 상수, 열거형을 정의한다.
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef ROA_TYPES_H
#define ROA_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

/* ============================================================
 * Constants
 * ============================================================ */

/** @brief 레이더 주파수 밴드 하한 (GHz) */
#define ROA_RADAR_FREQ_LOW_GHZ        (57u)

/** @brief 레이더 주파수 밴드 상한 (GHz) */
#define ROA_RADAR_FREQ_HIGH_GHZ       (66u)

/** @brief 내연기관 차량 최대 모니터링 시간 (초) */
#define ROA_MAX_MONITOR_ICE_SEC       (86400u)   /* 24h */

/** @brief EV 차량 최대 모니터링 시간 (초) */
#define ROA_MAX_MONITOR_EV_SEC        (600u)     /* 10min */

/** @brief 2차 경고 최대 반복 횟수 */
#define ROA_MAX_ALARM_CYCLE           (8u)

/** @brief 2차 경고 경적 지속 시간 (초) */
#define ROA_ALARM_HORN_SEC            (25u)

/** @brief confidence 임계값 — 이 값 이상이면 HIGH */
#define ROA_CONFIDENCE_HIGH_THRESHOLD (70u)

/** @brief DTC 코드: ROA 센서 자가진단 실패 */
#define ROA_DTC_SENSOR_FAIL           (0xC0001u)

/* ============================================================
 * Enumerations
 * ============================================================ */

/**
 * @brief 신뢰도 등급
 */
typedef enum {
    CONFIDENCE_LOW  = 0,
    CONFIDENCE_HIGH = 1
} ConfidenceLevel_t;

/**
 * @brief 후석 탑승자 감지 위치
 */
typedef enum {
    POSITION_UNKNOWN    = 0,
    POSITION_REAR_SEAT  = 1,
    POSITION_FRONT_SEAT = 2
} OccupantPosition_t;

/**
 * @brief 차량 타입 (모니터링 시간 기준)
 */
typedef enum {
    VEHICLE_TYPE_ICE = 0,   /**< Internal Combustion Engine */
    VEHICLE_TYPE_EV  = 1    /**< Electric Vehicle */
} VehicleType_t;

/**
 * @brief ROA 시스템 상태
 */
typedef enum {
    ROA_STATE_IDLE        = 0,
    ROA_STATE_MONITORING  = 1,
    ROA_STATE_WARNING_LV1 = 2,
    ROA_STATE_WARNING_LV2 = 3,
    ROA_STATE_FAULT       = 4,
    ROA_STATE_TERMINATED  = 5
} ROAState_t;

/**
 * @brief 함수 반환 코드
 */
typedef enum {
    ROA_OK                 = 0,
    ROA_ERR_SENSOR_FAIL    = 1,
    ROA_ERR_COMM_FAIL      = 2,
    ROA_ERR_BATTERY_LOW    = 3,
    ROA_ERR_INVALID_PARAM  = 4
} ROAResult_t;

/**
 * @brief 모바일 알림 전송 결과
 */
typedef enum {
    ALERT_SENT      = 0,
    ALERT_FAIL_CONN = 1
} AlertResult_t;

/* ============================================================
 * Structures
 * ============================================================ */

/**
 * @brief 레이더 원시 신호 데이터
 */
typedef struct {
    float    breathAmplitude;   /**< 호흡 진폭 */
    float    bodyMotion;        /**< 신체 움직임 크기 */
    float    frequency;         /**< 감지 주파수 (GHz) */
} RadarRawSignal_t;

/**
 * @brief 탑승자 감지 결과 (occupantDetected 메시지)
 */
typedef struct {
    ConfidenceLevel_t  confidence;   /**< 신뢰도 */
    OccupantPosition_t position;     /**< 감지 위치 */
    uint8_t            breathRate;   /**< 호흡수 (회/분) */
} OccupantDetectedMsg_t;

#endif /* ROA_TYPES_H */
