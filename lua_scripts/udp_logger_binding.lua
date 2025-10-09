--[[
    LuaJIT FFI Binding for UDP Logger
    
    이 모듈은 C++ DLL의 UDP 전송 및 로깅 기능을 Lua에서 사용할 수 있도록 합니다.
]]

local ffi = require("ffi")

-- C 함수 선언
ffi.cdef[[
    // 에러 코드
    static const int LUL_SUCCESS = 0;
    static const int LUL_ERROR_NETWORK = -1;
    static const int LUL_ERROR_INVALID_PARAMS = -2;
    static const int LUL_ERROR_NOT_INITIALIZED = -3;
    static const int LUL_ERROR_ALREADY_INITIALIZED = -4;
    
    // 로그 레벨
    static const int LUL_LOG_INFO = 0;
    static const int LUL_LOG_WARNING = 1;
    static const int LUL_LOG_ERROR = 2;
    
    // UDP 함수
    int InitializeUDP();
    int SendUDP(const char* ip, int port, const char* data, int length);
    void CleanupUDP();
    
    // 로그 함수
    void LogInfo(const char* message);
    void LogWarning(const char* message);
    void LogError(const char* message);
    void LogMessage(int level, const char* message);
    void SetLogTimestamp(int enable);
    
    // 에러 처리
    const char* GetLastError();
]]

-- DLL 로드 (경로는 사용자 환경에 맞게 수정 필요)
local lib_path = "lua_udp_logger.dll"  -- Windows
-- local lib_path = "./liblua_udp_logger.so"  -- Linux

local lib = ffi.load(lib_path)

-- Lua 래퍼 모듈
local UdpLogger = {}

-- 에러 코드
UdpLogger.SUCCESS = 0
UdpLogger.ERROR_NETWORK = -1
UdpLogger.ERROR_INVALID_PARAMS = -2
UdpLogger.ERROR_NOT_INITIALIZED = -3
UdpLogger.ERROR_ALREADY_INITIALIZED = -4

-- 로그 레벨
UdpLogger.LOG_INFO = 0
UdpLogger.LOG_WARNING = 1
UdpLogger.LOG_ERROR = 2

-- UDP 초기화
function UdpLogger.initUDP()
    local result = lib.InitializeUDP()
    if result ~= 0 then
        local err = ffi.string(lib.GetLastError())
        return false, err
    end
    return true
end

-- UDP 패킷 전송
function UdpLogger.sendUDP(ip, port, data)
    if type(ip) ~= "string" or type(port) ~= "number" or type(data) ~= "string" then
        return false, "Invalid parameters"
    end
    
    local result = lib.SendUDP(ip, port, data, #data)
    if result ~= 0 then
        local err = ffi.string(lib.GetLastError())
        return false, err
    end
    return true
end

-- UDP 정리
function UdpLogger.cleanupUDP()
    lib.CleanupUDP()
end

-- INFO 로그
function UdpLogger.logInfo(message)
    if type(message) ~= "string" then
        message = tostring(message)
    end
    lib.LogInfo(message)
end

-- WARNING 로그
function UdpLogger.logWarning(message)
    if type(message) ~= "string" then
        message = tostring(message)
    end
    lib.LogWarning(message)
end

-- ERROR 로그
function UdpLogger.logError(message)
    if type(message) ~= "string" then
        message = tostring(message)
    end
    lib.LogError(message)
end

-- 지정된 레벨의 로그
function UdpLogger.logMessage(level, message)
    if type(message) ~= "string" then
        message = tostring(message)
    end
    lib.LogMessage(level, message)
end

-- 타임스탬프 설정
function UdpLogger.setLogTimestamp(enable)
    lib.SetLogTimestamp(enable and 1 or 0)
end

-- 마지막 에러 메시지
function UdpLogger.getLastError()
    return ffi.string(lib.GetLastError())
end

return UdpLogger

