add_library(Framebuffer STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/renderthread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtoglframebuffer.cpp)

target_link_libraries(Framebuffer
    ${QT_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    Camera
    )
