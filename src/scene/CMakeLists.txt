set(${CMAKE_BINARY_DIR}/src/scene/sceneparser.cpp GENERATED)

ADD_CUSTOM_COMMAND(
    SOURCE ${CMAKE_SOURCE_DIR}/scene/sceneparser.l
    OUTPUT ${CMAKE_BINARY_DIR}/src/scene/sceneparser.cpp
    COMMAND ${FLEX_EXECUTABLE}
    ARGS -o ${CMAKE_BINARY_DIR}/src/scene/sceneparser.cpp
            ${CMAKE_SOURCE_DIR}/src/scene/sceneparser.l
    )

ADD_CUSTOM_COMMAND(
    SOURCE ${CMAKE_SOURCE_DIR}/src/scene/sceneparser.y
    OUTPUT ${CMAKE_BINARY_DIR}/src/scene/sceneparser.tab.cpp
           ${CMAKE_BINARY_DIR}/src/scene/sceneparser.tab.hpp
           ${CMAKE_BINARY_DIR}/src/scene/location.hh
           ${CMAKE_BINARY_DIR}/src/scene/position.hh
           ${CMAKE_BINARY_DIR}/src/scene/stack.hh
    COMMAND ${BISON_EXECUTABLE}
    ARGS -y ${CMAKE_SOURCE_DIR}/src/scene/sceneparser.y
         -o ${CMAKE_BINARY_DIR}/src/scene/sceneparser.tab.cpp
    DEPENDS ${CMAKE_BINARY_DIR}/src/scene/sceneparser.cpp
)

add_library(Scene STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/objparser.cpp
    ${CMAKE_BINARY_DIR}/src/scene/sceneparser.cpp
    ${CMAKE_BINARY_DIR}/src/scene/sceneparser.tab.cpp
    )

target_link_libraries(Scene
    ${FLEX_LIBRARIES}
    Acceleration
    Geometry
    Light
    Materials
    Tracer
    )
