#version 330 core

in vec3 vert_color;
in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    frag_color = mix(texture(texture0, tex_coord), texture(texture1, tex_coord), 0.5) * vec4(vert_color, 1.0);
}