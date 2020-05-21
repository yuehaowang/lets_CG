cmake_minimum_required(VERSION 3.0.0)
project(path-tracing VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS  "-w -O3")

# Find OpenMP
find_package(OpenMP)
include_directories(${OpenMP_CXX_INCLUDE_DIRS})

if(OpenMP_FOUND)
	message(STATUS "OpenMP Found.")
endif()

# Add Include Path
include_directories(
	src
	head
	include
)

# Find source codes
# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	src/*.c
    src/*.cpp
    head/*.hpp
	src/*.cc)
if(WIN32)
add_definitions(-D_AMD64_)
endif()
add_executable(path-tracing ${SOURCE_FILES})
# Link OpenMP
target_link_libraries(path-tracing OpenMP::OpenMP_CXX)


# Test
file(GLOB_RECURSE TEST_HEAD_FILES head/*.hpp)

add_executable(test_disk_sample test/disk_sample.cpp ${TEST_HEAD_FILES})
add_executable(test_transform test/transform.cpp ${TEST_HEAD_FILES})
