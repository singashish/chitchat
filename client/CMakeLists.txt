if(WIN32)
    # On Windows, link against the Winsock library
    set(WINSOCK_LIBRARY ws2_32)
endif()

add_executable(client client.cpp)
target_link_libraries(client PRIVATE socket_utils)
target_link_libraries(client PRIVATE ${WINSOCK_LIBRARY})