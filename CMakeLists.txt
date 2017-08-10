cmake_minimum_required(VERSION 3.7)
project(Stack_Based_Calculator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES calculator.cpp)
add_executable(Stack_Based_Calculator ${SOURCE_FILES})