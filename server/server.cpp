#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <socket_utils.h>
#include <mutex>

std::vector<int> accepted_sockets;
std::mutex accepted_sockets_mutex;

void broadcast_messages_to_clients(char *buffer, int sender_socket) {
    std::lock_guard<std::mutex> lock(accepted_sockets_mutex);

    for (int client_socket : accepted_sockets) {
        if (client_socket != sender_socket) {
            send(client_socket, buffer, strlen(buffer), 0);
        }
    }
}

void receive_and_print_incoming_data(int client_socket) {
    char buffer[1024];
    while (true) {
        ssize_t amount_received = recv(client_socket, buffer, 1024, 0);

        if (amount_received > 0) {
            buffer[amount_received] = 0;
            std::cout << buffer << "\n";

            broadcast_messages_to_clients(buffer, client_socket);
        }

        if (amount_received == 0) {
            break;
        }
    }

    // Close the socket
    close_socket(client_socket);

    // Remove the socket from the list
    {
        std::lock_guard<std::mutex> lock(accepted_sockets_mutex);
        auto it = std::find(accepted_sockets.begin(), accepted_sockets.end(), client_socket);
        if (it != accepted_sockets.end()) {
            accepted_sockets.erase(it);
        }
    }
}

void incoming_connection(int server_socket) {
    while (true) {
        sockaddr_in client_address;
        int client_address_size = sizeof(sockaddr_in);
        int client_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_address), &client_address_size);

        if (client_socket > 0) {
            {
                std::lock_guard<std::mutex> lock(accepted_sockets_mutex);
                accepted_sockets.push_back(client_socket);
            }

            // Create a thread for each incoming connection
            std::thread(receive_and_print_incoming_data, client_socket).detach();
        }
    }
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
    int server_socket = create_TCPIPV4_socket();
    if (server_socket == -1) {
        std::cerr << "Error creating server socket\n";
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Set up the server address structure
    sockaddr_in* server_address = create_IPV4_address("", 2000);

    int result = bind(server_socket, reinterpret_cast<sockaddr*>(server_address), sizeof(*server_address));

    if (result == 0) {
        std::cout << "Socket was bound successfully!\n";
    } else {
        std::cerr << "Error connecting to the server: " << strerror(errno) << "\n";
    }

    int listen_result = listen(server_socket, 10);

    // Create a vector to store threads
    std::vector<std::thread> threads;

    // Create a thread for accepting connections
    threads.emplace_back(incoming_connection, server_socket);

    // Wait for user input to exit
    std::cout << "Press Enter to exit...\n";
    std::cin.ignore();

    // Signal threads to finish and wait for them to complete
    {
        std::lock_guard<std::mutex> lock(accepted_sockets_mutex);
        for (int client_socket : accepted_sockets) {
#ifdef _WIN32
            shutdown(client_socket, SD_BOTH);
#else
            shutdown(client_socket, SHUT_RDWR);
#endif
            close_socket(client_socket);
        }
        accepted_sockets.clear();
    }

    // Shutdown and close the server socket
#ifdef _WIN32
    shutdown(server_socket, SD_BOTH);
    WSACleanup();
#else
    shutdown(server_socket, SHUT_RDWR);
#endif

    // Join the accept thread before exiting
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}