#version 330 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_normal;

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

out vec3 frag_pos;
out vec3 frag_normal;

void main()
{
    gl_Position = camera.projection * camera.view * mesh.model * vec4(vertex_pos, 1);

    frag_pos = vec3(gl_Position);
    frag_normal = mat3(transpose(inverse(mesh.model))) * vertex_normal;
}