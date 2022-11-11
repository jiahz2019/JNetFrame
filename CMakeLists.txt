cmake_minimum_required(VERSION 3.0)

project(JNetFrame)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include (cmake/utils.cmake)
include_directories(.)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)

set(LIB_SRC
    src/log.cc
    src/util.cc
    src/mutex.cc
    src/env.cc
    src/config.cc
    src/timer.cc
    src/fiber.cc
    src/scheduler.cc
    src/thread.cc
    src/iomanager.cc
    src/hook.cc
    src/fd_manager.cc
    src/address.cc
    src/socket.cc
    src/bytearray.cc
    src/stream.cc
    src/tcp_server.cc
    src/http/http-parser/http_parser.cc
    src/http/http.cc
    src/http/http_parser.cc 
    src/stream.cc 
    src/streams/socket_stream.cc
    src/http/http_session.cc 
    src/http/servlet.cc
    src/http/http_server.cc 
    )


add_library(jhz SHARED ${LIB_SRC})


set(LIBS
    jhz
    pthread
    yaml-cpp
    ${CONAN_LIBS}
)

sylar_add_executable(test_log "tests/test_log.cc" jhz "${LIBS}")
sylar_add_executable(test_config "tests/test_config.cc" jhz "${LIBS}")
sylar_add_executable(test_thread "tests/test_thread.cc" jhz "${LIBS}")
sylar_add_executable(test_fiber "tests/test_fiber.cc" jhz "${LIBS}")
sylar_add_executable(test_fiber2 "tests/test_fiber2.cc" jhz "${LIBS}")
sylar_add_executable(test_scheduler "tests/test_scheduler.cc" jhz "${LIBS}")
sylar_add_executable(test_iomanager "tests/test_iomanager.cc" jhz "${LIBS}")
sylar_add_executable(test_hook "tests/test_hook.cc" jhz "${LIBS}")
sylar_add_executable(test_address "tests/test_address.cc" jhz "${LIBS}")
sylar_add_executable(test_bytearray "tests/test_bytearray.cc" jhz "${LIBS}")
sylar_add_executable(test_tcp_server "tests/test_tcp_server.cc" jhz "${LIBS}")
sylar_add_executable(test_http_parser "tests/test_http_parser.cc" jhz "${LIBS}")
sylar_add_executable(test_http_server "tests/test_http_server.cc" jhz "${LIBS}")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)