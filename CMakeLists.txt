cmake_minimum_required(VERSION 3.10)
project(LiteGraphicsBackendAWT)

find_package(JNI REQUIRED)

# TODO: Replace with LiteGraphics abstraction
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad/cmake)
glad_add_library(glad EXCLUDE_FROM_ALL STATIC LOADER API gl:core=3.3)

set(LITE_GFX_AWT_SOURCES
        src/AWTSurface.cpp
        src/GLContext.cpp
        )

set(LITE_GFX_AWT_LIBRARIES
        glad
        ${JNI_LIBRARIES}
        )

if (UNIX AND NOT APPLE)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED GLX)

    target_compile_options(glad PRIVATE "-fPIC")

    list(APPEND LITE_GFX_AWT_SOURCES
            src/GLContextLinux.cpp
            )
    list(APPEND LITE_GFX_AWT_LIBRARIES
            OpenGL::GLX
            )
endif ()

add_library(litegfxawt SHARED ${LITE_GFX_AWT_SOURCES})
set_property(TARGET litegfxawt PROPERTY C_STANDARD 11)
set_property(TARGET litegfxawt PROPERTY CXX_STANDARD 11)

target_link_libraries(litegfxawt ${LITE_GFX_AWT_LIBRARIES})

target_include_directories(litegfxawt PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${GLAD_DIR}/include
        ${JNI_INCLUDE_DIRS}
        )
