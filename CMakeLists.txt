cmake_minimum_required(VERSION 3.21)
project(Zephyr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Zephyr)

add_subdirectory(Zephyr)

option(ZEPHYR_BUILD_TESTS "Build unit tests" OFF)
if(ZEPHYR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
