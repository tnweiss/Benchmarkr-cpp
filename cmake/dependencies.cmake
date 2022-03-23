include(FetchContent)

####### Set default versions ##########
message(----------------- Benchmarkr Dependencies -----------------)

if(NOT DEFINED SPDLOG_VERSION)
    set(SPDLOG_VERSION 1.9.2)
endif()
message("-- Using SPDLOG Version:                       ${SPDLOG_VERSION}")

if(NOT DEFINED ELK_CLIENT_VERSION)
    set(ELK_CLIENT_VERSION 0.0.9)
endif()
message("-- Using ELK_CLIENT_VERSION Version:           ${ELK_CLIENT_VERSION}")

if(NOT DEFINED NLOHMANN_JSON_VERSION)
    set(NLOHMANN_JSON_VERSION 3.10.5)
endif()
message("-- Using Nlohmann Json Version:                ${NLOHMANN_JSON_VERSION}")

if(NOT DEFINED CATCH_VERSION)
    set(CATCH_VERSION 2.13.8)
endif()
message("-- Using Catch Version:                        ${CATCH_VERSION}")

if(NOT DEFINED CPP_TERMINAL_VERSION)
    set(CPP_TERMINAL_VERSION 0.1.0)
endif()
message("-- Using CPP Terminal Version:                 ${CPP_TERMINAL_VERSION}")

message(-----------------------------------------------------------)

#
# 3 link libs variables
# 1 - benchmarkr-common BM_COM_LL
# 2 - benchmarkr-dev    BM_DEV_LL
# 3 - benchmarkr        BM_LL

if (UNIX)
    set(BM_COM_LL "PRIVATE;uuid;${BM_COM_LL}")
endif ()

####### Logging ##########
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog GIT_TAG v${SPDLOG_VERSION})
FetchContent_MakeAvailable(spdlog)
set(BM_COM_LL "PUBLIC;spdlog::spdlog;${BM_COM_LL}")
set(BM_DEV_LL "PUBLIC;spdlog::spdlog;${BM_DEV_LL}")
set(BM_LL "PUBLIC;spdlog::spdlog;${BM_LL}")

####### JSON ##########
if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/../dependencies/include/nlohmann/json.hpp)
    file(DOWNLOAD
            https://raw.githubusercontent.com/nlohmann/json/v${NLOHMANN_JSON_VERSION}/single_include/nlohmann/json.hpp
            ${CMAKE_CURRENT_LIST_DIR}/../dependencies/include/nlohmann/json.hpp)
endif()

####### Unit Testing ##########.
if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/../dependencies/include/catch.hpp)
    file(DOWNLOAD
            https://raw.githubusercontent.com/catchorg/Catch2/v${CATCH_VERSION}/single_include/catch2/catch.hpp
            ${CMAKE_CURRENT_LIST_DIR}/../dependencies/include/catch.hpp)
endif()

####### ELK Client ##########
#set(ELK_CLIENT_SKIP_TESTS true)
#FetchContent_Declare(elkClient GIT_REPOSITORY https://github.com/tnweiss/elk-cpp.git GIT_TAG v${ELK_CLIENT_VERSION} EXCLUDE_FROM_ALL)
#FetchContent_MakeAvailable(elkClient)
#set(BM_LL "PRIVATE;elkClient::elkClient;${BM_LL}")
find_package(elkClient 0.0.10 REQUIRED)

####### Terminal Color ##########
if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/../dependencies/include/cpp-terminal/terminal.h)
    file(DOWNLOAD
            https://raw.githubusercontent.com/jupyter-xeus/cpp-terminal/v${CPP_TERMINAL_VERSION}/cpp-terminal/terminal.h
            ${CMAKE_CURRENT_LIST_DIR}/../dependencies/include/cpp-terminal/terminal.h)
    file(DOWNLOAD
            https://raw.githubusercontent.com/jupyter-xeus/cpp-terminal/v${CPP_TERMINAL_VERSION}/cpp-terminal/terminal_base.h
            ${CMAKE_CURRENT_LIST_DIR}/../dependencies/include/cpp-terminal/terminal_base.h)
endif()
