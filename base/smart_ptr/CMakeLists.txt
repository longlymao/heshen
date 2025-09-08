project(smart_ptr LANGUAGES CXX)

add_library(smart_ptr INTERFACE)

target_include_directories(smart_ptr INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)