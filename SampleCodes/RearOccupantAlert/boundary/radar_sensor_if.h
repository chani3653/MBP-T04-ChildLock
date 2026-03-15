/**
 * @file radar_sensor_if.h
 * @brief «boundary» RadarSensorIF — mmWave 레이더 수신 인터페이스 헤더
 *
 * @details
 * UC-003 시퀀스 다이어그램의 RadarSensorIF 경계 객체를 구현한다.
 * ROA 센서(mmWave Radar)로부터 원시 신호를 수신하고
 * FFT 처리·호흡 패턴 추출·노이즈 필터링을 수행한다.
 *
 * @note Implemented Date: 2026-03-15
 * @note Model Version   : claude-sonnet-4-6
 */

#ifndef RADAR_SENSOR_IF_H
#define RADAR_SENSOR_IF_H

#include "../include/roa_types.h"

/* ============================================================
 * Function Declarations
 * ============================================================ */

/**
 * @brief 레이더 센서 자가진단 수행 (시퀀스 step EF-003-1 대응)
 * @return ROA_OK if sensor is healthy, ROA_ERR_SENSOR_FAIL otherwise
 */
ROAResult_t RadarSensorIF_selfDiagnostic(void);

/**
 * @brief 레이더 활성화 명령 전달 (시퀀스 step 2)
 * @return ROA_OK on success
 */
ROAResult_t RadarSensorIF_activateRadar(void);

/**
 * @brief 레이더 원시 신호 수신 (시퀀스 step 3)
 * @param[out] signal 수신된 원시 신호 데이터
 * @return ROA_OK on success, ROA_ERR_SENSOR_FAIL on read error
 */
ROAResult_t RadarSensorIF_getRawSignal(RadarRawSignal_t *signal);

/**
 * @brief FFT 처리 및 탑승자 감지 분석 (시퀀스 step 4 → 5)
 * @param[in]  signal  원시 레이더 신호
 * @param[out] result  감지 결과 메시지
 * @return ROA_OK on success
 */
ROAResult_t RadarSensorIF_analyzeSignal(const RadarRawSignal_t *signal,
                                        OccupantDetectedMsg_t  *result);

/**
 * @brief 탑승 감지 루프 1회 실행 (시퀀스 step 3~5 통합)
 * @param[out] result 감지 결과
 * @return ROA_OK on success, ROA_ERR_SENSOR_FAIL on sensor error
 */
ROAResult_t RadarSensorIF_startDetectionMode(OccupantDetectedMsg_t *result);

#endif /* RADAR_SENSOR_IF_H */
