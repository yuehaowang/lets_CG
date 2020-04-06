#version 330 core

#define MAX_LIGHT_NUM 10


/* Input */
in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_texcoord;
flat in vec3 frag_cam_pos;
flat in mat3 frag_TBN;

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


/* Lighting */
struct DirectionalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};
uniform DirectionalLight dir_lights[MAX_LIGHT_NUM];
uniform int dir_lights_count;

struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pt_lights[MAX_LIGHT_NUM];
uniform int pt_lights_count;

vec3 compute_phong_lighting(
    Material material, vec3 normal, vec3 view_dir, vec3 light_dir,
    vec3 light_ambient, vec3 light_diffuse, vec3 light_specular)
{
    vec3 mat_diffuse = min(material.diffuse + vec3(texture(material.diffuse_map, frag_texcoord)), 1.0);
    vec3 mat_specular = min(material.specular + vec3(texture(material.specular_map, frag_texcoord)), 1.0);
    float mat_shininess = material.shininess + texture(material.shininess_map, frag_texcoord).r + 0.0001;

    // ambient
    vec3 ambient = light_ambient * mat_diffuse;
    // diffuse
    vec3 diffuse = light_diffuse * max(dot(normal, light_dir), 0.0) * mat_diffuse;
    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 specular = light_specular * pow(max(dot(view_dir, reflect_dir), 0.0), mat_shininess) * mat_specular;

    return ambient + diffuse + specular;
}

vec3 compute_dir_light(Material material, vec3 normal, vec3 view_dir)
{
    vec3 res = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < dir_lights_count && i < MAX_LIGHT_NUM; i++) {
        DirectionalLight light = dir_lights[i];

        vec3 light_dir = normalize(-light.direction);
        res += compute_phong_lighting(
            material, normal, view_dir, light_dir,
            light.ambient, light.diffuse, light.specular
        );
    }

    return res;
}

vec3 compute_pt_light(Material material, vec3 normal, vec3 view_dir)
{
    vec3 res = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < pt_lights_count && i < MAX_LIGHT_NUM; i++) {
        PointLight light = pt_lights[i];

        vec3 light_dir = light.position - frag_pos;
        float dist = length(light_dir);
        light_dir = normalize(light_dir);
        float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
        res += compute_phong_lighting(
            material, normal, view_dir, light_dir,
            light.ambient, light.diffuse, light.specular
        ) * attenuation;
    }

    return res;
}


void main()
{
    vec3 normal;
    if (frag_TBN == mat3(0)) {
        normal = normalize(frag_normal);
    } else {
        normal = normalize(frag_TBN * (texture(material.normal_map, frag_texcoord).rgb * 2.0 - 1.0));
    }
    
    vec3 view_dir = normalize(frag_cam_pos - frag_pos);

    vec3 color = vec3(0.0, 0.0, 0.0);

    // Directional Lights
    color += compute_dir_light(material, normal, view_dir);
    // Point Lights
    color += compute_pt_light(material, normal, view_dir);

    frag_color = vec4(color, 1.0);
}