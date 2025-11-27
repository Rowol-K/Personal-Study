# ⚾ Multiplayer Numeric Baseball Server (C++ / Winsock2)

> **C언어 레거시 코드의 기술적 부채를 청산하고, 고성능 멀티스레드 소켓 서버로 확장한 프로젝트입니다.**

![C++](https://img.shields.io/badge/Language-C++17-blue) ![Winsock2](https://img.shields.io/badge/Network-Winsock2-red) ![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

## 📌 1. 프로젝트 개요 (Overview)
이 프로젝트는 단순한 콘솔 기반의 싱글 플레이 게임을 **네트워크 멀티플레이어 게임**으로 고도화하는 과정을 담고 있습니다.
초기 **절차지향적 C언어 코드(Legacy)의** 한계를 극복하기 위해 **객체지향(OOP) 아키텍처**로 리팩토링을 수행했으며, 이후 **TCP/IP 소켓 통신**을 도입하여 실시간 대전이 가능한 서버-클라이언트 구조를 구축했습니다.

### 🎯 핵심 목표
* **Technical Debt Cleanup:** 유지보수가 어려운 스파게티 코드를 모듈화된 클래스 구조로 재설계.
* **Network Implementation:** Winsock2 API를 활용한 **Raw Socket** 프로그래밍 및 멀티스레드 서버 구축.
* **Troubleshooting:** 네트워크 통신 간 발생하는 실무적인 이슈(Packet Coalescing, Race Condition) 해결.

---

## 🛠️ 2. 아키텍처 및 기술 스택 (Architecture)

### 🏗️ 시스템 구조
본 프로젝트는 **Server**(중계 및 판정)와 **Client**(입출력)가 명확히 분리된 구조를 가집니다.

* **Server:** `NetworkManager` (통신) + `GameEngine` (로직/판정)
* **Client:** `InputValidator` (검증) + `UI` (입출력)

### 🧩 기술적 특징
| 카테고리 | 적용 기술 및 내용 |
| :--- | :--- |
| **Language** | **Modern C++** (STL: vector, string, thread, mutex) |
| **Network** | **Winsock2 (TCP/IP)**, Blocking Socket |
| **Concurrency** | **std::thread** (1:N Client Handling), **std::mutex** (Critical Section 보호) |
| **Protocol** | **Struct Packing** (`#pragma pack`)을 통한 직렬화(Serialization) |

---

## 🚀 3. 주요 구현 내용 (Key Features)

### A. 객체지향 리팩토링 (OOP Refactoring)
* **책임의 분리:** 게임의 로직을 담당하는 `GameEngine`과 통신을 담당하는 `NetworkManager`로 클래스를 분리하여 결합도를 낮췄습니다.
* **유연한 확장성:** 난이도(자릿수)를 상수로 관리하여, 코드 수정 없이 3자리/4자리/9자리 야구 게임으로 확장이 가능합니다.

### B. 멀티스레드 소켓 서버 (Multi-threaded Server)
* **1 Client : 1 Thread 모델:** 클라이언트 접속 시 전담 스레드를 생성하여 독립적인 통신을 보장합니다.
* **Thread-Safe Broadcasting:** 정답자가 발생했을 때 `std::mutex`로 보호된 접속자 리스트를 순회하며 모든 클라이언트에게 결과를 브로드캐스팅합니다.

### C. 클라이언트 비동기 처리
* **UI Blocking 해결:** 입력 대기(`cin`) 중에도 서버의 메시지를 수신할 수 있도록 **수신 전용 스레드**(RecvThread)를 분리하여 구현했습니다.

---

## 🔥 4. 트러블 슈팅 (Troubleshooting)

개발 과정에서 발생한 주요 기술적 문제와 해결 과정입니다.

### 💥 Issue 1: TCP 패킷 뭉침 현상 (Sticky Packet)
* **현상:** 클라이언트가 '로그인 패킷'과 '게임 시작 패킷'을 연속으로 보낼 때, 서버가 이를 하나의 데이터로 인식하여 뒤쪽 패킷을 처리하지 못하는 현상 발생.
* **원인:** TCP의 스트림(Stream) 특성과 Nagle 알고리즘으로 인해 데이터가 버퍼에 뭉쳐서 도착함.
* **해결:** 수신 버퍼(`recv`) 내에서 **패킷 헤더**(Size)를 먼저 읽고, 패킷 크기만큼 데이터를 잘라내며 반복 처리하는 **파싱 루프**(Parsing Loop) 로직을 구현하여 해결.

### 💥 Issue 2: 경쟁 상태 (Race Condition)
* **현상:** 여러 클라이언트가 동시에 접속하거나 연결을 끊을 때 서버가 비정상 종료됨.
* **원인:** 여러 스레드가 동시에 `std::vector<SOCKET> m_clients` 리스트에 접근하여 메모리 충돌 발생.
* **해결:** `std::mutex`와 `std::lock_guard`를 도입하여 임계 영역(Critical Section)을 보호, 스레드 안전성(Thread-Safety) 확보.

### 💥 Issue 3: 클라이언트 UI 입력 대기(Blocking) 문제
* **현상:** 멀티플레이 환경에서 다른 유저가 정답을 맞춰 게임이 종료되었음에도, 내 클라이언트 화면에는 결과 메시지가 뜨지 않고 입력 대기 상태(`cin`)에 머물러 있는 현상 발생.
* **원인:** `std::cin`이 사용자 입력을 기다리는 동안 메인 스레드가 블로킹(Blocking) 되어, 서버로부터 수신된 패킷을 처리하거나 출력할 수 없었음.
* **해결:** **수신 전용 스레드**(RecvThread)를 분리하여 서버 메시지를 비동기로 출력하도록 구조를 변경하고, 입력 직전과 직후에 게임 종료 플래그(`inGame`)를 이중 체크하여 뒷북 입력을 방지함.

### 💥 Issue 4: 변수 중복 선언으로 인한 컴파일 에러
* **현상:** `switch-case` 문 내부에서 패킷 타입별로 구조체 포인터를 선언할 때, 동일한 변수명(`pkt`)을 재사용하려다 `redeclaration` 에러 발생.
* **원인:** C++의 `switch` 문은 전체가 하나의 스코프(Scope)로 취급되기 때문에, `case` 라벨만으로는 변수의 유효 범위가 분리되지 않음.
* **해결:** 각 `case` 문을 **중괄호 `{ }`**로 감싸주어 변수의 유효 범위를 명확히 분리(Block Scope)함으로써 해결.

### 💥 Issue 5: 정적 멤버 변수 초기화 시점 문제
* **현상:** `GamePacket.h`에서 난이도 상수를 `4`로 변경했음에도, 서버 실행 시 정답이 여전히 `3`자리로 생성되는 현상.
* **원인:** `NetworkManager` 클래스 내의 `static GameEngine` 객체가 생성될 때, 생성자에 전달되는 기본값 인자가 의도한 상수(`DEFAULT_GAME_LEVEL`)가 아닌 하드코딩된 값이나 디버그 출력문의 착시로 인해 오해 발생.
* **해결:** `NetworkManager.cpp` 상단의 정적 변수 초기화 구문에서 명시적으로 상수를 주입하도록 수정하고, 디버그 출력문 또한 반복문을 사용하여 동적으로 자릿수를 표기하도록 개선.

---

## 🌿 5. Git 브랜치 전략 (Branch Strategy)

단계별 성장을 기록하기 위해 체계적인 브랜치 전략을 사용했습니다.

* **`master`**: 리팩토링 전, **C언어 레거시 원본 코드** 보존.
* **`refactor/oop`**: **C++ 객체지향 리팩토링** 작업 완료 버전.
* **`feat/socket-basic`**: **소켓 통신 및 멀티스레드** 기능 구현 버전.
* **`main`**: 모든 기능이 통합된 **최종 배포 버전**.

---

## 💻 6. 실행 방법 (How to Run)

1.  `Numeric Baseball Refactoring.sln` 솔루션 파일을 엽니다. (Visual Studio 2022 권장)
2.  **Server** 프로젝트(`Numeric Baseball Refactoring`)를 먼저 빌드하고 실행합니다.
    * *(현재 디버깅 편의를 위해 서버 콘솔창에 정답 숫자가 노출되도록 설정되어 있습니다.)*
3.  **Client** 프로젝트(`BaseballClient`)를 실행하여 서버에 접속합니다.
    * *Visual Studio에서 '새 인스턴스 시작' 기능을 통해 여러 개의 클라이언트를 동시에 실행할 수 있습니다.*
4.  **Client**의 콘솔 창 안내에 따라 숫자야구 게임의 규칙(S/B/O)에 맞춰 숫자를 입력하고 게임을 진행합니다.

---

## 🔮 7. 향후 개선 계획 (Future Roadmap)

현재의 프로토타입을 넘어, 실제 상용화 가능한 수준의 게임 서버로 발전시키기 위한 기술적 목표입니다.

### 1️⃣ 사용자 식별 시스템 고도화 (Custom User Identity)
* **현재:** 클라이언트 접속 시 하드코딩된 닉네임("Rowol")을 사용 중.
* **개선:** 로그인 패킷(`PT_LOGIN`) 전송 단계에서 유저가 직접 닉네임을 설정할 수 있도록 입력 로직을 추가하고, 서버 내 `ClientSession` 객체에서 유저별 고유 ID와 닉네임을 매핑하여 관리합니다. 이를 통해 채팅 및 결과 브로드캐스팅 시 명확한 식별이 가능하도록 개선할 예정입니다.

### 2️⃣ 세션 기반 1:1 매치메이킹 시스템 (Session-based Matchmaking)
* **현재:** 서버 내 단일 정적 엔진(`static GameEngine`)을 사용하여 모든 유저가 하나의 정답을 공유하는 구조.
* **개선:** **'로비(Lobby) - 매칭(Matching) - 게임룸(Game Room)'** 구조를 도입하여 다중 게임 세션을 지원합니다.
    * **매치메이킹:** 대기열 큐(Queue)를 생성하여 2명의 유저가 모이면 독립적인 게임 방(Room)을 생성합니다.
    * **인스턴스 분리:** 각 방마다 별도의 `GameEngine` 인스턴스를 생성하여, 유저들이 서로 간섭받지 않고 1:1 대전을 즐길 수 있도록 서버 아키텍처를 확장합니다.
