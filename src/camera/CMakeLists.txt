add_library(Camera STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp)

target_link_libraries(Camera
    Mathlib)
