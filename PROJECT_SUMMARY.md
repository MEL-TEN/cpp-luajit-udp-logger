# 프로젝트 요약: C++ LuaJIT UDP Logger

## 프로젝트 개요

**C++ LuaJIT UDP Logger**는 Visual Studio 환경에서 C++과 LuaJIT을 연동하여 UDP 전송 및 콘솔 로그 기능을 Lua 스크립트에서 사용할 수 있도록 구현한 라이브러리입니다. 이 프로젝트는 OpenRouter API를 통해 Claude 3.5 Sonnet과 GPT-4 Turbo의 설계 의견을 수집하여 개발되었습니다.

## 구현된 주요 기능

### 1. UDP 전송 기능 (`udp_sender.cpp`)

**Windows Sockets API**를 사용하여 UDP 패킷을 전송하는 기능을 구현했습니다. 주요 특징은 다음과 같습니다.

- **초기화 및 정리**: `InitializeUDP()`와 `CleanupUDP()` 함수를 통해 소켓 리소스를 관리합니다.
- **패킷 전송**: `SendUDP()` 함수로 지정된 IP 주소와 포트로 데이터를 전송합니다.
- **스레드 안전성**: `std::mutex`를 사용하여 멀티스레드 환경에서 안전하게 동작합니다.
- **에러 처리**: 명확한 에러 코드를 반환하고, `GetLastError()` 함수로 상세한 에러 메시지를 제공합니다.

### 2. 콘솔 로거 기능 (`console_logger.cpp`)

다양한 로그 레벨을 지원하는 콘솔 출력 기능을 구현했습니다.

- **로그 레벨**: INFO, WARNING, ERROR 세 가지 레벨을 지원합니다.
- **타임스탬프**: 밀리초 단위까지 표시되는 타임스탬프를 포함할 수 있습니다.
- **색상 코딩**: Windows 콘솔에서 로그 레벨에 따라 다른 색상으로 표시됩니다.
  - INFO: 녹색
  - WARNING: 노란색
  - ERROR: 빨간색
- **스레드 안전성**: 로그 출력 시 뮤텍스를 사용하여 동시성 문제를 방지합니다.

### 3. LuaJIT FFI 바인딩 (`udp_logger_binding.lua`)

LuaJIT의 FFI(Foreign Function Interface)를 사용하여 C++ 함수를 Lua에서 직접 호출할 수 있도록 바인딩했습니다.

- **C 함수 선언**: `ffi.cdef`를 사용하여 C++ API를 선언합니다.
- **Lua 래퍼**: 사용자 친화적인 Lua 함수로 래핑하여 편리하게 사용할 수 있습니다.
- **에러 처리**: Lua 스타일의 에러 처리(success, error 반환)를 제공합니다.

## AI 컨설팅 결과

OpenRouter API를 통해 두 개의 주요 AI 모델에게 설계 조언을 받았습니다.

### Claude 3.5 Sonnet의 주요 조언

1. **함수 시그니처 설계**: `extern "C"`와 `__declspec(dllexport)`를 사용한 명확한 API 정의
2. **UDP 구현**: WSAStartup 호출, 소켓 에러 체크, 주소 구조체 설정
3. **FFI 바인딩**: 타입 매핑과 함수 포인터 처리
4. **에러 처리**: 예외 대신 에러 코드 사용, 로그 기능 활용
5. **스레드 안전성**: 뮤텍스를 사용한 공유 리소스 보호

### GPT-4 Turbo의 주요 조언

1. **메모리 관리**: 언어 경계를 넘지 않는 메모리 할당/해제
2. **블로킹/논블로킹 모드**: UDP의 특성을 고려한 모드 선택
3. **타입 매핑**: LuaJIT FFI에서 정확한 타입 지정
4. **예외 및 반환 코드**: 외부 API 경계에서는 반환 코드 사용
5. **스레드 로컬 저장소**: 전역 변수 대신 스레드 로컬 저장소 고려

## 프로젝트 구조

