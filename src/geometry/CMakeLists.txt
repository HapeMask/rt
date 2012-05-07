add_library(Geometry STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/aabb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/meshtriangle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shape.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sphere.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/triangle.cpp)

target_link_libraries(Geometry
    ${QT_LIBRARIES}
    ${OPENGL_LIBRARIES}
    Mathlib)
