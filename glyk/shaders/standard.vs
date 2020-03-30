#version 330 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texcoord;

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
out vec2 frag_texcoord;

void main()
{
    vec4 world_pos = mesh.model * vec4(vertex_pos, 1.0);

    frag_pos = world_pos.xyz;
    frag_normal = mat3(transpose(inverse(mesh.model))) * vertex_normal;
    frag_texcoord = vertex_texcoord;
    gl_Position = camera.projection * camera.view * world_pos;
}