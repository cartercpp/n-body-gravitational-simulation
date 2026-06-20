cmake_minimum_required(VERSION 4.2)
project(NBodyGravitationalSimulation)

set(CMAKE_CXX_STANDARD 23)

add_executable(NBodyGravitationalSimulation main.cpp
        vector.h)
