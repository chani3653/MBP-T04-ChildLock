/**
 * @file mobile_app_if.h
 * @brief «boundary» MobileAppIF — 블루링크 / Kia Connect 모바일 앱 인터페이스 헤더
 *
 * @details
 * UC-003 시퀀스 다이어그램의 MobileAppIF 경계 객체를 구현한다.
 * SMS 및 푸시 알림을 통해 운전자에게 후석 탑승자 감지를 알린다.
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef MOBILE_APP_IF_H
#define MOBILE_APP_IF_H

#include "../include/roa_types.h"

/* ============================================================
 * Structures
 * ============================================================ */

/**
 * @brief 모바일 알림 전송 확인 결과 (step 11a)
 */
typedef struct {
    bool smsSent;       /**< SMS 전송 성공 여부 */
    bool pushDelivered; /**< 푸시 알림 전달 성공 여부 */
} DeliveryConfirmation_t;

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief SMS + 푸시 알림 전송 (시퀀스 step 11)
 * @param[in]  vehicleLocation 차량 위치 정보 문자열
 * @param[in]  alertMsg        경고 메시지 문자열
 * @param[out] confirmation    전송 결과 확인
 * @return ROA_OK on success, ROA_ERR_COMM_FAIL on communication error
 */
ROAResult_t MobileAppIF_sendAlert(const char            *vehicleLocation,
                                  const char            *alertMsg,
                                  DeliveryConfirmation_t *confirmation);

#endif /* MOBILE_APP_IF_H */
