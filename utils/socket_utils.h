#pragma once

#ifdef _WIN32
    #include <WinSock2.h>
    #include <WS2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif

struct AcceptedSocket {
    int accepted_client_socket;
    sockaddr_in address;
    int error;
    bool accepted_successfully;
};

int create_TCPIPV4_socket();
sockaddr_in* create_IPV4_address(const char* ip, int port);
void close_socket(int sock);