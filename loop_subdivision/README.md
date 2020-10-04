# Loop Subdivision

An implementation of Loop subdivision.

## Dependencies

- Eigen

## Usage

1. Build this project: 

    ```
    mkdir build && cd ./build
    cmake ..
    make
    ```

2. Place your .obj model to the *resources/* directory.

3. Suppose your model filename is *abc.obj*. Type the command to perform subdivision on your model:

    ```
    ./loop-subdivision abc
    ```

4. Result is saved to *abc_subdiv.obj*.


## Results

### Cloud_1

- Before:

![Cloud_1 Original](results/Cloud_1.png)

- After subdivision:

![Cloud_1 Subdivision](results/Cloud_1_subdiv.png)

### Pikachu

- Before:

![Pikachu Original](results/pikachu.png)

- After subdivision:

![Pikachu Subdivision](results/pikachu_subdiv.png)