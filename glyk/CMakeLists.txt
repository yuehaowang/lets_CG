project(glyk)


###### Add source files ######
add_library(${PROJECT_NAME}
    src/camera.cpp
    src/geometry.cpp
    src/light.cpp
    src/loader.cpp
    src/material.cpp
    src/mesh.cpp
    src/object3d.cpp
    src/scene.cpp
    src/shader.cpp
    src/window.cpp
)


###### Add shader files ######
include(${CMAKE_SOURCE_DIR}/FindShaders.cmake)


###### Set include directories ######
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)


###### Link thirdparty libraries ######
target_link_libraries(${PROJECT_NAME}
    PRIVATE stb_image
)
