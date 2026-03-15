/**
 * @file radar_sensor_if.c
 * @brief «boundary» RadarSensorIF — mmWave 레이더 수신 인터페이스 구현
 *
 * @details
 * UC-003 시퀀스 다이어그램 step 2~5를 담당한다.
 *   step 2: activateRadar(57-66 GHz ISM band)
 *   step 3: rawSignal(breathAmplitude, bodyMotion, frequency)
 *   step 4: FFT 처리 / 호흡 패턴 추출 / 노이즈 필터링
 *   step 5: occupantDetected(confidence, position, breathRate)
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 *
 * @assumption 실제 하드웨어 드라이버 대신 시뮬레이션 값으로 동작한다.
 * @reason     임베디드 환경 없이 호스트 PC에서 컴파일·테스트 가능하게 하기 위함.
 */

#include "radar_sensor_if.h"
#include <stdio.h>
#include <math.h>

/* ============================================================
 * Internal constants
 * ============================================================ */

/** @brief FFT 신뢰도 임계 호흡 진폭 */
#define BREATH_AMPLITUDE_THRESHOLD  (0.3f)

/** @brief 노이즈 필터링 임계 신체 움직임 */
#define BODY_MOTION_NOISE_THRESHOLD (0.05f)

/** @brief 정상 호흡수 범위 하한 (회/분) */
#define BREATH_RATE_MIN             (12u)

/** @brief 정상 호흡수 범위 상한 (회/분) */
#define BREATH_RATE_MAX             (25u)

/* ============================================================
 * Internal state
 * ============================================================ */

/** @brief 센서 활성화 여부 */
static bool s_radarActive = false;

/* ============================================================
 * Function Implementations
 * ============================================================ */

/**
 * @brief 레이더 센서 자가진단 수행
 */
ROAResult_t RadarSensorIF_selfDiagnostic(void)
{
    /*
     * Assumption: 실제 환경에서는 레이더 IC 레지스터를 읽어 상태를 확인한다.
     * Reason: 호스트 시뮬레이션에서는 항상 정상으로 반환한다.
     */
    printf("[RadarSensorIF] Self-diagnostic: OK\n");
    return ROA_OK;
}

/**
 * @brief 레이더 활성화 (57-66 GHz ISM band)
 */
ROAResult_t RadarSensorIF_activateRadar(void)
{
    printf("[RadarSensorIF] activateRadar(%u-%u GHz ISM band)\n",
           ROA_RADAR_FREQ_LOW_GHZ, ROA_RADAR_FREQ_HIGH_GHZ);
    s_radarActive = true;
    return ROA_OK;
}

/**
 * @brief 레이더 원시 신호 수신 (시뮬레이션)
 */
ROAResult_t RadarSensorIF_getRawSignal(RadarRawSignal_t *signal)
{
    if (signal == NULL) {
        return ROA_ERR_INVALID_PARAM;
    }

    if (!s_radarActive) {
        printf("[RadarSensorIF] ERROR: Radar is not active\n");
        return ROA_ERR_SENSOR_FAIL;
    }

    /*
     * Assumption: 시뮬레이션 값으로 탑승자가 존재하는 상황을 모사한다.
     * Reason: 실제 mmWave 드라이버 없이 동작 검증을 위함.
     */
    signal->breathAmplitude = 0.45f;   /* 정상 호흡 진폭 */
    signal->bodyMotion      = 0.08f;   /* 미세 움직임 */
    signal->frequency       = 60.0f;   /* 60 GHz ISM band */

    printf("[RadarSensorIF] rawSignal(breathAmplitude=%.2f, bodyMotion=%.2f, freq=%.1f GHz)\n",
           signal->breathAmplitude, signal->bodyMotion, signal->frequency);
    return ROA_OK;
}

/**
 * @brief FFT 처리 및 탑승자 감지 분석
 */
ROAResult_t RadarSensorIF_analyzeSignal(const RadarRawSignal_t *signal,
                                        OccupantDetectedMsg_t  *result)
{
    if ((signal == NULL) || (result == NULL)) {
        return ROA_ERR_INVALID_PARAM;
    }

    printf("[RadarSensorIF] Analyzing signal (FFT processing, noise filtering)...\n");

    /* 노이즈 필터링: 신체 움직임이 임계값 이하이면 노이즈로 간주 */
    if (signal->bodyMotion < BODY_MOTION_NOISE_THRESHOLD) {
        result->confidence = CONFIDENCE_LOW;
        result->position   = POSITION_UNKNOWN;
        result->breathRate = 0u;
        printf("[RadarSensorIF] confidence=LOW → monitoring continues\n");
        return ROA_OK;
    }

    /* 호흡 진폭 기반 탑승자 감지 판정 */
    if (signal->breathAmplitude >= BREATH_AMPLITUDE_THRESHOLD) {
        result->confidence = CONFIDENCE_HIGH;
        result->position   = POSITION_REAR_SEAT;
        /*
         * Assumption: 호흡수는 breathAmplitude 기반 단순 선형 추정으로 계산.
         * Reason: 실제 FFT 결과 없이 시뮬레이션하기 위함.
         */
        result->breathRate = (uint8_t)(BREATH_RATE_MIN +
                             (uint8_t)((signal->breathAmplitude - BREATH_AMPLITUDE_THRESHOLD)
                             * 10.0f));
        if (result->breathRate > BREATH_RATE_MAX) {
            result->breathRate = BREATH_RATE_MAX;
        }
        printf("[RadarSensorIF] occupantDetected(confidence=HIGH, position=rearSeat, breathRate=%u/min)\n",
               result->breathRate);
    } else {
        result->confidence = CONFIDENCE_LOW;
        result->position   = POSITION_UNKNOWN;
        result->breathRate = 0u;
        printf("[RadarSensorIF] confidence=LOW → monitoring continues\n");
    }

    return ROA_OK;
}

/**
 * @brief 탑승 감지 1회 사이클 실행 (step 3~5 통합)
 */
ROAResult_t RadarSensorIF_startDetectionMode(OccupantDetectedMsg_t *result)
{
    ROAResult_t      ret;
    RadarRawSignal_t rawSignal;

    if (result == NULL) {
        return ROA_ERR_INVALID_PARAM;
    }

    ret = RadarSensorIF_getRawSignal(&rawSignal);
    if (ret != ROA_OK) {
        return ret;
    }

    return RadarSensorIF_analyzeSignal(&rawSignal, result);
}
