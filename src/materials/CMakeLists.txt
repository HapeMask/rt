add_library(Materials STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/aniso.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/beckmann.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/microfacet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/blinn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bsdf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frosted.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bxdf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/material.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/phong.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lambertian.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/specular.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/substrate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ward.cpp)

target_link_libraries(Materials
    Utility
    Samplers
    )
