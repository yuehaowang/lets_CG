project(triangle)

add_executable(${PROJECT_NAME} main.cpp)

include(${CMAKE_SOURCE_DIR}/FindShaders.cmake)

target_link_libraries(${PROJECT_NAME}
    glyk ${LIBS}
)