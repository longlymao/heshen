#include "iocpcore.h"
#include <iostream>

static LPFN_ACCEPTEX LoadAcceptEx() {
    GUID guid = WSAID_ACCEPTEX;
    LPFN_ACCEPTEX acceptEx = nullptr;
    DWORD bytes = 0;

    SOCKET socket = WSASocketW(
        AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (socket == INVALID_SOCKET) {
        std::cerr << "WSASocket failed with error: " << WSAGetLastError()
                  << std::endl;
        return nullptr;
    }

    int rst = WSAIoctl(socket,
                       SIO_GET_EXTENSION_FUNCTION_POINTER,
                       &guid,
                       sizeof(guid),
                       &acceptEx,
                       sizeof(acceptEx),
                       &bytes,
                       nullptr,
                       nullptr);
    if (rst == SOCKET_ERROR) {
        std::cerr << "WSAIoctl failed with error: " << WSAGetLastError()
                  << std::endl;
    }

    closesocket(socket);
    return acceptEx;
}

static LPFN_CONNECTEX LoadConnectEx() {
    GUID guid = WSAID_CONNECTEX;
    LPFN_CONNECTEX connectEx = nullptr;
    DWORD bytes = 0;

    SOCKET socket = WSASocketW(
        AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (socket == INVALID_SOCKET) {
        std::cerr << "WSASocket failed with error: " << WSAGetLastError()
                  << std::endl;
        return nullptr;
    }

    int rst = WSAIoctl(socket,
                       SIO_GET_EXTENSION_FUNCTION_POINTER,
                       &guid,
                       sizeof(guid),
                       &connectEx,
                       sizeof(connectEx),
                       &bytes,
                       nullptr,
                       nullptr);
    if (rst == SOCKET_ERROR) {
        std::cerr << "WSAIoctl failed with error: " << WSAGetLastError()
                  << std::endl;
    }

    closesocket(socket);
    return connectEx;
}

static LPFN_GETACCEPTEXSOCKADDRS LoadGetAcceptExSockaddrs() {
    GUID guid = WSAID_GETACCEPTEXSOCKADDRS;
    LPFN_GETACCEPTEXSOCKADDRS getAcceptExSockaddrs = nullptr;
    DWORD bytes = 0;

    SOCKET socket = WSASocketW(
        AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (socket == INVALID_SOCKET) {
        std::cerr << "WSASocket failed with error: " << WSAGetLastError()
                  << std::endl;
        return nullptr;
    }

    int rst = WSAIoctl(socket,
                       SIO_GET_EXTENSION_FUNCTION_POINTER,
                       &guid,
                       sizeof(guid),
                       &getAcceptExSockaddrs,
                       sizeof(getAcceptExSockaddrs),
                       &bytes,
                       nullptr,
                       nullptr);
    if (rst == SOCKET_ERROR) {
        std::cerr << "WSAIoctl failed with error: " << WSAGetLastError()
                  << std::endl;
    }

    closesocket(socket);
    return getAcceptExSockaddrs;
}

LPFN_ACCEPTEX IocpCore::GetAcceptEx() {
    static LPFN_ACCEPTEX acceptEx = LoadAcceptEx();
    return acceptEx;
}

LPFN_CONNECTEX IocpCore::GetConnectEx() {
    static LPFN_CONNECTEX connectEx = LoadConnectEx();
    return connectEx;
}

LPFN_GETACCEPTEXSOCKADDRS IocpCore::GetGetAcceptExSockaddrs() {
    static LPFN_GETACCEPTEXSOCKADDRS getAcceptExSockaddrs =
        LoadGetAcceptExSockaddrs();
    return getAcceptExSockaddrs;
}