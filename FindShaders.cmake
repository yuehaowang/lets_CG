###### Find shaders ######
file(GLOB SHADER_FILES "shaders/*.vs" "shaders/*.fs")

foreach(SHADER ${SHADER_FILES})
    message(STATUS "Find shader file: ${SHADER}")
    file(COPY ${SHADER} DESTINATION ${PROJECT_BINARY_DIR}/shaders/)
endforeach(SHADER)