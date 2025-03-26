# 🎮 RoRaNumberBaseball

멀티플레이어 숫자 야구 게임 (Unreal Engine 5 / Listen Server 기반)

---

## 📌 개요

`RoRaNumberBaseball`는 UE5 기반 리슨 서버 구조로 제작된 멀티플레이어 숫자 야구 게임입니다.  
채팅 기반으로 숫자를 입력하고, 상대와 턴을 주고받으며 **3자리 숫자**를 맞추는 게임입니다.

---

## ✅ 현재 구현된 기능

- **채팅 기반 숫자 입력 시스템**
  - "/123" 형식으로 숫자 입력
  - 채팅 메시지와 명령 구분 처리
- **Strike / Ball / Out 판정 로직**
  - 정답 비교 및 결과 계산
  - Out은 무효 입력이나 중복 숫자에도 적용됨
- **턴 제어 시스템**
  - 플레이어 별 차례 진행 및 전환
  - 턴 종료 조건(정답/기회 소진) 처리
- **참여 인원 선택 UI**
  - 게임 시작 전, 원하는 인원 수 선택 가능
  - 확인 버튼 클릭 시 서버로 정보 전달
- **클라이언트 ↔ 서버 구조 통신**
  - GameMode / PlayerController 기반 명령 처리
  - Listen Server 환경에서 테스트 가능
- **Function Library 도입**
  - 숫자 파싱, 유효성 검사 등 공통 기능 분리 및 재사용

---

## ❌ 아직 구현되지 않은 기능

- [ ] **타이머 기능**
  - 각 턴당 제한 시간
  - 시간 초과 시 자동 OUT 처리 예정
- [ ] **게임 진행 상황 UI**
  - 자신 및 상대의 시도, 결과 리스트 표시
  - HUD 형태로 실시간 업데이트
- [ ] **결과 화면 / 리매치 기능**
  - HUD 형태로 승패 결과 출력(채팅으로 출력중) 및 리셋 기능

---

## 🧱 기술 스택

- Unreal Engine 5 (C++ / UMG)
- Listen Server 기반 멀티플레이 구조
- GameMode / GameState / PlayerController 분리 설계

---

## 🧩 Function Library

- **`UJudgeLibrary`**
  - 프로젝트 전반에서 사용되는 숫자 야구 관련 유틸리티 함수 모음
  - ex. 입력값 유효성 검사 
  - 모든 함수는 `static`이며, Blueprint에서도 호출 가능

- **`URandomLibrary`**
  - 프로젝트 전반에서 사용되는 랜덤 숫자자 관련 유틸리티 함수 모음
  - ex. 랜덤 숫자 생성성
  - 모든 함수는 `static`이며, Blueprint에서도 호출 가능
---

## 🛠️ 향후 계획

- 게임 HUD 구성 개선
- 게임 재시작 자동화
- 타이머와 턴제 플레이 추가

---

## 📁 프로젝트 구조

```
Source/
├── GameMode/
├── PlayerController/
├── GameState/
├── UI/
│   ├── ChatWidget
│   └── PlayerCountSelectWidget
├── FunctionLibrary/
│   └── JudgeLibrary.h/.cpp
└── └── RandomLibrary.h/.cpp


---

## 📷 스크린샷 (추가 예정)

> *게임 화면 및 UI는 추후 이미지와 함께 업데이트됩니다.*
