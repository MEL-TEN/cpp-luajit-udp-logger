# Visual Studio 빌드 가이드

이 문서는 Visual Studio를 사용하여 `lua_udp_logger` DLL을 빌드하는 방법을 설명합니다.

## 사전 요구사항

1.  **Visual Studio**: C++ 데스크톱 개발 워크로드가 설치된 Visual Studio 2019 또는 2022.
2.  **LuaJIT**: 시스템에 미리 빌드된 LuaJIT 라이브러리가 필요합니다. 이 가이드에서는 LuaJIT이 `C:\luajit` 경로에 설치되었다고 가정합니다. 해당 경로는 사용자 환경에 맞게 변경해야 합니다.
    -   `C:\luajit\include`: `lua.hpp`, `luajit.h` 등 헤더 파일 포함
    -   `C:\luajit\lib`: `lua51.lib` 라이브러리 파일 포함

## 1. 새 프로젝트 생성

1.  Visual Studio를 열고 **새 프로젝트 만들기**를 선택합니다.
2.  **동적 연결 라이브러리(DLL)** 템플릿을 선택하고 **다음**을 클릭합니다.
3.  프로젝트 이름으로 `lua_udp_logger`를 입력하고, 소스 코드가 있는 위치에 프로젝트를 생성합니다. **만들기**를 클릭합니다.

## 2. 소스 파일 추가

1.  **솔루션 탐색기**에서 생성된 프로젝트를 확인합니다.
2.  기존에 생성된 `framework.h`, `pch.h`, `pch.cpp`, `dllmain.cpp` 등은 제거해도 무방합니다.
3.  **헤더 파일** 필터에 `include/lua_udp_logger.h` 파일을 추가합니다.
4.  **소스 파일** 필터에 `src/udp_sender.cpp`와 `src/console_logger.cpp` 파일을 추가합니다.

## 3. 프로젝트 속성 설정

프로젝트를 마우스 오른쪽 버튼으로 클릭하고 **속성**을 선택하여 프로젝트 속성 페이지를 엽니다. **모든 구성** 및 **모든 플랫폼**에 대해 다음 설정을 적용합니다.

### 3.1. C/C++ > 일반 (General)

-   **추가 포함 디렉터리 (Additional Include Directories)**:
    -   `$(ProjectDir)include`
    -   `C:\luajit\include` (사용자의 LuaJIT 설치 경로로 변경)

### 3.2. C/C++ > 전처리기 (Preprocessor)

-   **전처리기 정의 (Preprocessor Definitions)**:
    -   `LUA_UDP_LOGGER_EXPORTS`
    -   `_WINSOCK_DEPRECATED_NO_WARNINGS`
    -   `_CRT_SECURE_NO_WARNINGS`
    -   (기존 정의에 추가)

### 3.3. 링커 > 일반 (General)

-   **추가 라이브러리 디렉터리 (Additional Library Directories)**:
    -   `C:\luajit\lib` (사용자의 LuaJIT 라이브러리 경로로 변경)

### 3.4. 링커 > 입력 (Input)

-   **추가 종속성 (Additional Dependencies)**:
    -   `lua51.lib` (LuaJIT 버전에 따라 파일명이 다를 수 있음)
    -   `ws2_32.lib`
    -   (기존 종속성에 추가)

## 4. 빌드

1.  메뉴에서 **빌드** > **솔루션 빌드**를 선택합니다.
2.  빌드가 성공하면 프로젝트 출력 디렉터리(예: `x64/Debug` 또는 `x64/Release`)에 `lua_udp_logger.dll` 파일이 생성됩니다.

## 5. 테스트

1.  생성된 `lua_udp_logger.dll` 파일을 `lua_scripts` 디렉터리로 복사합니다.
2.  `lua_scripts` 디렉터리에서 LuaJIT 실행 파일을 사용하여 테스트 스크립트를 실행합니다.

    ```bash
    # UDP 수신기 실행 (별도의 터미널에서)
    luajit udp_receiver.lua

    # 테스트 스크립트 실행
    luajit test_example.lua
    ```

3.  `test_example.lua`를 실행하면 콘솔에 로그가 출력되고, `udp_receiver.lua`를 실행한 터미널에 UDP 패킷 수신 정보가 표시됩니다.

