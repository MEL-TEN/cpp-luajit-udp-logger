# API 레퍼런스

이 문서는 `lua_udp_logger` 라이브러리의 C++ API와 Lua API에 대한 상세한 레퍼런스를 제공합니다.

## C++ API

### UDP 함수

#### `InitializeUDP()`

UDP 소켓을 초기화합니다. 이 함수는 UDP 패킷을 전송하기 전에 반드시 호출해야 합니다.

**시그니처:**
```cpp
int InitializeUDP();
```

**반환값:**
- `LUL_SUCCESS` (0): 성공
- `LUL_ERROR_ALREADY_INITIALIZED` (-4): 이미 초기화됨
- `LUL_ERROR_NETWORK` (-1): 네트워크 초기화 실패

**예제:**
```cpp
int result = InitializeUDP();
if (result != LUL_SUCCESS) {
    const char* error = GetLastError();
    printf("Failed to initialize UDP: %s\n", error);
}
```

---

#### `SendUDP()`

UDP 패킷을 지정된 IP 주소와 포트로 전송합니다.

**시그니처:**
```cpp
int SendUDP(const char* ip, int port, const char* data, int length);
```

**파라미터:**
- `ip`: 대상 IP 주소 (예: "192.168.1.100")
- `port`: 대상 포트 번호 (1-65535)
- `data`: 전송할 데이터 버퍼
- `length`: 데이터 길이 (바이트)

**반환값:**
- `LUL_SUCCESS` (0): 성공
- `LUL_ERROR_INVALID_PARAMS` (-2): 잘못된 파라미터
- `LUL_ERROR_NOT_INITIALIZED` (-3): UDP가 초기화되지 않음
- `LUL_ERROR_NETWORK` (-1): 전송 실패

**예제:**
```cpp
const char* message = "Hello, UDP!";
int result = SendUDP("127.0.0.1", 12345, message, strlen(message));
if (result != LUL_SUCCESS) {
    printf("Failed to send UDP packet\n");
}
```

---

#### `CleanupUDP()`

UDP 소켓을 정리하고 리소스를 해제합니다.

**시그니처:**
```cpp
void CleanupUDP();
```

**예제:**
```cpp
CleanupUDP();
```

---

### 로그 함수

#### `LogInfo()`

INFO 레벨의 로그 메시지를 콘솔에 출력합니다.

**시그니처:**
```cpp
void LogInfo(const char* message);
```

**파라미터:**
- `message`: 출력할 로그 메시지

**예제:**
```cpp
LogInfo("Application started successfully");
```

---

#### `LogWarning()`

WARNING 레벨의 로그 메시지를 콘솔에 출력합니다.

**시그니처:**
```cpp
void LogWarning(const char* message);
```

**파라미터:**
- `message`: 출력할 로그 메시지

**예제:**
```cpp
LogWarning("Configuration file not found, using defaults");
```

---

#### `LogError()`

ERROR 레벨의 로그 메시지를 콘솔에 출력합니다.

**시그니처:**
```cpp
void LogError(const char* message);
```

**파라미터:**
- `message`: 출력할 로그 메시지

**예제:**
```cpp
LogError("Failed to connect to database");
```

---

#### `LogMessage()`

지정된 레벨의 로그 메시지를 콘솔에 출력합니다.

**시그니처:**
```cpp
void LogMessage(int level, const char* message);
```

**파라미터:**
- `level`: 로그 레벨 (`LUL_LOG_INFO`, `LUL_LOG_WARNING`, `LUL_LOG_ERROR`)
- `message`: 출력할 로그 메시지

**예제:**
```cpp
LogMessage(LUL_LOG_INFO, "Custom log message");
```

---

#### `SetLogTimestamp()`

로그 메시지에 타임스탬프를 포함할지 여부를 설정합니다.

**시그니처:**
```cpp
void SetLogTimestamp(int enable);
```

**파라미터:**
- `enable`: 1이면 타임스탬프 활성화, 0이면 비활성화

**예제:**
```cpp
SetLogTimestamp(0);  // 타임스탬프 비활성화
LogInfo("This log has no timestamp");
SetLogTimestamp(1);  // 타임스탬프 활성화
```

---

#### `GetLastError()`

마지막으로 발생한 에러의 메시지를 반환합니다.

