#version 330 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_normal;

struct Transform {
    mat4 model;
    mat4 view;
    mat4 projection;
};
uniform Transform transform;

out vec3 frag_pos;
out vec3 frag_normal;

void main()
{
    gl_Position = transform.projection * transform.view * transform.model * vec4(vertex_pos, 1);

    frag_pos = vec3(gl_Position);
    frag_normal = mat3(transpose(inverse(transform.model))) * vertex_normal;
}