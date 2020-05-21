project(texture)

add_executable(${PROJECT_NAME} main.cpp)

find_shaders()

target_link_libraries(${PROJECT_NAME}
    glyk stb_image ${LIBS}
)