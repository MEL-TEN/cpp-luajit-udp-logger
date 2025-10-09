# C++ LuaJIT UDP Logger

C++과 LuaJIT을 연동하여 UDP 전송 및 콘솔 로그 기능을 Lua 스크립트에서 사용할 수 있도록 구현한 프로젝트입니다. Visual Studio 환경에서 DLL로 빌드되며, LuaJIT의 FFI(Foreign Function Interface)를 통해 Lua에서 직접 호출할 수 있습니다.

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
└── README.md
```

## 주요 기능

### UDP 전송
C++에서 구현된 UDP 소켓을 통해 데이터를 전송합니다. Windows Sockets API를 사용하며, 스레드 안전성을 보장합니다.

**주요 함수:**
- `InitializeUDP()`: UDP 소켓 초기화
- `SendUDP(ip, port, data, length)`: UDP 패킷 전송
- `CleanupUDP()`: 리소스 정리

### 콘솔 로그
다양한 로그 레벨(INFO, WARNING, ERROR)을 지원하는 콘솔 출력 기능을 제공합니다. 타임스탬프와 색상 코딩을 지원하며, 스레드 안전합니다.

**주요 함수:**
- `LogInfo(message)`: INFO 레벨 로그
- `LogWarning(message)`: WARNING 레벨 로그
- `LogError(message)`: ERROR 레벨 로그
- `SetLogTimestamp(enable)`: 타임스탬프 표시 설정

### LuaJIT FFI 바인딩
LuaJIT의 FFI를 사용하여 Lua 스크립트에서 C++ 함수를 직접 호출할 수 있습니다. 별도의 바인딩 코드 작성 없이 간단하게 연동됩니다.

## 개발 환경

- **IDE**: Visual Studio 2019/2022 (C++ 데스크톱 개발 워크로드)
- **언어**: C++11 이상
- **런타임**: LuaJIT 2.0+ (외부 설치 필요)
- **플랫폼**: Windows 10/11 (Linux/Mac도 지원 가능)

## 빌드 방법

### Visual Studio 사용

1. Visual Studio에서 새 **동적 연결 라이브러리(DLL)** 프로젝트를 생성합니다.
2. 소스 파일(`src/*.cpp`)과 헤더 파일(`include/*.h`)을 프로젝트에 추가합니다.
3. 프로젝트 속성에서 다음을 설정합니다:
   - **추가 포함 디렉터리**: `$(ProjectDir)include`, LuaJIT 헤더 경로
   - **전처리기 정의**: `LUA_UDP_LOGGER_EXPORTS`, `_WINSOCK_DEPRECATED_NO_WARNINGS`
   - **추가 종속성**: `ws2_32.lib`
4. 빌드하여 `lua_udp_logger.dll`을 생성합니다.

상세한 빌드 가이드는 [docs/build_guide_vs.md](./docs/build_guide_vs.md)를 참조하세요.

### CMake 사용

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## 사용 방법

### 1. DLL 준비
빌드된 `lua_udp_logger.dll` 파일을 Lua 스크립트와 같은 디렉터리에 복사합니다.

### 2. Lua 스크립트 작성

```lua
local UdpLogger = require("udp_logger_binding")

-- 로그 출력
UdpLogger.logInfo("Application started")
UdpLogger.logWarning("This is a warning")
UdpLogger.logError("An error occurred")

-- UDP 초기화 및 전송
local success, err = UdpLogger.initUDP()
if success then
    UdpLogger.sendUDP("127.0.0.1", 12345, "Hello, UDP!")
    UdpLogger.cleanupUDP()
end
```

### 3. 실행

```bash
luajit your_script.lua
```

더 많은 예제는 [lua_scripts/test_example.lua](./lua_scripts/test_example.lua)를 참조하세요.

## API 레퍼런스

상세한 API 문서는 [docs/api_reference.md](./docs/api_reference.md)를 참조하세요.

## AI 컨설팅

이 프로젝트는 OpenRouter API를 통해 Claude 3.5 Sonnet과 GPT-4 Turbo의 설계 의견을 수집하여 구현되었습니다. AI 컨설팅 결과는 [docs/ai_consultation_summary.md](./docs/ai_consultation_summary.md)에서 확인할 수 있습니다.

## 라이선스

MIT License

## 기여

이슈 및 풀 리퀘스트를 환영합니다!

## 작성자

**Manus AI** - OpenRouter를 통한 AI 협업으로 개발

