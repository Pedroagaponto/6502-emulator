cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# set the project name
project(nes)
include_directories(include)

file (GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(nes ${SRC_FILES})
