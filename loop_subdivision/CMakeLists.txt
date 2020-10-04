cmake_minimum_required(VERSION 3.0.0)
project(ray-tracing VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS  "-O3")


# Add include path
include_directories(
	src
	head
	include
)

# Find source files
file(GLOB_RECURSE SOURCE_FILES 
    src/main.cpp
	head/*.hpp)


# Specific flags
if(WIN32)
	add_definitions(-D_AMD64_)
endif()

# Executables
add_executable(loop-subdivision ${SOURCE_FILES})

