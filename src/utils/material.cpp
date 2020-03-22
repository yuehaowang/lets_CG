#include "material.h"


/***************** Material *****************/

Material::Material()
: shader_name("")
{

}

Material::Material(const std::string& shader)
: shader_name(shader)
{

}

Material::~Material()
{

}

std::string Material::ShaderName() const
{
    return shader_name;
}


/***************** BasicColorMaterial *****************/

std::string BasicColorMaterial::material_uniform_name = "material";

BasicColorMaterial::BasicColorMaterial()
: Material("")
, diffuse(Vec3f(0.0f, 0.0f, 0.0f))
, specular(Vec3f(0.0f, 0.0f, 0.0f))
, shininess(0)
{

}

BasicColorMaterial::BasicColorMaterial(
    const std::string& shader_name,
    const Vec3f& diffuse,
    const Vec3f& specular,
    float shininess)
: Material(shader_name)
, diffuse(diffuse)
, specular(specular)
, shininess(shininess)
{

}

Vec3f BasicColorMaterial::Diffuse() const
{
    return diffuse;
}

Vec3f BasicColorMaterial::Specular() const
{
    return specular;
}

float BasicColorMaterial::Shininess() const
{
    return shininess;
}

void BasicColorMaterial::PipeUniformData(GLuint shader_id) const
{
    glUniform3f(
        glGetUniformLocation(shader_id, ShaderMaterialUniformIdentifier("diffuse").c_str()),
        diffuse.x, diffuse.y, diffuse.z
    );
    glUniform3f(
        glGetUniformLocation(shader_id, ShaderMaterialUniformIdentifier("specular").c_str()),
        specular.x, specular.y, specular.z
    );
    glUniform1f(
        glGetUniformLocation(shader_id, ShaderMaterialUniformIdentifier("shininess").c_str()),
        shininess
    );
}

std::string BasicColorMaterial::ShaderMaterialUniformIdentifier(const std::string& member_name) const
{
    return material_uniform_name + "." + member_name;
}
