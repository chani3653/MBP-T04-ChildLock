/**
 * @file roa_controller.h
 * @brief «controller» ROAController — 후석 탑승자 감지 경고 제어 로직 헤더
 *
 * @details
 * UC-003 시퀀스 다이어그램의 ROAController 제어 객체를 구현한다.
 * Boundary / Entity 객체를 조율하여 감지 → 1차 경고 → 2차 경고 흐름을 실행한다.
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef ROA_CONTROLLER_H
#define ROA_CONTROLLER_H

#include "../include/roa_types.h"

/* ============================================================
 * Structures
 * ============================================================ */

/**
 * @brief ROAController 초기화 파라미터
 */
typedef struct {
    VehicleType_t vehicleType;   /**< 차량 타입 (ICE / EV) */
    bool          evBatteryLow;  /**< EV 보조 배터리 부족 여부 (EF-003-3) */
    bool          doorOpened;    /**< 도어 열림 여부 (AF-003-1) */
} ROAInitParams_t;

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 컨트롤러 초기화
 * @param[in] params 초기화 파라미터
 */
void ROAController_init(const ROAInitParams_t *params);

/**
 * @brief 감지 모드 시작 (전체 시퀀스 실행)
 * @return ROA_OK on success,
 *         ROA_ERR_BATTERY_LOW if EV battery low,
 *         ROA_ERR_COMM_FAIL if mobile alert failed (non-fatal),
 *         ROA_ERR_SENSOR_FAIL if sensor diagnostic failed
 */
ROAResult_t ROAController_startDetectionMode(void);

/**
 * @brief 현재 컨트롤러 상태 조회
 * @return 현재 ROAState_t
 */
ROAState_t ROAController_getState(void);

#endif /* ROA_CONTROLLER_H */
