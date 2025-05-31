# 🎲 SMMarble - 숙명여대 캠퍼스 보드게임

**SMMarble**은 숙명여자대학교 캠퍼스를 배경으로 한 보드게임입니다.  
부루마블(Blue Marble) 형식으로, 플레이어는 주사위를 굴려 교내 주요 건물을 돌아다니며 건물을 구매하고, 이벤트를 경험하며 승리를 노리는 전략형 게임입니다.

---

## 📌 주요 기능

- 🎲 주사위 굴리기를 통한 말 이동
- 🏢 캠퍼스 내 주요 건물 구매 및 임대 시스템
- 💸 장학금/벌금 등 랜덤 이벤트
- 👩‍🎓 플레이어 턴 전환 및 자산 관리
- 🏁 특정 조건 만족 시 게임 종료 및 승리 판정

---

## 🛠 기술 스택

- **언어**: Python
- **라이브러리**: `pygame`
- **그래픽**: 2D 이미지 및 배경 음악, 효과음 포함 (assets 폴더)

---

## 📁 폴더 구성

SMMarble/
├── main.py               # 게임 실행 메인 파일
├── board.py              # 보드 구성 및 게임판 관련 클래스
├── player.py             # 플레이어 상태 및 행동 정의
├── event.py              # 랜덤 이벤트 처리 로직
├── assets/               # 이미지, 아이콘, 배경음 등 리소스
└── README.md             # 프로젝트 소개 파일


---

## ▶️ 실행 방법

1. Python 3.x 설치  
2. 필요한 라이브러리 설치:

   ```bash
   pip install pygame
git clone https://github.com/wis-hyun/SMMarble.git
cd SMMarble
python main.py
