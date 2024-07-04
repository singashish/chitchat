# Check for Windows platform
if(WIN32)
    # On Windows, link against the Winsock library
    set(WINSOCK_LIBRARY ws2_32)
endif()

# Add the executable for the UI test
add_executable( ui 
    ui.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp
)

# Link the executable with the necessary libraries
target_link_libraries( ui PRIVATE 
    glfw 
    ${OPENGL_LIBRARIES} 
    ${WINSOCK_LIBRARY}
)