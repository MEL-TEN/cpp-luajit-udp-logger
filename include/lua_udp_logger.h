#ifndef LUA_UDP_LOGGER_H
#define LUA_UDP_LOGGER_H

#ifdef _WIN32
    #ifdef LUA_UDP_LOGGER_EXPORTS
        #define LUA_UDP_LOGGER_API __declspec(dllexport)
    #else
        #define LUA_UDP_LOGGER_API __declspec(dllimport)
    #endif
#else
    #define LUA_UDP_LOGGER_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// 에러 코드 정의
#define LUL_SUCCESS 0
#define LUL_ERROR_NETWORK -1
#define LUL_ERROR_INVALID_PARAMS -2
#define LUL_ERROR_NOT_INITIALIZED -3
#define LUL_ERROR_ALREADY_INITIALIZED -4

// 로그 레벨 정의
#define LUL_LOG_INFO 0
#define LUL_LOG_WARNING 1
#define LUL_LOG_ERROR 2

// UDP 관련 함수
/**
 * UDP 소켓을 초기화합니다.
 * @return 성공 시 LUL_SUCCESS, 실패 시 에러 코드
 */
LUA_UDP_LOGGER_API int InitializeUDP();

/**
 * UDP 패킷을 전송합니다.
 * @param ip 대상 IP 주소 (문자열)
 * @param port 대상 포트 번호
 * @param data 전송할 데이터
 * @param length 데이터 길이
 * @return 성공 시 LUL_SUCCESS, 실패 시 에러 코드
 */
LUA_UDP_LOGGER_API int SendUDP(const char* ip, int port, const char* data, int length);

/**
 * UDP 소켓을 정리합니다.
 */
LUA_UDP_LOGGER_API void CleanupUDP();

// 로그 관련 함수
/**
 * INFO 레벨 로그를 출력합니다.
 * @param message 로그 메시지
 */
LUA_UDP_LOGGER_API void LogInfo(const char* message);

/**
 * WARNING 레벨 로그를 출력합니다.
 * @param message 로그 메시지
 */
LUA_UDP_LOGGER_API void LogWarning(const char* message);

/**
 * ERROR 레벨 로그를 출력합니다.
 * @param message 로그 메시지
 */
LUA_UDP_LOGGER_API void LogError(const char* message);

/**
 * 지정된 레벨의 로그를 출력합니다.
 * @param level 로그 레벨 (LUL_LOG_INFO, LUL_LOG_WARNING, LUL_LOG_ERROR)
 * @param message 로그 메시지
 */
LUA_UDP_LOGGER_API void LogMessage(int level, const char* message);

/**
 * 로그에 타임스탬프 표시 여부를 설정합니다.
 * @param enable 1이면 활성화, 0이면 비활성화
 */
LUA_UDP_LOGGER_API void SetLogTimestamp(int enable);

/**
 * 마지막 에러 메시지를 반환합니다.
 * @return 에러 메시지 문자열 (내부 버퍼, 복사 불필요)
 */
LUA_UDP_LOGGER_API const char* GetLastError();

#ifdef __cplusplus
}
#endif

#endif // LUA_UDP_LOGGER_H

