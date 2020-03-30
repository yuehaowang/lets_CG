#version 330 core

#define MAX_LIGHT_NUM 100


/* Input */
in vec3 frag_pos;
in vec3 frag_normal;
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
};
uniform Material material;


/* Directional Light */
struct DirectionalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};
uniform DirectionalLight dir_lights[MAX_LIGHT_NUM];
uniform int dir_lights_count;

vec3 compute_dir_light(Material material, vec3 normal, vec3 view_dir)
{
    vec3 res = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < dir_lights_count && i < MAX_LIGHT_NUM; i++) {
        DirectionalLight light = dir_lights[i];

        vec3 light_dir = normalize(-light.direction);

        vec3 mat_diffuse = min(material.diffuse + vec3(texture(material.diffuse_map, frag_texcoord)), 1.0);
        vec3 mat_specular = min(material.specular + vec3(texture(material.specular_map, frag_texcoord)), 1.0);
        float mat_shininess = material.shininess + texture(material.shininess_map, frag_texcoord).r + 0.0001;

        // ambient
        vec3 ambient = light.ambient * mat_diffuse;
        // diffuse
        vec3 diffuse = light.diffuse * max(dot(normal, light_dir), 0.0) * mat_diffuse;
        // specular
        vec3 reflect_dir = reflect(-light_dir, normal);
        vec3 specular = light.specular * pow(max(dot(view_dir, reflect_dir), 0.0), mat_shininess) * mat_specular;

        res += ambient + diffuse + specular;
    }

    return res;
}


/* Camera */
struct Camera {
    mat4 view;
    mat4 projection;
    vec3 position;
};
uniform Camera camera;


void main()
{
    vec3 normal = normalize(frag_normal);
    vec3 view_dir = normalize(camera.position - frag_pos);

    vec3 color = vec3(0.0, 0.0, 0.0);

    // Directional Lights
    color += compute_dir_light(material, normal, view_dir);

    frag_color = vec4(color, 1.0);
}