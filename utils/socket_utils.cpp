#include <socket_utils.h>
#include <iostream>

int create_TCPIPV4_socket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

sockaddr_in* create_IPV4_address(const char* ip, int port) {
    sockaddr_in *address = new sockaddr_in();
    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if (strlen(ip) == 0) {
        address->sin_addr.s_addr = INADDR_ANY;
    }
    else {
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);
    }

    return address;
}

void close_socket(int sock) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}