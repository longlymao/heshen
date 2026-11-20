add_library(rolling-math INTERFACE)

target_include_directories(rolling-math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

option(MATH_BUILD_TESTS "Build math library unit tests" ON)
if (TRUE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()