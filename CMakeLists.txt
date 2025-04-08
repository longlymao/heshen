cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options("/utf-8")
endif()

project(heshen)

add_subdirectory(rolling-math)
add_subdirectory(rolling-utils)
add_subdirectory(rolling-audio)

add_subdirectory(rolling-render)