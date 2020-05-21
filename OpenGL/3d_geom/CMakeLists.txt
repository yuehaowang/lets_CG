project(3d_geom)

add_executable(${PROJECT_NAME} main.cpp)

find_shaders()

target_link_libraries(${PROJECT_NAME}
    glyk ${LIBS}
)