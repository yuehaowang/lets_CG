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
file(GLOB SHADER_FILES
    "shaders/*.vs"
    "shaders/*.fs"
)

foreach(SHADER ${SHADER_FILES})
    file(COPY ${SHADER} DESTINATION ${PROJECT_BINARY_DIR}/shaders/)
endforeach(SHADER)

# message(STATUS ${PROJECT_BINARY_DIR})


###### Set include directories ######
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)


###### Link thirdparty libraries ######
target_link_libraries(${PROJECT_NAME}
    PRIVATE stb_image
)
