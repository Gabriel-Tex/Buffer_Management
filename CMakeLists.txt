cmake_minimum_required(VERSION 3.28)
project(BufferManagement)

add_executable(buffer src/main.cpp src/BufferManagement.cpp)
target_include_directories(buffer PRIVATE include)