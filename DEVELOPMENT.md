# Development Environment Definition

## 1. Overview

본 프로젝트는 **ISO 26262 기반 Automotive Software Development Process**를 학습하기 위한  
**SW 개발 시뮬레이션 프로젝트**이다.

UML 기반 설계를 수행한 후 이를 기반으로 간단한 C 코드를 구현하고  
단위 테스트를 통해 동작을 검증하는 것을 목표로 한다.

본 프로젝트에서 구현되는 기능 예시는 다음과 같다.

- 충돌 감지 시 비상 도어 해제 기능
- 후석 탑승자 감지 경고 (ROA)

해당 기능들은 UML Sequence Diagram 기반으로 설계되며  
설계와 코드 간 **Traceability**를 유지한다.

---

# 2. Programming Language

본 프로젝트는 **C Language (ISO C11)** 을 기반으로 구현한다.

선정 이유:

- Automotive ECU 개발에서 C 언어 사용이 일반적
- Embedded 환경에서 높은 성능 및 제어 가능
- ISO 26262 적용 사례 다수 존재

사용 표준
C Standard : ISO/IEC 9899:2011 (C11)

---

# 3. Compiler Environment

본 프로젝트에서는 다음 컴파일 환경을 사용한다.

| 항목       | 환경          |
| ---------- | ------------- |
| Compiler   | GCC           |
| Version    | GCC 11 이상   |
| OS         | macOS / Linux |
| Build Tool | Make          |

컴파일 옵션
-std=c11
-Wall
-Wextra
-Werror
설정 목적

- 잠재적 오류 조기 발견
- 코드 안정성 향상
- Automotive 코드 품질 기준에 근접

---

# 4. Project Directory Structure

```
project-root
│
├── src
│ ├── controller
│ ├── entity
│ └── boundary
│
├── include
│
├── test
│
├── docs
│
├── uml
│
└── Makefile
```

설계 계층

| Layer    | 설명                            |
| -------- | ------------------------------- |
| boundary | ECU 인터페이스 (CAN, Sensor 등) |
| control  | 기능 로직                       |
| entity   | 데이터 모델                     |

이는 UML **Boundary / Control / Entity 패턴**을 기반으로 한다.

---

# 5. UML Based Development

본 프로젝트는 UML 기반 설계를 수행한 후 코드를 구현한다.

설계 프로세스

```
Use Case 정의
↓
Sequence Diagram 작성
↓
Controller 설계
↓
C 코드 구현
↓
Unit Test 수행
```

예시 기능

UC-002 : Crash Emergency Unlock  
UC-003 : Rear Occupant Alert

각 기능은 UML Sequence Diagram을 기준으로 구현된다.

---

# 6. Coding Rules

코딩 규칙은 **MISRA C Guidelines 일부를 참고**한다.

주요 규칙

- 전역 변수 최소화
- 명확한 타입 사용
- 포인터 사용 제한
- Magic Number 금지

예시
#define MAX_OCCUPANT 4

---

# 7. Testing Strategy

본 프로젝트에서는 다음 테스트 방식을 적용한다.

| Test Type       | Description             |
| --------------- | ----------------------- |
| Unit Test       | 각 Controller 함수 검증 |
| Simulation Test | 이벤트 기반 기능 검증   |

테스트 방식
센서 이벤트 입력

Controller 동작 수행

상태 변화 확인

로그 출력 검증

예시 테스트

Crash Event 발생
input:
airbagDeployed = true

expected:
Door Unlock Command

---

# 8. Traceability

ISO 26262 프로세스를 간단히 모사하기 위해  
다음 Traceability를 유지한다.
Use Case
↓
Sequence Diagram
↓
Source Code
↓
Test Case

이를 통해 설계와 구현 간의 연계를 유지한다.

---

# 9. Future Improvements

향후 다음 항목을 확장할 수 있다.

- Static Analysis Tool 적용
- MISRA C Rule Check
- Continuous Integration
- Code Coverage 측정

---

# 10. Implementation Rule

모든 구현은 다음 원칙을 따른다.

1. 코드가 반드시 컴파일 가능해야 한다.
2. 상세 설계(UML)에 기반하여 구현한다.
3. 설계에 없는 내용은 임의 구현 가능하나 주석으로 명시한다.

---

# 11. Assumption Rule

UML 상세 설계에 명시되지 않은 사항은
개발자가 합리적인 가정을 기반으로 구현할 수 있다.

단, 다음을 주석으로 명시해야 한다.

- Assumption
- Reason
