# Mobius Bootcamp PBL Project Team04

현대모비스 × 멋쟁이사자처럼 **Mobius Bootcamp PBL Team 04** 레포지토리입니다.  
본 프로젝트는 **ISO 26262 기반 기능 안전 소프트웨어 개발 프로세스**를 학습하고,  
UML 기반 설계부터 구현, 빌드, 실행 검증까지 수행하는 것을 목표로 합니다.

---

# 🔗 Wiki

👉 프로젝트 상세 문서는 GitHub Wiki에서 확인할 수 있습니다.

- 📊 [System Benchmark & Analysis](../../wiki/System_Benchmark_and_Analysis)
- 📄 [Software Requirement Specification (SRS)](../../wiki/SwRW_Software_Requirement_Specification)
- 🧩 [Software Detailed Design (SDD)](../../wiki/SwDD_Software_Detailed_Design)

---

# 1. Project Overview

## 프로젝트명

**Rear Occupant Alert (ROA) Simulation**

## 프로젝트 목표

- 차량 내 **Rear Occupant Alert(ROA)** 기능을 C 언어로 시뮬레이션
- UML 기반 설계를 바탕으로 기능 구현
- 요구사항 기반 시나리오 검증
- GitHub Actions를 이용한 **자동 빌드 및 실행 검증(CI)** 환경 구성

## 주요 키워드

- ISO 26262
- Functional Safety
- UML
- C Language
- Makefile
- GitHub Actions
- Verification

---

# 2. Team Information

## 팀명

**락앤락**

## 팀장

**박찬휘**

## 팀원

1. 김희도
2. 박찬휘
3. 오태림

---

# 3. Repository Structure

```text
.
├── AGENTS.md
├── DEVELOPMENT.md
├── Github.md
├── README.md
├── doc
│   └── uml_diagram
│       ├── defaults
│       ├── FlowChart
│       └── AddFailureState
└── SampleCodes
    └── RearOccupantAlert
        ├── Makefile
        ├── main.c
        ├── include
        │   └── roa_types.h
        ├── controller
        │   ├── roa_controller.c
        │   └── roa_controller.h
        ├── entity
        │   ├── occupant_data.c
        │   └── occupant_data.h
        └── boundary
            ├── radar_sensor_if.c
            ├── radar_sensor_if.h
            ├── cluster_if.c
            ├── cluster_if.h
            ├── alarm_if.c
            ├── alarm_if.h
            ├── mobile_app_if.c
            └── mobile_app_if.h
```

---

# 4. Development Scope

본 저장소는 다음 흐름을 기준으로 구성되었습니다.

1. 요구사항 정의
2. UML 기반 설계
3. 모듈 구조 설계
4. C 언어 구현
5. Makefile 기반 빌드
6. 시나리오 실행 및 결과 검증
7. GitHub Actions 기반 CI 자동화

---

# 5. Rear Occupant Alert Simulation

## 기능 설명

ROA(Rear Occupant Alert)는 차량 내 후석 탑승자 또는 잔류 상황을 감지하고,  
조건에 따라 경고를 수행하는 기능입니다.

본 프로젝트에서는 세 가지 대표 시나리오를 통해 기능을 검증합니다.

### Scenario 1. Main Success

- ICE 차량
- 도어 닫힘
- 배터리 정상
- 탑승자 감지 후 1차 경고, 이후 2차 경고 수행

### Scenario 2. AF-003-1

- ICE 차량
- 1차 경고 후 도어 열림
- 경고 시퀀스 즉시 종료

### Scenario 3. EF-003-3

- EV 차량
- 보조 배터리 부족
- 마지막 1회 경고 후 종료

---

# 6. Build & Run

## 실행 위치

```bash
cd SampleCodes/RearOccupantAlert
```

## 빌드

```bash
make
```

## 실행

```bash
make run
```

## 클린

```bash
make clean
```

---

# 7. Build Environment

- Language: **C11**
- Compiler: **gcc**
- Build Tool: **Make**
- OS: Linux / macOS / GitHub Actions Ubuntu Runner

예상 컴파일 옵션:

```make
-std=c11 -Wall -Wextra -Werror
```

헤더 include 경로:

```make
-I. -Iinclude -Icontroller -Ientity -Iboundary
```

---

# 8. CI / GitHub Actions

본 프로젝트는 GitHub Actions를 통해 **자동 빌드 및 실행 검증**을 수행합니다.

## CI 목적

- 코드 변경 시 자동 빌드
- 실행 가능 여부 확인
- 기본적인 무결성 검증
- 지속적인 품질 확인

## CI 수행 항목

1. 저장소 체크아웃
2. 빌드 도구 설치
3. make clean
4. make
5. make run

## Workflow 위치

```text
.github/workflows/c-cpp.yml
```

## 기대 효과

- 수동 빌드 누락 방지
- 변경사항 반영 즉시 검증
- 시뮬레이션 코드 실행 보장

---

# 9. UML & Documents

문서 및 다이어그램은 다음 경로에 존재합니다.

```text
doc/uml_diagram/defaults
doc/uml_diagram/FlowChart
doc/uml_diagram/AddFailureState
```

---

# 10. Development Notes

본 프로젝트는 단순 코드 구현이 아닌  
**설계 → 구현 → 검증 → 자동화** 흐름을 경험하는 데 목적이 있습니다.

핵심 역량:

- 기능 안전 관점 설계
- 모듈 구조 설계
- 요구사항 기반 개발
- CI 기반 자동 검증

---

# 11. Future Improvements

- make test 기반 테스트 자동화
- 단위 테스트 / 통합 테스트 확장
- 커버리지 측정 도입
- 요구사항 추적성 강화
- CI 테스트 리포트 자동화

---

# 12. Maintainers

- 박찬휘
- Team 04 Members

---

# 13. License

본 프로젝트는 Mobius Bootcamp PBL 학습 목적의 저장소입니다.
