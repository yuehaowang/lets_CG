cmake_minimum_required (VERSION 3.6)

project(lets_opengl)


###### Add include and link directories ######
# Add extra include directories
include_directories(/usr/local/include)
# Add extra lib directories
link_directories(/usr/local/lib)


###### Enable C++ 11 ######
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")


###### Find packages ######
# glfw
find_package(glfw3 3.3 REQUIRED)
# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
# glm
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Config libraries
if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    set(LIBS
        glfw GLEW::GLEW OpenGL::GL ${GLM_LIBRARIES}
        ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY}
    )
endif (APPLE)


###### Copy common resources ######
include(FindResources.cmake)


###### Add subdirectories ######
add_subdirectory(glyk)
add_subdirectory(thirdparty)
add_subdirectory(triangle)
add_subdirectory(math_test)
add_subdirectory(texture)
add_subdirectory(3d_geom)
add_subdirectory(cube_texture)
add_subdirectory(lighting)
