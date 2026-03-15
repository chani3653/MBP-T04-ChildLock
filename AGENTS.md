# AI Development Instruction

본 프로젝트에서 AI는 다음 규칙을 반드시 준수한다.

## 1. Development Environment

AI는 DEVELOPMENT.md에 정의된 개발 환경을 따른다.

## 2. GitHub Workflow

AI는 Github.md에 정의된 GitHub 전략을 따른다.

## 3. Implementation Rule

모든 코드는 다음 조건을 만족해야 한다.

- 컴파일 가능
- UML 설계 기반 구현
- Boundary / Controller / Entity 구조 유지

## 4. Coding Style

모든 함수는 Doxygen 스타일 주석을 포함해야 한다.

예시

/\*\*

- @file crash_controller.c
- @brief Crash detection logic
  \*/

## 5. Annotation

각 구현 코드에는 다음 Annotation을 포함한다.

- Implemented Date
- Model Version

## 6. 참고 폴더

Usecase는 다음 폴더를 참조한다

- /doc/uml_diagram/AddFailureState -> 기초설계 버전에 failure state가 추가 된 버전
- /doc/uml_diagram/defaults -> 기초설계 버전
- /doc/uml_diagram/FlowChart -> 각 Usecase별 플로우 차트

모든 것을 종합하여 프로그램을 만든다

## 7. 코드의 저장

코드의 저장은 각 Usecase별 기능에 맞는 폴더 이름으로 생성 후 내부에 기능 이름의 C언어 파일로 저장한다.
