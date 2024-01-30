if(WIN32)
    # On Windows, link against the Winsock library
    set(WINSOCK_LIBRARY ws2_32)
endif()

add_executable(server server.cpp)
target_link_libraries(server PRIVATE socket_utils)
target_link_libraries(server PRIVATE ${WINSOCK_LIBRARY})