#include <iostream>
#include <string>
#include <socket_utils.h>

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock\n";
        return 1;
    }
#endif

    // Create a socket
    int client_socket = create_TCPIPV4_socket();
    if (client_socket == -1) {
        std::cerr << "Error creating client socket\n";

#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Server information
    std::string server_ip = "127.0.0.1";
    int server_port = 2000;

    // Set up the client address structure
    sockaddr_in *client_address = create_IPV4_address(server_ip.c_str(), server_port);

    // Connect to the server
    int result = connect(client_socket, reinterpret_cast<sockaddr*>(client_address), sizeof(*client_address));

    if (result == 0) {
        std::cout << "Connected to the server!\n";
    } else {
        std::cerr << "Error connecting to the server: " << strerror(errno) << "\n";

        // Close the socket
        close_socket(client_socket);

#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    std::cout << "(type 'exit' to close.)\n";

    std::string line;
    while (true) {
        std::getline(std::cin, line);

        if (line == "exit") {
            break;
        }

        // Send data to the server
        ssize_t amount_sent = send(client_socket, line.c_str(), line.size(), 0);

        if (amount_sent < 0) {
            std::cerr << "Error sending data: " << strerror(errno) << "\n";
            break;
        }
    }

    // Close the socket
    close_socket(client_socket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}