project(texture)

add_executable(${PROJECT_NAME} main.cpp)

file(GLOB SHADER_FILES
    "shaders/*.vs"
    "shaders/*.fs"
)

foreach(SHADER ${SHADER_FILES})
    file(COPY ${SHADER} DESTINATION ${PROJECT_BINARY_DIR}/shaders/)
endforeach(SHADER)

target_link_libraries(${PROJECT_NAME}
    glyk stb_image ${LIBS}
)