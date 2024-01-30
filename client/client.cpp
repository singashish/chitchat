#include <iostream>
#include <thread>
#include <string>
#include <socket_utils.h>
#include <atomic>

std::atomic<bool> exit_flag(false);

void send_message_to_server(int socket) {
    std::string uname;
    std::cout << "Enter your name: ";
    std::getline(std::cin, uname);

    std::cout << "(type 'exit' to close.)\n";
    std::string msg;
    while (true) {
        std::getline(std::cin, msg);

        std::string full_msg = uname + ": " + msg;

        if (msg == "exit") {
            break;
        }

        // Send data to the server
        ssize_t msg_sent_size = send(socket, full_msg.c_str(), full_msg.size(), 0);

        if (msg_sent_size < 0) {
            std::cerr << "Error sending data: " << strerror(errno) << "\n";
            break;
        }
    }

    // Set the exit flag
    exit_flag = true;
}

void print_messages(int socket) {
    char buffer[1024];

    while (!exit_flag) {
        ssize_t received_msg_size = recv(socket, buffer, 1024, 0);

        if (received_msg_size > 0) {
            buffer[received_msg_size] = 0;
            std::cout << buffer << "\n";
        }

        if (received_msg_size == 0) {
            break;
        }
    }

    // Close the socket
    close_socket(socket);
}

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

        // Close the socket and deallocate memory
        close_socket(client_socket);
        delete client_address;

#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Create a thread for printing messages
    std::thread print_thread(print_messages, client_socket);

    send_message_to_server(client_socket);

    // Close the socket and deallocate memory
    close_socket(client_socket);
    delete client_address;

    // Wait for the print thread to finish
    print_thread.join();

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}