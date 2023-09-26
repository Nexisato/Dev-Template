# This file is used to generate the build system for the Pool-Threads project.

cmake_minimum_required(VERSION 3.10)
project(Pool-Threads)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} 
    threadpool.h    
    test_threadpool.cpp
)

