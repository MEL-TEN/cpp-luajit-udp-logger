--[[
    Simple UDP Receiver
    
    UDP 패킷을 수신하여 콘솔에 출력하는 간단한 수신기입니다.
    test_example.lua와 함께 사용하여 UDP 전송을 확인할 수 있습니다.
]]

local socket = require("socket")

local host = "127.0.0.1"
local port = 12345

print("=== UDP Receiver ===")
print("Listening on " .. host .. ":" .. port)
print("Press Ctrl+C to stop\n")

-- UDP 소켓 생성
local udp = socket.udp()
udp:setsockname(host, port)
udp:settimeout(1)  -- 1초 타임아웃

local running = true
local packet_count = 0

while running do
    local data, ip, port_or_err = udp:receivefrom()
    if data then
        packet_count = packet_count + 1
        print(string.format("[%d] Received from %s:%d", packet_count, ip, port_or_err))
        print("Data: " .. data)
        print("")
    elseif ip ~= "timeout" then
        print("Error: " .. tostring(ip))
        running = false
    end
end

udp:close()
print("\nReceiver stopped.")

