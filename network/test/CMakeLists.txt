add_executable(test_server
    server/main.cpp
)

target_link_libraries(test_server PRIVATE
    iocpbase
)

add_executable(test_client
    client/main.cpp
)

target_link_libraries(test_client PRIVATE
    iocpbase
)