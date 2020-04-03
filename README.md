# Let's OpenGL

Learning and Practice of Modern OpenGL.


## Setup

### Build Tools

- GCC/Clang (C++ 11 supported)
- CMake

### On macOS

Install packages below via *Homebrew*. In addition, you may also need to install *Xcode*, which encompasses C/C++ build tools and essential frameworks for development.

```bash
# The OpenGL extension wrangler library
brew install glew
# Multi-platform Library for OpenGL
brew install glfw3
# OpenGL Mathematics
brew install glm
```

### On Ubuntu

Install packages below via *apt-get*.

```bash
sudo apt-get install cmake libx11-dev xorg-dev libglu1-mesa-dev freeglut3-dev libglew1.5 libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev libglm-dev
```


## Build & Run

Type commands below to build the project.

```bash
# Clone this repo
git clone https://github.com/yuehaowang/lets_openGL.git
# Get into the project directory
cd ./lets_openGL
# Create a build/ directory
mkdir build
cd build/
# Build
cmake ..
make
```

If you manage to build the project, you can run binaries as below:

```bash
# Draw a triangle
triangle/triangle

# Bind textures to 2D geometries
texture/texture

# Draw a 3D geometry
3d_geom/3d_geom

# Test mathematical computation
math_test/math_test

# Bind textures to 3D geometries
cube_texture/cube_texture

# Phong shading
lighting/lighting

```


## Notes

### Row-major v.s. Column-major order

`mat` in GLSL reads input data in column-major order not row-major order, e.g. `mat2(1, 2, 3, 4)` will construct a matrix like

```math
1  3
2  4
```

Therefore, matrices stored in row-major order should be transposed before sent to shader programs. A more general way is to set the 3rd parameter of `glUnifromMatrixXXX` to `GL_TRUE`. Also note, some mathematical frameworks like *glm* stores matrices in column-major order and `mat[i][j]` gives element at column *i* and row *j*, which is different from the mathematical convention.


## References

- [Learn OpenGL](https://learnopengl.com/)
- [OpenGL Projection Matrix](http://www.songho.ca/opengl/gl_projectionmatrix.html)
- [WebGL Fundamentals](https://webglfundamentals.org/)
- [How to install OpenGL environment on Ubuntu](https://medium.com/@Plimsky/how-to-install-a-opengl-environment-on-ubuntu-e3918cf5ab6c)
- [CMake Examples](http://ttroy50.github.io/cmake-examples/)
- Dunn, Fletcher, and Ian Parberry. 3D math primer for graphics and game development. Jones & Bartlett Publishers, 2010.