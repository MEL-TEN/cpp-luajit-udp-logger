cmake_minimum_required(VERSION 3.10)
project(lua_udp_logger VERSION 1.0.0 LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 소스 파일
set(SOURCES
    src/udp_sender.cpp
    src/console_logger.cpp
)

# 헤더 파일
set(HEADERS
    include/lua_udp_logger.h
)

# 공유 라이브러리 생성
add_library(lua_udp_logger SHARED ${SOURCES} ${HEADERS})

# 포함 디렉터리
target_include_directories(lua_udp_logger PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 전처리기 정의
target_compile_definitions(lua_udp_logger PRIVATE
    LUA_UDP_LOGGER_EXPORTS
)

# Windows 특정 설정
if(WIN32)
    target_compile_definitions(lua_udp_logger PRIVATE
        _WINSOCK_DEPRECATED_NO_WARNINGS
        _CRT_SECURE_NO_WARNINGS
    )
    target_link_libraries(lua_udp_logger PRIVATE ws2_32)
endif()

# 설치 설정
install(TARGETS lua_udp_logger
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS} DESTINATION include)

