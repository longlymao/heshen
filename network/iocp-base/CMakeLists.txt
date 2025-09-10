project(iocpclient VERSION 1.0)

include_directories(src)

add_library(iocpclient STATIC
    worker/iocpworker.h worker/iocpworker.cpp
)

target_include_directories(iocpclient PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(iocpclient PUBLIC WIN32_LEAN_AND_MEAN)