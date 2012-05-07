# - Try to find GLEW
# Once done this will define
#  
#  GLEW_FOUND        - system has GLEW
#  GLEW_INCLUDE_DIR  - GLEW include directory
#  GLEW_LIBRARY DIR  - GLEW library directory
#  GLEW_LIBRARIES    - Link these to use GLEW
#
if(WIN32)
    message("NO WIN32 GLEW finding yet.")
    set(GLEW_LIBRARIES glew32)
else()
    find_path(GLEW_INCLUDE_DIR GL/glew.h
        "/usr/include/GL"
        )
    find_path(GLEW_LIBRARY_DIR libGLEW.so.1.5
        "/usr/lib"
        )
    set(GLEW_LIBRARIES GLEW)
endif()

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG GLEW_LIBRARIES)

MARK_AS_ADVANCED(
    GLEW_INCLUDE_DIR
    GLEW_LIBRARIES
)
