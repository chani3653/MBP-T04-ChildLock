/**
 * @file ecl_types.h
 * @brief Shared type definitions for UC-001 Electronic Child Lock (ECL)
 *
 * @details
 * UC-001 도어 잠금/해제 제어 시스템에서 사용되는
 * 공통 타입, 상수, 열거형을 정의한다.
 *
 * @note Implemented Date: 2026-03-22
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef ECL_TYPES_H
#define ECL_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

/* ============================================================
 * Constants
 * ============================================================ */

/** @brief 시동 OFF 후 버튼 조작 허용 제한 시간 (초) = 3분 */
#define ECL_ENGINE_OFF_TIMEOUT_SEC    (180u)

/** @brief CAN ACK 수신 대기 최대 시간 (ms) */
#define ECL_CAN_ACK_TIMEOUT_MS        (200u)

/** @brief DTC 코드: 양쪽 DCU ACK 미수신 (EF-001-2) */
#define ECL_DTC_ACK_FAIL              (0xC0011u)

/** @brief DTC 코드: 액추에이터 기계적 걸림 (EF-001-3) */
#define ECL_DTC_ACTUATOR_JAM          (0xC0012u)

/** @brief 클러스터 "조작 불가" 메시지 표시 시간 (초) */
#define ECL_CLUSTER_NOTICE_SEC        (3u)

/* ============================================================
 * Enumerations
 * ============================================================ */

/**
 * @brief 함수 반환 코드
 */
typedef enum {
    ECL_OK                 = 0,
    ECL_ERR_TIMEOUT        = 1,   /**< EF-001-1: 시동 OFF 3분 초과 */
    ECL_ERR_ACK_FAIL       = 2,   /**< EF-001-2: DCU ACK 미수신 */
    ECL_ERR_ACTUATOR_JAM   = 3,   /**< EF-001-3: 액추에이터 기계적 걸림 */
    ECL_ERR_INVALID_PARAM  = 4
} ECLResult_t;

/**
 * @brief 도어 잠금 상태
 */
typedef enum {
    LOCK_STATE_UNLOCKED = 0,
    LOCK_STATE_LOCKED   = 1
} LockStateValue_t;

/**
 * @brief ECL 컨트롤러 내부 상태
 */
typedef enum {
    ECL_STATE_IDLE       = 0,
    ECL_STATE_PROCESSING = 1,
    ECL_STATE_LOCKED     = 2,
    ECL_STATE_UNLOCKED   = 3,
    ECL_STATE_FAULT      = 4
} ECLState_t;

/**
 * @brief 토글 신호 종류
 */
typedef enum {
    TOGGLE_SIGNAL_LOCK   = 0,
    TOGGLE_SIGNAL_UNLOCK = 1
} ToggleSignal_t;

/**
 * @brief CAN 명령 종류
 */
typedef enum {
    CAN_CMD_LOCK   = 0,
    CAN_CMD_UNLOCK = 1
} CANCommand_t;

/**
 * @brief 도어 대상 (좌/우/양쪽)
 */
typedef enum {
    DOOR_TARGET_LEFT  = 0,
    DOOR_TARGET_RIGHT = 1,
    DOOR_TARGET_BOTH  = 2
} DoorTarget_t;

/* ============================================================
 * Structures
 * ============================================================ */

/**
 * @brief DCU ACK 응답 (step 8: return ACK)
 */
typedef struct {
    bool leftOK;    /**< 좌측 DCU ACK 성공 여부 */
    bool rightOK;   /**< 우측 DCU ACK 성공 여부 */
} DoorAck_t;

#endif /* ECL_TYPES_H */
