#version 330 core

layout(location = 0) in vec3 vertex_pos;


struct Mesh {
    mat4 model;
};
uniform Mesh mesh;

struct Camera {
    mat4 view;
    mat4 projection;
    vec3 position;
};
uniform Camera camera;

out vec3 color;


void main()
{
    color = vertex_pos;
    gl_Position = camera.projection * camera.view * mesh.model * vec4(vertex_pos, 1.0);
}