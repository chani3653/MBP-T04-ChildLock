/**
 * @file alarm_if.c
 * @brief «boundary» AlarmIF — 경적 / 비상등 인터페이스 구현
 *
 * @details
 * UC-003 시퀀스 다이어그램 step 10~10a를 담당한다.
 *   step 10 : activateAlarm(horn=25sec, hazardLights=ON)
 *   step 10a: 경적 25초 울림 + 비상등 점멸
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 *
 * @assumption 실제 CAN 메시지 대신 콘솔 출력으로 시뮬레이션.
 * @reason     호스트 환경에서 동작 검증이 가능하게 하기 위함.
 */

#include "alarm_if.h"
#include <stdio.h>

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 경적 및 비상등 활성화
 */
ROAResult_t AlarmIF_activateAlarm(uint32_t hornDurationSec, bool hazardLightsOn)
{
    printf("[AlarmIF] activateAlarm(horn=%usec, hazardLights=%s)\n",
           hornDurationSec, hazardLightsOn ? "ON" : "OFF");
    printf("[AlarmIF] >>> 경적 %u초 울림\n", hornDurationSec);

    if (hazardLightsOn) {
        printf("[AlarmIF] >>> 비상등 점멸 시작\n");
    }

    return ROA_OK;
}

/**
 * @brief 경적 및 비상등 비활성화
 */
ROAResult_t AlarmIF_deactivateAlarm(void)
{
    printf("[AlarmIF] deactivateAlarm() — 경적·비상등 중단\n");
    return ROA_OK;
}
