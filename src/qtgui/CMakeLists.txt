add_library(QtGUI STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/qtmainwin.cpp)

target_link_libraries(QtGUI
    ${QT_LIBRARIES}
    ${OPENGL_LIBRARIES}
    Framebuffer
    )
