#include "../include/lua_udp_logger.h"

#ifdef _WIN32
    #include <WinSock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

#include <mutex>
#include <string>
#include <cstring>

// 전역 변수
static SOCKET g_udpSocket = INVALID_SOCKET;
static std::mutex g_socketMutex;
static bool g_initialized = false;
static std::string g_lastError;

// 내부 함수: 에러 메시지 설정
static void SetLastErrorMessage(const char* message) {
    g_lastError = message;
}

// UDP 초기화
LUA_UDP_LOGGER_API int InitializeUDP() {
    std::lock_guard<std::mutex> lock(g_socketMutex);
    
    if (g_initialized) {
        SetLastErrorMessage("UDP already initialized");
        return LUL_ERROR_ALREADY_INITIALIZED;
    }

#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        SetLastErrorMessage("WSAStartup failed");
        return LUL_ERROR_NETWORK;
    }
#endif

    g_udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (g_udpSocket == INVALID_SOCKET) {
        SetLastErrorMessage("Failed to create UDP socket");
#ifdef _WIN32
        WSACleanup();
#endif
        return LUL_ERROR_NETWORK;
    }

    g_initialized = true;
    SetLastErrorMessage("Success");
    return LUL_SUCCESS;
}

// UDP 패킷 전송
LUA_UDP_LOGGER_API int SendUDP(const char* ip, int port, const char* data, int length) {
    std::lock_guard<std::mutex> lock(g_socketMutex);
    
    // 파라미터 검증
    if (!ip || !data || length <= 0) {
        SetLastErrorMessage("Invalid parameters");
        return LUL_ERROR_INVALID_PARAMS;
    }
    
    if (!g_initialized || g_udpSocket == INVALID_SOCKET) {
        SetLastErrorMessage("UDP not initialized");
        return LUL_ERROR_NOT_INITIALIZED;
    }

    // 주소 구조체 설정
    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(static_cast<unsigned short>(port));
    
#ifdef _WIN32
    inet_pton(AF_INET, ip, &destAddr.sin_addr);
#else
    inet_pton(AF_INET, ip, &destAddr.sin_addr);
#endif

    // 데이터 전송
    int bytesSent = sendto(g_udpSocket, data, length, 0, 
                           reinterpret_cast<struct sockaddr*>(&destAddr), 
                           sizeof(destAddr));
    
    if (bytesSent == SOCKET_ERROR) {
        SetLastErrorMessage("Failed to send UDP packet");
        return LUL_ERROR_NETWORK;
    }

    SetLastErrorMessage("Success");
    return LUL_SUCCESS;
}

// UDP 정리
LUA_UDP_LOGGER_API void CleanupUDP() {
    std::lock_guard<std::mutex> lock(g_socketMutex);
    
    if (g_udpSocket != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(g_udpSocket);
        WSACleanup();
#else
        close(g_udpSocket);
#endif
        g_udpSocket = INVALID_SOCKET;
    }
    
    g_initialized = false;
    SetLastErrorMessage("Cleaned up");
}

// 마지막 에러 메시지 반환
LUA_UDP_LOGGER_API const char* GetLastError() {
    return g_lastError.c_str();
}

