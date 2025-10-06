find_package(spdlog 1.14.1 QUIET CONFIG)

if(NOT spdlog_FOUND)
    message(STATUS "spdlog not found, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.14.1
        GIT_SHALLOW TRUE
    )
    set(SPDLOG_FMT_EXTERNAL ON)
    FetchContent_MakeAvailable(spdlog)
endif()