#version 330 core

layout(location = 0) in vec3 pos;

out vec3 color;
out mat3 color_table;

void main()
{
    gl_Position = vec4(pos, 1.0);

    color = vec3(1.0, 0, 0);
}