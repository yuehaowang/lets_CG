#version 330 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;

uniform mat4 trans;

out vec3 frag_color;

void main()
{
    gl_Position = trans * vec4(vertex_pos, 1);

    frag_color = vertex_color / 255;
}