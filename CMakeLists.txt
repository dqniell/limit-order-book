cmake_minimum_required(VERSION 3.15)
project(LimitOrderBook)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(lob
    src/main.cpp
    src/OrderBook.cpp
    src/Parser.cpp
)