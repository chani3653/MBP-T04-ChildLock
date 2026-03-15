/**
 * @file roa_controller.h
 * @brief «control» ROAController — BCM/IBU 후석 탑승자 경고 컨트롤러 헤더
 *
 * @details
 * UC-003 시퀀스 다이어그램의 ROAController 제어 객체를 구현한다.
 * 시동 OFF + 잠금 완료 트리거로부터 시작하여 전체 ROA 경고 시퀀스를 조율한다.
 *
 * 주요 시나리오:
 *   - Main Success: 탑승자 감지 → 1차 경고 → 2차 경고 (경적+비상등+모바일)
 *   - AF-003-1: 1차 경고 후 도어 열림/잠금 해제 → 즉시 종료
 *   - EF-003-1: ROA 센서 자가진단 실패 → DTC 기록 후 비활성화
 *   - EF-003-2: 모바일 통신 불가 → 경적/비상등만 반복
 *   - EF-003-3: EV 보조 배터리 부족 → 마지막 1회 2차 경고 후 종료
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
 * @brief ROA 시스템 초기화 파라미터
 */
typedef struct {
    VehicleType_t vehicleType;      /**< 차량 타입 (ICE / EV) */
    bool          evBatteryLow;     /**< EV 보조 배터리 부족 여부 (EF-003-3) */
    bool          doorOpened;       /**< 도어 열림 이벤트 (AF-003-1 시뮬레이션) */
} ROAInitParams_t;

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief ROA 컨트롤러 초기화
 * @param[in] params 초기화 파라미터
 */
void ROAController_init(const ROAInitParams_t *params);

/**
 * @brief ROA 감지 모드 시작 — 전체 시퀀스 진입점 (트리거: 시동 OFF + 잠금 완료)
 * @return ROA_OK on success, error code on failure
 */
ROAResult_t ROAController_startDetectionMode(void);

/**
 * @brief 현재 ROA 시스템 상태 조회
 * @return 현재 ROAState_t 상태값
 */
ROAState_t ROAController_getState(void);

#endif /* ROA_CONTROLLER_H */
