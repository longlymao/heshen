find_package(fmt 10.2.1 QUIET CONFIG)

if(NOT fmt_FOUND)
    message(STATUS "fmt not found, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.2.1
        GIT_SHALLOW TRUE
    )
    set(FMT_INSTALL ON)
    FetchContent_MakeAvailable(fmt)
endif()