cmake_minimum_required(VERSION 3.19)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled1 Consist_List.h Google_tests/Source.cpp)
add_subdirectory(Google_tests)