# CC specifies which compiler we're using
CC = g++ -g -std=c++11 -O0

# INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include

# LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -Werror -Wall

# DETECTED_OS specifies the current computer OS
ifeq ($(OS), Windows_NT)
	DETECTED_OS := Windows
else
	DETECTED_OS := $(shell uname -s)
endif

# LINKER_FLAGS specifies the libraries we're linking against

# macOS
ifeq ($(DETECTED_OS), Darwin)
	# Cocoa, IOKit, and CoreVideo are needed for static GLFW3
	LINKER_FLAGS = -framework OpenGL -lglfw -lglew
else ifeq ($(DETECTED_OS), Linux)
	LINKER_FLAGS = -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
endif

# SOURCE specifies which files to compile as part of the project
LIB_OBJS = build/loader.o \
		   build/shader.o \
		   build/window.o \
		   build/mesh.o   \
		   build/displayobject.o \
		   build/camera.o \
		   build/light.o  \
		   build/scene.o  \
		   build/material.o

# EXECUTABLE specifies the path of the executable file
EXECUTABLE = build/$@

# COMMON_OPTS specifies some repeated compiling options
COMMON_OPTS = $(LIB_OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS)

# target hints
CHOOSE_TARGET_HINT = "\
	Available Targets: \n \
		\n \
		\t 0. triangle \n \
		\t 1. 3d_geom \n \
		\t 2. math_test \n \
		\t 3. texture \n \
		\t 4. cube_texture \n \
\n (e.g. make 3d_geom)"

# common headers
GL_HEADER = src/utils/gl_header_files.h
MATH_HEADERS = src/utils/mat4x4.h src/utils/vec3.h
IMAGE_HEADERS = src/utils/external/stb_image.h src/utils/image.h
OVERALL_HEADERS = $(GL_HEADER) $(MATH_HEADERS) $(IMAGE_HEADERS)

define RUN_EXECUTABLE
	
	$(CC) build/$@.o $(COMMON_OPTS) -o $(EXECUTABLE)
	@echo "\n\nRunning...\n\n"
	$(EXECUTABLE)
	
endef


# all
all :
	@echo $(CHOOSE_TARGET_HINT)


# triangle
triangle : $(LIB_OBJS) build/triangle.o
	@mkdir -p build
	$(RUN_EXECUTABLE)

build/triangle.o : src/triangle.cpp src/utils/gl_header_files.h
	@mkdir -p build
	$(CC) -c src/triangle.cpp -o build/triangle.o


# 3D geom demo
3d_geom : $(LIB_OBJS) build/3d_geom.o
	@mkdir -p build
	$(RUN_EXECUTABLE)

build/3d_geom.o : src/3d_geom.cpp $(GL_HEADER) $(MATH_HEADERS)
	@mkdir -p build
	$(CC) -c src/3d_geom.cpp -o build/3d_geom.o


# matrix test
math_test : $(LIB_OBJS) build/math_test.o
	@mkdir -p build
	$(RUN_EXECUTABLE)

build/math_test.o : src/math_test.cpp $(MATH_HEADERS)
	@mkdir -p build
	$(CC) -c src/math_test.cpp -o build/math_test.o


# texture
texture : $(LIB_OBJS) build/texture.o
	@mkdir -p build
	$(RUN_EXECUTABLE)

build/texture.o : src/texture.cpp $(OVERALL_HEADERS)
	@mkdir -p build
	$(CC) -c src/texture.cpp -o build/texture.o


# cube texture
cube_texture : $(LIB_OBJS) build/cube_texture.o
	@mkdir -p build
	$(RUN_EXECUTABLE)

build/cube_texture.o : src/cube_texture.cpp $(OVERALL_HEADERS)
	@mkdir -p build
	$(CC) -c src/cube_texture.cpp -o build/cube_texture.o


# lighting
lighting : $(LIB_OBJS) build/lighting.o
	@mkdir -p build
	$(RUN_EXECUTABLE)

build/lighting.o : src/lighting.cpp $(OVERALL_HEADERS)
	@mkdir -p build
	$(CC) -c src/lighting.cpp -o build/lighting.o


# utils
build/loader.o : src/utils/loader.cpp src/utils/loader.h src/utils/external/stb_image.h src/utils/image.h
	@mkdir -p build
	$(CC) -c src/utils/loader.cpp -o build/loader.o

build/shader.o : src/utils/shader.cpp src/utils/shader.h src/utils/gl_header_files.h build/loader.o
	@mkdir -p build
	$(CC) -c src/utils/shader.cpp -o build/shader.o

build/window.o : src/utils/window.cpp src/utils/window.h src/utils/gl_header_files.h
	@mkdir -p build
	$(CC) -c src/utils/window.cpp -o build/window.o

build/displayobject.o : src/utils/displayobject.cpp src/utils/displayobject.h src/utils/gl_header_files.h $(MATH_HEADERS)
	@mkdir -p build
	$(CC) -c src/utils/displayobject.cpp -o build/displayobject.o

build/mesh.o : src/utils/mesh.cpp src/utils/mesh.h src/utils/material.h src/utils/gl_header_files.h build/shader.o build/displayobject.o
	@mkdir -p build
	$(CC) -c src/utils/mesh.cpp -o build/mesh.o

build/camera.o : src/utils/camera.cpp src/utils/camera.h src/utils/gl_header_files.h build/displayobject.o
	@mkdir -p build
	$(CC) -c src/utils/camera.cpp -o build/camera.o

build/light.o : src/utils/light.cpp src/utils/light.h src/utils/gl_header_files.h build/displayobject.o
	@mkdir -p build
	$(CC) -c src/utils/light.cpp -o build/light.o

build/material.o : src/utils/material.cpp src/utils/material.h src/utils/gl_header_files.h $(MATH_HEADERS)
	@mkdir -p build
	$(CC) -c src/utils/material.cpp -o build/material.o

build/scene.o : src/utils/scene.cpp src/utils/scene.h src/utils/gl_header_files.h build/light.o build/mesh.o build/camera.o $(MATH_HEADERS)
	@mkdir -p build
	$(CC) -c src/utils/scene.cpp -o build/scene.o


# clear build directory
.PHONY : clean
clean :
	-rm build/*
