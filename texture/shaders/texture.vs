#version 330 core

layout(location = 0) in vec3 vert;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 in_tex_coord;

uniform mat4 trans;

out vec3 vert_color;
out vec2 tex_coord;

void main()
{
    gl_Position = trans * vec4(vert, 1.0);

    vert_color = color;
    tex_coord = in_tex_coord;
}
