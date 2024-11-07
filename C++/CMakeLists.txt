cmake_minimum_required(VERSION 3.29)
project(AI_series_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(AI_series_1 main.cpp
        Agent.h
        Env.h
        Agent.cpp
        Env.cpp
)
