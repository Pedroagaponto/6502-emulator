cmake_minimum_required(VERSION 3.10)

# set the project name
project(nes)
include_directories(include)

# add the executable
add_executable(nes src/main.cpp src/cpu6502.cpp)