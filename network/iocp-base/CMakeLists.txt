add_library(iocpbase STATIC
    worker/iocpworker.h worker/iocpworker.cpp
    core/iocpcore.h core/iocpcore.cpp
    server/iocpserver.h server/iocpserver.cpp
    client/iocpclient.h client/iocpclient.cpp
)

target_include_directories(iocpbase PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(iocpbase PUBLIC WIN32_LEAN_AND_MEAN)

target_link_libraries(iocpbase PUBLIC
  ws2_32.lib
  mswsock.lib
)