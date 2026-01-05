add_library(pipe-win STATIC
    "src/name_pipe_host.h" "src/name_pipe_host.cpp"
)

target_compile_definitions(pipe-win PUBLIC WIN32_LEAN_AND_MEAN)
target_include_directories(pipe-win PUBLIC "src")

target_link_libraries(pipe-win PRIVATE
    fmt::fmt
    spdlog::spdlog
)

add_executable(pipe-win-test-host "tests/pipe_win_test_host.cpp")
target_link_libraries(pipe-win-test-host PRIVATE pipe-win)

add_executable(pipe-win-test-client "tests/pipe_win_test_client.cpp")
target_link_libraries(pipe-win-test-client PRIVATE pipe-win)