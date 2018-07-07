#version 330 core

in vec3 vert;
in vec2 tex_coord;

out vec4 frag;

uniform sampler2D tex;

void main()
{
    frag = texture(tex, tex_coord);
}