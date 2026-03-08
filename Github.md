# GitHub Usage Strategy

## 1. Overview

본 프로젝트는 **ISO 26262 기반 SW 개발 시뮬레이션 프로젝트**로  
GitHub를 이용하여 다음을 관리한다.

- Source Code
- UML 설계
- 프로젝트 문서
- 개발 이력 관리

---

# 2. Branch Strategy

Git Flow 기반으로 다음 Branch 구조를 사용한다.
main
develop
feature/*

Branch 설명

| Branch | Description |
|------|-------------|
| main | 안정 버전 |
| develop | 통합 개발 브랜치 |
| feature | 기능 개발 |

예시

feature/crash-unlock

feature/roa-detection

---

# 3. Commit Convention

Commit 메시지는 다음 형식을 사용한다.
type: message
예시
feat: add crash emergency unlock logic
fix: correct occupant detection condition
docs: update UML diagram

Type 종류

| Type | 설명 |
|-----|-----|
| feat | 기능 추가 |
| fix | 버그 수정 |
| docs | 문서 수정 |
| refactor | 코드 개선 |

---

# 4. Issue Management

GitHub Issue를 이용하여 다음을 관리한다.

- 기능 개발
- 버그
- 설계 작업

Issue 예시
#12 Implement Crash Detection Logic

Issue 구성
Description
Related UML
Expected Result

---

# 5. Pull Request Process

기능 개발은 Pull Request를 통해 병합한다.

Process
```
feature branch 생성
↓
기능 개발
↓
Pull Request 생성
↓
Code Review
↓
develop branch merge
```
---

# 6. Code Review

Pull Request에서는 다음을 검토한다.

- Coding Rule 준수 여부
- 설계(UML)와 코드 일치 여부
- 기능 동작 검증

---

# 7. Documentation Management

프로젝트 문서는 다음 위치에 저장한다.
docs/

문서 종류

| Document | Description |
|---------|-------------|
| DevelopmentEnvironment.md | 개발 환경 정의 |
| Github.md | GitHub 운영 전략 |
| Architecture.md | 시스템 구조 |
| TestPlan.md | 테스트 계획 |

---

# 8. UML Management

UML 다이어그램은 다음 경로에 저장한다.
uml/

사용 도구
draw.io

관리 대상

- Use Case Diagram
- Sequence Diagram
- Architecture Diagram

---

# 9. Wiki Usage

GitHub Wiki를 활용하여 다음을 정리한다.

- 기능 설명
- 설계 배경
- UML 해설

---

# 10. Continuous Integration (Optional)

향후 GitHub Actions를 이용하여

- Build 자동화
- Unit Test 실행

을 수행할 수 있다.
