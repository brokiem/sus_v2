cmake_minimum_required(VERSION 3.24)
project(sus)

set(CMAKE_CXX_STANDARD 20)

add_executable(sus main.cpp)

target_compile_options(sus PRIVATE -mwindows -O3)

target_link_libraries(sus gdiplus)
