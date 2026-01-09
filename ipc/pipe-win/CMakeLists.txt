add_library(pipe-win STATIC
    "src/name_pipe_base.h" "src/name_pipe_base.cpp"
    "src/name_pipe_client.h" "src/name_pipe_client.cpp"
    "src/name_pipe_server.h" "src/name_pipe_server.cpp"
)

target_compile_definitions(pipe-win PUBLIC WIN32_LEAN_AND_MEAN)
target_include_directories(pipe-win PUBLIC "src")

target_link_libraries(pipe-win PUBLIC
    fmt::fmt
    spdlog::spdlog
    magic_enum::magic_enum
)

target_link_libraries(pipe-win PUBLIC ws2_32)

add_executable(pipe-win-test-server "tests/pipe_win_test_server.cpp")
target_link_libraries(pipe-win-test-server PRIVATE pipe-win)

add_executable(pipe-win-test-client "tests/pipe_win_test_client.cpp")
target_link_libraries(pipe-win-test-client PRIVATE pipe-win)