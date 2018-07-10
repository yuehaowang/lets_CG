# CC specifies which compiler we're using
CC = g++ -g

# INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include

# LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

# LINKER_FLAGS specifies the libraries we're linking against
# Cocoa, IOKit, and CoreVideo are needed for static GLFW3
LINKER_FLAGS = -framework OpenGL -lglfw -lglew

# SOURCE specifies which files to compile as part of the project
LIB_OBJS = build/loader.o \
		   build/shader.o \
		   build/window.o

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
	$(RUN_EXECUTABLE)

build/triangle.o : src/triangle.cpp src/utils/gl_header_files.h
	$(CC) -c src/triangle.cpp -o build/triangle.o


# 3D geom demo
3d_geom : $(LIB_OBJS) build/3d_geom.o
	$(RUN_EXECUTABLE)

build/3d_geom.o : src/3d_geom.cpp $(GL_HEADER) $(MATH_HEADERS)
	$(CC) -c src/3d_geom.cpp -o build/3d_geom.o


# matrix test
math_test : build/math_test.o
	$(RUN_EXECUTABLE)

build/math_test.o : src/math_test.cpp $(MATH_HEADERS)
	$(CC) -c src/math_test.cpp -o build/math_test.o


# texture
texture : $(LIB_OBJS) build/texture.o
	$(RUN_EXECUTABLE)

build/texture.o : src/texture.cpp $(OVERALL_HEADERS)
	$(CC) -c src/texture.cpp -o build/texture.o


# cube texture
cube_texture : $(LIB_OBJS) build/cube_texture.o
	$(RUN_EXECUTABLE)

build/cube_texture.o : src/cube_texture.cpp $(OVERALL_HEADERS)
	$(CC) -c src/cube_texture.cpp -o build/cube_texture.o


# utils
build/loader.o : src/utils/loader.cpp src/utils/loader.h src/utils/external/stb_image.h src/utils/image.h
	$(CC) -c src/utils/loader.cpp -o build/loader.o

build/shader.o : src/utils/shader.cpp src/utils/shader.h src/utils/gl_header_files.h build/loader.o
	$(CC) -c src/utils/shader.cpp -o build/shader.o

build/window.o : src/utils/window.cpp src/utils/window.h src/utils/gl_header_files.h
	$(CC) -c src/utils/window.cpp -o build/window.o


# clear build directory
.PHONY : clean
clean :
	-rm build/*
