/**
 * @file mobile_app_if.c
 * @brief «boundary» MobileAppIF — 블루링크 / Kia Connect 모바일 앱 인터페이스 구현
 *
 * @details
 * UC-003 시퀀스 다이어그램 step 11~11a를 담당한다.
 *   step 11 : sendAlert(type=SMS+PUSH, vehicleLocation, msg='뒷좌석 탑승자 감지')
 *   step 11a: return deliveryConfirmation(SMS=sent, PUSH=delivered)
 *
 * 예외 처리:
 *   EF-003-2: 모바일 앱 통신 불가 → ROA_ERR_COMM_FAIL 반환
 *             상위 컨트롤러에서 2차 경고(경적/비상등)만 반복 처리
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 *
 * @assumption 실제 네트워크 통신 대신 콘솔 출력으로 시뮬레이션.
 * @reason     호스트 환경에서 동작 검증이 가능하게 하기 위함.
 */

#include "mobile_app_if.h"
#include <stdio.h>

/* ============================================================
 * Internal state (simulated connectivity flag)
 * ============================================================ */

/**
 * @brief 모바일 앱 통신 가능 여부 (시뮬레이션용)
 *
 * Assumption: 실제 환경에서는 LTE/5G 연결 상태를 확인한다.
 * Reason: EF-003-2 예외 흐름 테스트를 위해 외부에서 설정 가능하게 한다.
 */
static bool s_mobileConnected = true;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief SMS + 푸시 알림 전송
 */
ROAResult_t MobileAppIF_sendAlert(const char             *vehicleLocation,
                                  const char             *alertMsg,
                                  DeliveryConfirmation_t *confirmation)
{
    if ((vehicleLocation == NULL) || (alertMsg == NULL) || (confirmation == NULL)) {
        return ROA_ERR_INVALID_PARAM;
    }

    printf("[MobileAppIF] sendAlert(type=SMS+PUSH, location='%s', msg='%s')\n",
           vehicleLocation, alertMsg);

    if (!s_mobileConnected) {
        /* EF-003-2: 모바일 앱 통신 불가 */
        printf("[MobileAppIF] ERROR: Connection failed — skipping SMS/PUSH\n");
        confirmation->smsSent       = false;
        confirmation->pushDelivered = false;
        return ROA_ERR_COMM_FAIL;
    }

    confirmation->smsSent       = true;
    confirmation->pushDelivered = true;
    printf("[MobileAppIF] return deliveryConfirmation(SMS=sent, PUSH=delivered)\n");
    return ROA_OK;
}
