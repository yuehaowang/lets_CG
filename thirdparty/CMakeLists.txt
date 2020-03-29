project(thirdparty)

###### STB Image ######
add_library(stb_image INTERFACE)

target_include_directories(stb_image
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)


###### Tiny Obj Loader ######
add_library(tiny_obj_loader INTERFACE)

target_include_directories(tiny_obj_loader
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)