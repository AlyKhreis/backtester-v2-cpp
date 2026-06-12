cmake_minimum_required(VERSION 3.20)
project(backtester)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(backtester
        main.cpp
        src/data_handler.cpp
)