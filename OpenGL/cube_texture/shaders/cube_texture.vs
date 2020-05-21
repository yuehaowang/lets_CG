#version 330 core

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_tex_coord;

out vec3 vert;
out vec2 tex_coord;

uniform mat4 trans;

void main()
{
    gl_Position = trans * vec4(in_vert, 1.0);
    
    tex_coord = in_tex_coord;
    vert = in_vert;
}