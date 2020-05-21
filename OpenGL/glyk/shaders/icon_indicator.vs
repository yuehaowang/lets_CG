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

out vec2 frag_texcoord;


void main()
{
    mat4 model = mat4(
        vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0), mesh.model[3]);
    vec4 world_pos = model * mat4(transpose(mat3(camera.view))) * vec4(vertex_pos, 1.0);

    frag_texcoord = vertex_texcoord;
    gl_Position = camera.projection * camera.view * world_pos;
}