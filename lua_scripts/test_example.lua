--[[
    UDP Logger 테스트 예제
    
    이 스크립트는 UDP Logger 라이브러리의 기능을 테스트합니다.
]]

-- 바인딩 모듈 로드
local UdpLogger = require("udp_logger_binding")

print("=== UDP Logger Test ===\n")

-- 1. 로그 기능 테스트
print("1. Testing Console Logger...")
UdpLogger.logInfo("This is an INFO message")
UdpLogger.logWarning("This is a WARNING message")
UdpLogger.logError("This is an ERROR message")

print("\n2. Testing with timestamp disabled...")
UdpLogger.setLogTimestamp(false)
UdpLogger.logInfo("Info without timestamp")
UdpLogger.logWarning("Warning without timestamp")

print("\n3. Re-enabling timestamp...")
UdpLogger.setLogTimestamp(true)
UdpLogger.logInfo("Info with timestamp restored")

-- 2. UDP 기능 테스트
print("\n4. Testing UDP functionality...")

-- UDP 초기화
local success, err = UdpLogger.initUDP()
if success then
    UdpLogger.logInfo("UDP initialized successfully")
else
    UdpLogger.logError("Failed to initialize UDP: " .. err)
    os.exit(1)
end

-- UDP 패킷 전송 테스트
local test_ip = "127.0.0.1"  -- 로컬호스트
local test_port = 12345
local test_message = "Hello from Lua via UDP!"

UdpLogger.logInfo("Sending UDP packet to " .. test_ip .. ":" .. test_port)
success, err = UdpLogger.sendUDP(test_ip, test_port, test_message)

if success then
    UdpLogger.logInfo("UDP packet sent successfully")
else
    UdpLogger.logError("Failed to send UDP packet: " .. err)
end

-- 여러 패킷 전송 테스트
print("\n5. Sending multiple UDP packets...")
for i = 1, 5 do
    local message = string.format("Test packet #%d", i)
    success, err = UdpLogger.sendUDP(test_ip, test_port, message)
    if success then
        UdpLogger.logInfo("Sent: " .. message)
    else
        UdpLogger.logError("Failed to send packet #" .. i .. ": " .. err)
    end
    -- 약간의 지연
    os.execute("timeout /t 1 >nul 2>&1")  -- Windows
    -- os.execute("sleep 1")  -- Linux/Mac
end

-- JSON 형식 데이터 전송 예제
print("\n6. Sending JSON data...")
local json_data = '{"type":"log","level":"info","message":"Test from Lua","timestamp":' .. os.time() .. '}'
success, err = UdpLogger.sendUDP(test_ip, test_port, json_data)
if success then
    UdpLogger.logInfo("JSON data sent successfully")
else
    UdpLogger.logError("Failed to send JSON data: " .. err)
end

-- UDP 정리
print("\n7. Cleaning up...")
UdpLogger.cleanupUDP()
UdpLogger.logInfo("UDP cleaned up")

print("\n=== Test Complete ===")