```
cpp-luajit-udp-logger/
├── src/                      # C++ 소스 파일
│   ├── udp_sender.cpp        # UDP 전송 구현
│   └── console_logger.cpp    # 콘솔 로거 구현
├── include/                  # C++ 헤더 파일
│   └── lua_udp_logger.h      # API 정의
├── lua_scripts/              # Lua 스크립트
│   ├── udp_logger_binding.lua  # FFI 바인딩
│   ├── test_example.lua        # 테스트 예제
│   └── udp_receiver.lua        # UDP 수신 테스트
├── docs/                     # 문서
│   ├── build_guide_vs.md     # Visual Studio 빌드 가이드
│   ├── api_reference.md      # API 레퍼런스
│   └── ai_consultation_summary.md  # AI 컨설팅 결과
├── CMakeLists.txt            # CMake 빌드 파일
├── LICENSE                   # MIT 라이선스
└── README.md                 # 프로젝트 소개
```

## 기술적 특징

### 스레드 안전성

모든 공유 리소스(UDP 소켓, 콘솔 출력)에 대한 접근은 `std::mutex`를 사용하여 보호됩니다. 이를 통해 멀티스레드 환경에서도 안전하게 사용할 수 있습니다.

### 크로스 플랫폼 지원

Windows를 주 타겟으로 하지만, 조건부 컴파일을 통해 Linux/Mac에서도 빌드할 수 있도록 설계되었습니다.

```cpp
#ifdef _WIN32
    // Windows 특정 코드
#else
    // POSIX 코드
#endif
```

### 명확한 에러 처리

모든 함수는 명확한 에러 코드를 반환하며, `GetLastError()` 함수를 통해 상세한 에러 메시지를 확인할 수 있습니다.

| 에러 코드 | 의미 |
|----------|------|
| 0 | 성공 |
| -1 | 네트워크 에러 |
| -2 | 잘못된 파라미터 |
| -3 | 초기화되지 않음 |
| -4 | 이미 초기화됨 |

## 사용 예제

### Lua에서 UDP 전송

```lua
local UdpLogger = require("udp_logger_binding")

-- UDP 초기화
local success, err = UdpLogger.initUDP()
if not success then
    print("Failed to initialize UDP: " .. err)
    return
end

-- 패킷 전송
success, err = UdpLogger.sendUDP("192.168.1.100", 12345, "Hello, World!")
if success then
    UdpLogger.logInfo("Packet sent successfully")
else
    UdpLogger.logError("Failed to send packet: " .. err)
end

-- 정리
UdpLogger.cleanupUDP()
```

### Lua에서 로그 출력

```lua
local UdpLogger = require("udp_logger_binding")

-- 타임스탬프 활성화
UdpLogger.setLogTimestamp(true)

-- 다양한 레벨의 로그 출력
UdpLogger.logInfo("Application started")
UdpLogger.logWarning("Configuration file not found, using defaults")
UdpLogger.logError("Failed to connect to database")
```

## 빌드 방법

### Visual Studio

1. 새 DLL 프로젝트 생성
2. 소스 파일 추가
3. 프로젝트 속성 설정 (포함 디렉터리, 전처리기 정의, 링커 종속성)
4. 빌드

상세한 가이드: [docs/build_guide_vs.md](./docs/build_guide_vs.md)

### CMake

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## 문서

- **[README.md](./README.md)**: 프로젝트 소개 및 빠른 시작 가이드
- **[docs/build_guide_vs.md](./docs/build_guide_vs.md)**: Visual Studio 빌드 가이드
- **[docs/api_reference.md](./docs/api_reference.md)**: 상세한 API 레퍼런스
- **[docs/ai_consultation_summary.md](./docs/ai_consultation_summary.md)**: AI 컨설팅 결과

## GitHub 리포지토리

**URL**: https://github.com/MEL-TEN/cpp-luajit-udp-logger

## 라이선스

MIT License - 자유롭게 사용, 수정, 배포할 수 있습니다.

## 결론

이 프로젝트는 C++과 LuaJIT을 효과적으로 연동하여 UDP 전송 및 로깅 기능을 제공하는 실용적인 라이브러리입니다. OpenRouter를 통한 AI 컨설팅을 활용하여 설계 단계부터 모범 사례를 적용했으며, 스레드 안전성과 에러 처리를 고려한 견고한 구현을 제공합니다. Visual Studio 환경에서 쉽게 빌드하고 사용할 수 있으며, 상세한 문서와 예제를 통해 빠르게 시작할 수 있습니다.

