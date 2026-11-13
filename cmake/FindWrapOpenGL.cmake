# Minimal FindWrapOpenGL for systems without OpenGL development headers.
# Qt6Gui requires the WrapOpenGL::WrapOpenGL interface target at configure time.
if(TARGET WrapOpenGL::WrapOpenGL)
    set(WrapOpenGL_FOUND ON)
    return()
endif()

add_library(WrapOpenGL::WrapOpenGL INTERFACE IMPORTED)

if(TARGET OpenGL::GL)
    target_link_libraries(WrapOpenGL::WrapOpenGL INTERFACE OpenGL::GL)
else()
    find_library(QTASKMAN_LIBGL NAMES GL libGL.so.1 PATHS /usr/lib64 /lib64 /usr/lib /lib)
    if(QTASKMAN_LIBGL)
        target_link_libraries(WrapOpenGL::WrapOpenGL INTERFACE "${QTASKMAN_LIBGL}")
    endif()
endif()

set(WrapOpenGL_FOUND ON)
