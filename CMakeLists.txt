cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include directories
include_directories(utils)

# Add subdirectories
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(utils)