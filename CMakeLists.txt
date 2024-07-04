cmake_minimum_required( VERSION 3.10 )
project( ChitChat )

# Set C++ standard
set( CMAKE_CXX_STANDARD 11 )

find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

# GLFW
set( GLFW_DIR ${CMAKE_CURRENT_LIST_DIR}/external/glfw )
option( GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF )
option( GLFW_BUILD_TESTS "Build the GLFW test programs" OFF )
option( GLFW_BUILD_DOCS "Build the GLFW documentation" OFF )
option( GLFW_INSTALL "Generate installation target" OFF )
option( GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF )
add_subdirectory( ${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL )
include_directories( ${GLFW_DIR}/include )
include_directories( ${GLFW_DIR}/deps )

# Dear ImGui
set( IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/external/imgui )
include_directories( ${IMGUI_DIR} ${IMGUI_DIR}/backends .. )

# Add subdirectories
add_subdirectory( client )
# add_subdirectory( server )
# add_subdirectory( utils )