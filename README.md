# 🚗 Automated Parking Tower Control System
> 2025 기능경기대회 전기기기 종목 출전작  
> ATmega16A 기반 자동 주차타워 입·출차 제어 시스템

---

## 📌 Project Overview
본 프로젝트는 **ATmega16A 마이크로컨트롤러를 기반으로 제작한 자동 주차타워 제어 시스템**입니다.  
기능경기대회 전기기기 종목의 산업 자동화 과제를 모티브로 설계되었으며,  
차량의 **입차 / 저장 / 정보조회 / 출차 / 요금정산 / 모터구동**까지 전 과정을 하나의 임베디드 로직으로 구현하였습니다.

단순 LED 제어 수준이 아닌 실제 산업 설비처럼  
**DC모터, 스텝모터, 엔코더, 적외선 센서, FND, Shift Register, ADC 버튼 입력**을 통합하여  
완전한 자동화 주차 프로세스를 구현한 것이 핵심입니다.

---

## 🎬 Demonstration Video
▶ [주차타워 시스템 동작 영상 보기](https://www.youtube.com/watch?v=OMSTihPh2Og)

---

## 🛠 Hardware Configuration

| Part | Description |
|------|-------------|
| MCU | ATmega16A |
| Display | 4Digit FND |
| LED Output | Shift Register(IC4015) 기반 15채널 LED |
| Input | ADC 저항분배식 7버튼 + 엔코더 스위치 |
| Motor1 | DC Motor (수평 이동) |
| Motor2 | Step Motor (수직 승강) |
| Sensor | DC Sensor / Step Position Sensor |
| Encoder | 차량 위치 선택용 Rotary Encoder |
| Lamp | 상태 표시용 램프 출력 |

---

## ⚙ Core Technologies

- AVR Embedded C Programming
- Interrupt Timer Scheduling
- Finite State Machine Logic
- ADC Multi Button Input Processing
- Shift Register LED Multiplexing
- FND Dynamic Scanning
- DC Motor PWM Direction Control
- Step Motor Sequence Control
- Sensor Feedback Based Positioning
- Parking Data Memory Management

---

## 🔥 Main Features

### 1. 차량 정보 조회 모드
- 엔코더를 이용한 차고 위치 선택
- 저장된 차량번호 / 주차위치 / 입차시간 확인
- 빈 차고 선택 시 에러 처리

---

### 2. 입차 모드 (Parking Mode)
- 차량번호 입력
- 주차시간 입력
- 차고 위치 선택
- 차량 데이터 메모리 저장
- DC모터 수평이동
- 스텝모터 수직 승강
- 센서 피드백 기반 자동 위치 정렬
- 지정 차고 차량 저장 완료

---

### 3. 출차 모드 (Leaving Mode)
- 차량번호 검색
- 저장 차량 위치 자동 탐색
- 주차시간 기반 요금 계산
- 요금 지불 확인
- 차량 자동 인출
- 데이터 메모리 초기화

---

### 4. 실시간 에러 검출
- 이미 차량이 존재하는 차고 선택 시 Error
- 존재하지 않는 차량번호 검색 시 Error
- 램프 점멸 및 FND Error 출력

---

## 🧠 System Logic Structure

본 시스템은 전체 동작을 **State Machine 기반**으로 설계하였습니다.

```c
mode = 0 : Idle
mode = 1 : 차량 조회 모드
mode = 2 : 입차 제어 모드
mode = 3 : 출차 제어 모드
