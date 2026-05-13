cmake_minimum_required(VERSION 3.20)

project(pc_monitor_backend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pc-monitor-backend
    main.cpp
)

target_compile_options(pc-monitor-backend PRIVATE -Wall -Wextra -Wpedantic)