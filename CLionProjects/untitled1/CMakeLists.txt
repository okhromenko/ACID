cmake_minimum_required(VERSION 3.19)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)


add_executable(untitled1 Google_tests/Source.cpp Consist_list.h)
add_subdirectory(Google_tests)