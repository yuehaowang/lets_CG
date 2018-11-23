# Intro

This is a repo where I upload my OpenGL learning notes and code.

Reference: https://learnopengl.com/

# Setup

## On macOS

### GLEW

```
brew install glew
```

### GLFW3

```
brew install glfw3
```

### GLM (if needed)

```
brew install glm
```

## On Ubuntu

```
sudo apt-get install cmake libx11-dev xorg-dev libglu1-mesa-dev freeglut3-dev libglew1.5 libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev libglm-dev
```

Reference: https://medium.com/@Plimsky/how-to-install-a-opengl-environment-on-ubuntu-e3918cf5ab6c

# How to Run

```
make target_name
```
All of the `target_name` will be listed by typing `make`.


# Notes

## Matrix

### Matrix Transposition

```
(AB)' = B'A'
```

### Inverse Matrix

```
AA^-1 = 1

(AB)^-1 = B^-1 * A^-1
```

### Matrix Transformation

**Model**

Transformation: translate -> rotate -> scale

```
model_mat_4x4 = Translation * Rotation * Scaling
```

**Model - View - Projection (MVP)**

MVP matrix is used to transform space coordinates to image plane: Word coordinates (* Model) -> Camera coordinates (* View) -> Image plane coordinates (* Projection).


```
vertex_pos_2d = Projection * View * Model * pos_vec_3d
```

### Caveats

`mat` in GLSL reads input data one after another but `mat` is constructed by columns (a `vec`) not rows, which means `mat2(1, 2, 3, 4)` will construct a matrix:

```
1  3
2  4
```

Therefore, every matrix passed to shader program by `glUnifromMatrixXXX` will be transposed via  `mat`'s data-reading process if `glUnifromMatrixXXX`'s 3rd parameter is not set to `GL_TRUE`. However, some math libraries like glm will provide transposed transformation matrix, for example, translation matrix constructed by the code below will give a transposed matrix:

```cpp
glm::mat4 trans;
trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

/*

trans:

1 0 0 0
0 1 0 0
0 0 1 0
x y z 1

but translation matrix is expected to look like:

1 0 0 x
0 1 0 y
0 0 1 z
0 0 0 1

*/
```

Also note, `glm::mat` is similar to `mat`, namely constructed by columns and `m[i][j]` gives element at column *i* and row *j* (different from the mathematical convention). 
