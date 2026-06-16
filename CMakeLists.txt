cmake_minimum_required(VERSION 3.20)
project(backtester)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(backtester
        main.cpp
        src/data_handler.cpp
        src/strategy.h
        src/ma_crossover.cpp
        src/ma_crossover.h
        src/portfolio.cpp
        src/portfolio.h
        src/execution_handler.cpp
        src/execution_handler.h
)