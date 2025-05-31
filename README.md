# 🎓 SMMarble - 숙명여대 캠퍼스 졸업 보드게임 (C 버전)

**SMMarble**은 숙명여자대학교 캠퍼스를 배경으로 한 C 언어 기반 콘솔 보드게임입니다.  
플레이어는 주사위를 굴려 강의를 듣고, 축제나 식사를 즐기며 에너지를 관리하고, 졸업 요건을 만족시키는 것을 목표로 합니다.

---

## 🧩 게임 개요

- 💡 목표: **총 이수 학점 충족 + 홈 도착** → 졸업
- 🎲 방식: 주사위를 굴려 이동하며, 강의/식사/축제/실험실 등 다양한 노드를 만남
- 🧪 실험 성공을 통해 졸업 조건에 가까워짐
- 📋 플레이어는 각자 이름, 에너지, 위치, 이수 학점, 졸업 상태 등을 관리함

---

## ⚙️ 구현 기술

- **언어**: C
- **사용 헤더/모듈**:
  - `smm_object.h`: 노드 객체 정의
  - `smm_database.h`: 리스트/데이터베이스 처리
  - `smm_common.h`: 공통 매크로 및 상수
- **파일 입력 기반** 구성:
  - `marbleBoardConfig.txt`: 말판 노드 정보
  - `marbleFoodConfig.txt`: 음식 카드 정보
  - `marbleFestivalConfig.txt`: 축제 카드 정보

---

## 📁 주요 파일

```
SMMarble/
├── main.c                  # 게임 전체 로직 메인
├── smm_object.h/.c         # 객체 생성 및 속성 처리
├── smm_database.h/.c       # 데이터베이스 리스트 관리
├── smm_common.h            # 공통 정의
├── marbleBoardConfig.txt   # 보드 노드 구성
├── marbleFoodConfig.txt    # 음식 카드 구성
├── marbleFestivalConfig.txt# 축제 카드 구성
```

---

## ▶️ 실행 방법

1. GCC 컴파일러 설치
2. 모든 `.c`, `.h` 파일과 `.txt` 설정 파일이 같은 폴더에 있는지 확인
3. 터미널에서 다음 명령어 실행:

```bash
gcc -o SMMarble main.c smm_object.c smm_database.c
./SMMarble
```

(Windows 환경일 경우 `SMMarble.exe`가 생성됨)

---

## 🎮 게임 흐름 요약

1. 말판/카드 구성 파일을 읽어 노드 초기화
2. 플레이어 수 입력 및 생성 (`generatePlayers`)
3. 각 턴마다:
   - 주사위 굴리기 (`rolldie`)
   - 이동 및 액션 수행 (`goForward`, `actionNode`)
   - 졸업 여부 체크 (`isGraduated`)
4. 졸업 시 승리자 학점 정보 출력 (`printPlayerInfo`)

---

## 🧠 주요 함수 예시

```c
void actionNode(int player);
void generatePlayers(int n, int initEnergy);
void goForward(int player, int step);
int isGraduated(void);
```

---

## 🏁 졸업 조건

- 누적 이수 학점이 기준 이상 (예: 20학점 이상)
- 게임판을 한 바퀴 돌아 **HOME 노드**에 도착할 것
- 실험 성공 시 실험실에서 추가 이벤트 발생

---

## 🙋‍♀️ 개발자

- 김성현 (SungHyun Kim)  
- 숙명여자대학교 AI융합전공  
- C 기반 게임 개발 프로젝트 (2023.11)

---

## 📄 라이선스

이 프로젝트는 비상업적 학습 목적의 오픈소스입니다.  
자유롭게 참고/수정 가능하며 출처를 남겨주세요.
