#include "../include/lua_udp_logger.h"

#include <iostream>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
    #include <windows.h>
#endif

// 전역 변수
static std::mutex g_logMutex;
static bool g_enableTimestamp = true;

// 내부 함수: 현재 시간 문자열 반환
static std::string GetCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::tm tm_now;
#ifdef _WIN32
    localtime_s(&tm_now, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_now);
#endif
    
    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return oss.str();
}

// 내부 함수: 로그 레벨 문자열 반환
static const char* GetLogLevelString(int level) {
    switch (level) {
        case LUL_LOG_INFO:    return "INFO";
        case LUL_LOG_WARNING: return "WARNING";
        case LUL_LOG_ERROR:   return "ERROR";
        default:              return "UNKNOWN";
    }
}

// 내부 함수: 콘솔 색상 설정 (Windows)
#ifdef _WIN32
static void SetConsoleColor(int level) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (level) {
        case LUL_LOG_INFO:
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        case LUL_LOG_WARNING:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        case LUL_LOG_ERROR:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            break;
        default:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
    }
}

static void ResetConsoleColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
#else
static void SetConsoleColor(int level) {
    switch (level) {
        case LUL_LOG_INFO:
            std::cout << "\033[32m"; // Green
            break;
        case LUL_LOG_WARNING:
            std::cout << "\033[33m"; // Yellow
            break;
        case LUL_LOG_ERROR:
            std::cout << "\033[31m"; // Red
            break;
    }
}

static void ResetConsoleColor() {
    std::cout << "\033[0m"; // Reset
}
#endif

// 로그 메시지 출력 (내부 구현)
static void LogMessageInternal(int level, const char* message) {
    if (!message) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(g_logMutex);
    
    SetConsoleColor(level);
    
    if (g_enableTimestamp) {
        std::cout << "[" << GetCurrentTimeString() << "] ";
    }
    
    std::cout << "[" << GetLogLevelString(level) << "] " << message << std::endl;
    
    ResetConsoleColor();
}

// INFO 로그
LUA_UDP_LOGGER_API void LogInfo(const char* message) {
    LogMessageInternal(LUL_LOG_INFO, message);
}

// WARNING 로그
LUA_UDP_LOGGER_API void LogWarning(const char* message) {
    LogMessageInternal(LUL_LOG_WARNING, message);
}

// ERROR 로그
LUA_UDP_LOGGER_API void LogError(const char* message) {
    LogMessageInternal(LUL_LOG_ERROR, message);
}

// 지정된 레벨의 로그
LUA_UDP_LOGGER_API void LogMessage(int level, const char* message) {
    LogMessageInternal(level, message);
}

// 타임스탬프 설정
LUA_UDP_LOGGER_API void SetLogTimestamp(int enable) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    g_enableTimestamp = (enable != 0);
}

