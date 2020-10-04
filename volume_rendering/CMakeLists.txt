cmake_minimum_required(VERSION 3.0.0)
project(volume_rendering VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS  "-O3")

# Find OpenMP
find_package(OpenMP)
if(OpenMP_FOUND)
	include_directories(${OpenMP_CXX_INCLUDE_DIRS})
endif()

# Add include path
include_directories(
	head
	src
	include
)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	src/*.c
    src/*.cpp
	src/*.cc)

message(STATUS "Following files are complied " ${SOURCE_FILES})

# OS-specific flags
if(WIN32)
	add_definitions(-D_AMD64_)
endif()

# Find scripts
file(GLOB SCRIPT_FILES "*.sh")

foreach(RESOURCE ${SCRIPT_FILES})
    file(COPY ${RESOURCE} DESTINATION ${PROJECT_BINARY_DIR})
endforeach(RESOURCE)

message(STATUS "Find scripts: ${SCRIPT_FILES}")

# Add executable target
add_executable(volume_rendering ${SOURCE_FILES})
target_link_libraries(volume_rendering OpenMP::OpenMP_CXX)
