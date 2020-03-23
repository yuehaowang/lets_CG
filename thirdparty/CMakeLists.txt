project(thirdparty)

add_library(stb_image INTERFACE)

target_include_directories(stb_image
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)