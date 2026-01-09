find_package(magic_enum 0.9.7 QUIET CONFIG)

if(NOT magic_enum_FOUND)
    message(STATUS "magic_enum not found, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG v0.9.7
        GIT_SHALLOW TRUE
    )
    set(magic_enum_INSTALL ON)
    FetchContent_MakeAvailable(magic_enum)
endif()