project(glyk)


###### Add source files ######
add_library(${PROJECT_NAME}
    src/camera.cpp
    src/geometry.cpp
    src/light.cpp
    src/loader.cpp
    src/image.cpp
    src/material.cpp
    src/mesh.cpp
    src/object3d.cpp
    src/indicator.cpp
    src/invisibleobject3d.cpp
    src/scene.cpp
    src/shader.cpp
    src/texture.cpp
    src/window.cpp
)


###### Find shader files ######
find_shaders()

###### Find resource files ######
find_resources()


###### Set include directories ######
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)


###### Link thirdparty libraries ######
target_link_libraries(${PROJECT_NAME}
    PRIVATE stb_image tiny_obj_loader
)
