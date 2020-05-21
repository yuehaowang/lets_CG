project(model_texture)

add_executable(${PROJECT_NAME} main.cpp)

find_resources()

target_link_libraries(${PROJECT_NAME}
    glyk stb_image tiny_obj_loader ${LIBS}
)