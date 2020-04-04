#version 330 core


/* Input */
in vec2 frag_texcoord;

/* Ouput */
out vec4 frag_color;

/* Material */
struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D shininess_map;
    sampler2D normal_map;
};
uniform Material material;


void main()
{
    vec4 tex_color = texture(material.diffuse_map, frag_texcoord);
    tex_color.a *= 0.5;
    
    if(tex_color.a < 0.01) discard;
    
    frag_color = tex_color;
}