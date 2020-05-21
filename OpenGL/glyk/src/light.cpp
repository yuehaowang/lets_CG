#include "glyk/light.h"
#include "glyk/geometry.h"


/***************** Light *****************/

Light::Light()
: InvisibleObject3D()
{

}

Light::Light(const Vec3f& a, const Vec3f& d, const Vec3f& s)
: InvisibleObject3D()
, ambient(a)
, diffuse(d)
, specular(s)
{

}

Light::~Light()
{

}

Light::LightType Light::Type() const
{
    return type;
}

Vec3f Light::Ambient() const
{
    return ambient;
}

Vec3f Light::Diffuse() const
{
    return diffuse;
}
Vec3f Light::Specular() const
{
    return specular;
}

void Light::SetAmbient(const Vec3f& v)
{
    ambient = v;
}

void Light::SetDiffuse(const Vec3f& v)
{
    diffuse = v;
}

void Light::SetSpecular(const Vec3f& v)
{
    specular = v;
}

void Light::PipeUniformData(GLuint shader_id, unsigned int light_index)
{
    glUniform3f(
        glGetUniformLocation(shader_id, ShaderLightsUniformIdentifier(light_index, "ambient").c_str()),
        ambient.x, ambient.y, ambient.z
    );
    glUniform3f(
        glGetUniformLocation(shader_id, ShaderLightsUniformIdentifier(light_index, "diffuse").c_str()),
        diffuse.x, diffuse.y, diffuse.z
    );
    glUniform3f(
        glGetUniformLocation(shader_id, ShaderLightsUniformIdentifier(light_index, "specular").c_str()),
        specular.x, specular.y, specular.z
    );

    glUniform1i(
        glGetUniformLocation(shader_id, ShaderLightCountUniformIdentifier().c_str()), light_index + 1
    );
}


/***************** DirectionalLight *****************/

std::string DirectionalLight::lights_uniform_name = "dir_lights";

DirectionalLight::DirectionalLight(
    const Vec3f& a,
    const Vec3f& d,
    const Vec3f& s)
: Light(a, d, s)
{
    type = Directional;
}

void DirectionalLight::PipeUniformData(GLuint shader_id, unsigned int light_index)
{
    Light::PipeUniformData(shader_id, light_index);

    Vec3f direction = Forward();
    glUniform3f(
        glGetUniformLocation(shader_id, ShaderLightsUniformIdentifier(light_index, "direction").c_str()),
        direction.x, direction.y, direction.z
    );
}

std::string DirectionalLight::ShaderLightsUniformIdentifier(unsigned int index, const std::string& member_name)
{
    return lights_uniform_name + "[" + std::to_string(index) + "]." + member_name;
}

std::string DirectionalLight::ShaderLightCountUniformIdentifier()
{
    return lights_uniform_name + "_count";
}

void DirectionalLight::ShowIndicator()
{
    AddIndicator(new AxesIndicator());
    AddIndicator(new IconIndicator("glyk/resources/icon_dir_light.png"));
}


/***************** PointLight *****************/

std::string PointLight::lights_uniform_name = "pt_lights";

PointLight::PointLight(
    const Vec3f& a,
    const Vec3f& d,
    const Vec3f& s,
    float quadratic,
    float linear,
    float constant)
: Light(a, d, s)
, quadratic(quadratic)
, linear(linear)
, constant(constant)
{
    type = Point;
}

void PointLight::PipeUniformData(GLuint shader_id, unsigned int light_index)
{
    Light::PipeUniformData(shader_id, light_index);

    Vec3f pos = Position();
    glUniform3f(
        glGetUniformLocation(shader_id, ShaderLightsUniformIdentifier(light_index, "position").c_str()),
        pos.x, pos.y, pos.z
    );
    
    glUniform1f(
        glGetUniformLocation(shader_id, ShaderLightsUniformIdentifier(light_index, "quadratic").c_str()), quadratic
    );
    glUniform1f(
        glGetUniformLocation(shader_id, ShaderLightsUniformIdentifier(light_index, "linear").c_str()), linear
    );
    glUniform1f(
        glGetUniformLocation(shader_id, ShaderLightsUniformIdentifier(light_index, "constant").c_str()), constant
    );
}

std::string PointLight::ShaderLightsUniformIdentifier(unsigned int index, const std::string& member_name)
{
    return lights_uniform_name + "[" + std::to_string(index) + "]." + member_name;
}

std::string PointLight::ShaderLightCountUniformIdentifier()
{
    return lights_uniform_name + "_count";
}

void PointLight::ShowIndicator()
{
    AddIndicator(new AxesIndicator());
    AddIndicator(new IconIndicator("glyk/resources/icon_pt_light.png"));
}