**시그니처:**
```cpp
const char* GetLastError();
```

**반환값:**
- 에러 메시지 문자열 (내부 버퍼, 복사 불필요)

**예제:**
```cpp
int result = InitializeUDP();
if (result != LUL_SUCCESS) {
    const char* error = GetLastError();
    printf("Error: %s\n", error);
}
```

---

## Lua API

Lua에서는 `udp_logger_binding.lua` 모듈을 통해 C++ 함수를 호출할 수 있습니다.

### 모듈 로드

```lua
local UdpLogger = require("udp_logger_binding")
```

### UDP 함수

#### `UdpLogger.initUDP()`

UDP 소켓을 초기화합니다.

**반환값:**
- `success` (boolean): 성공 여부
- `error` (string): 실패 시 에러 메시지

**예제:**
```lua
local success, err = UdpLogger.initUDP()
if not success then
    print("Failed to initialize UDP: " .. err)
end
```

---

#### `UdpLogger.sendUDP(ip, port, data)`

UDP 패킷을 전송합니다.

**파라미터:**
- `ip` (string): 대상 IP 주소
- `port` (number): 대상 포트 번호
- `data` (string): 전송할 데이터

**반환값:**
- `success` (boolean): 성공 여부
- `error` (string): 실패 시 에러 메시지

**예제:**
```lua
local success, err = UdpLogger.sendUDP("127.0.0.1", 12345, "Hello from Lua!")
if not success then
    print("Failed to send UDP: " .. err)
end
```

---

#### `UdpLogger.cleanupUDP()`

UDP 소켓을 정리합니다.

**예제:**
```lua
UdpLogger.cleanupUDP()
```

---

### 로그 함수

#### `UdpLogger.logInfo(message)`

INFO 레벨 로그를 출력합니다.

**파라미터:**
- `message` (string): 로그 메시지

**예제:**
```lua
UdpLogger.logInfo("Application started")
```

---

#### `UdpLogger.logWarning(message)`

WARNING 레벨 로그를 출력합니다.

**파라미터:**
- `message` (string): 로그 메시지

**예제:**
```lua
UdpLogger.logWarning("Low memory warning")
```

---

#### `UdpLogger.logError(message)`

ERROR 레벨 로그를 출력합니다.

**파라미터:**
- `message` (string): 로그 메시지

**예제:**
```lua
UdpLogger.logError("Connection failed")
```

---

#### `UdpLogger.logMessage(level, message)`

지정된 레벨의 로그를 출력합니다.

**파라미터:**
- `level` (number): 로그 레벨 (`UdpLogger.LOG_INFO`, `UdpLogger.LOG_WARNING`, `UdpLogger.LOG_ERROR`)
- `message` (string): 로그 메시지

**예제:**
```lua
UdpLogger.logMessage(UdpLogger.LOG_INFO, "Custom log")
```

---

#### `UdpLogger.setLogTimestamp(enable)`

타임스탬프 표시 여부를 설정합니다.

**파라미터:**
- `enable` (boolean): true면 활성화, false면 비활성화

**예제:**
```lua
UdpLogger.setLogTimestamp(false)  -- 타임스탬프 비활성화
```

---

#### `UdpLogger.getLastError()`

마지막 에러 메시지를 반환합니다.

**반환값:**
- `error` (string): 에러 메시지

**예제:**
```lua
local error = UdpLogger.getLastError()
print("Last error: " .. error)
```

---

## 에러 코드

| 코드 | 상수 | 설명 |
|------|------|------|
| 0 | `LUL_SUCCESS` | 성공 |
| -1 | `LUL_ERROR_NETWORK` | 네트워크 에러 |
| -2 | `LUL_ERROR_INVALID_PARAMS` | 잘못된 파라미터 |
| -3 | `LUL_ERROR_NOT_INITIALIZED` | 초기화되지 않음 |
| -4 | `LUL_ERROR_ALREADY_INITIALIZED` | 이미 초기화됨 |

## 로그 레벨

| 레벨 | 상수 | 설명 |
|------|------|------|
| 0 | `LUL_LOG_INFO` | 정보 메시지 |
| 1 | `LUL_LOG_WARNING` | 경고 메시지 |
| 2 | `LUL_LOG_ERROR` | 에러 메시지 |

