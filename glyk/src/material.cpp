#include "glyk/material.h"
#include <iostream>


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


/***************** BasicMaterial *****************/

std::string BasicMaterial::material_uniform_name = "material";

BasicMaterial::BasicMaterial()
: Material("")
, shininess(0)
{

}

BasicMaterial::BasicMaterial(
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

BasicMaterial::BasicMaterial(
    const std::string& shader_name,
    const Vec3f& diffuse)
: Material(shader_name)
, shininess(0)
, diffuse(diffuse)
{

}

BasicMaterial::BasicMaterial(
    const std::string& shader_name,
    const Texture& diffuse_map,
    const Texture& specular_map,
    float shininess)
: Material(shader_name)
, shininess(shininess)
, diffuse_map(diffuse_map)
, specular_map(specular_map)
{

}

BasicMaterial::BasicMaterial(
    const std::string& shader_name,
    const Texture& diffuse_map,
    const Texture& specular_map,
    const Texture& shininess_map)
: Material(shader_name)
, shininess(0)
, diffuse_map(diffuse_map)
, specular_map(specular_map)
, shininess_map(shininess_map)
{

}

BasicMaterial::BasicMaterial(
    const std::string& shader_name,
    const Texture& diffuse_map)
: Material(shader_name)
, shininess(0)
, diffuse_map(diffuse_map)
{

}

Vec3f BasicMaterial::Diffuse() const
{
    return diffuse;
}

Vec3f BasicMaterial::Specular() const
{
    return specular;
}

float BasicMaterial::Shininess() const
{
    return shininess;
}

void BasicMaterial::PipeUniformData(GLuint shader_id) const
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

    if (!diffuse_map.IsNull()) {
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shader_id, ShaderMaterialUniformIdentifier("diffuse_map").c_str()), 0);
        glBindTexture(GL_TEXTURE_2D, diffuse_map.TexId());
    } else {
        glUniform1i(glGetUniformLocation(shader_id, ShaderMaterialUniformIdentifier("diffuse_map").c_str()), 5);
    }
    if (!specular_map.IsNull()) {
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(shader_id, ShaderMaterialUniformIdentifier("specular_map").c_str()), 1);
        glBindTexture(GL_TEXTURE_2D, specular_map.TexId());
    } else {
        glUniform1i(glGetUniformLocation(shader_id, ShaderMaterialUniformIdentifier("specular_map").c_str()), 5);
    }
    if (!shininess_map.IsNull()) {
        glActiveTexture(GL_TEXTURE2);
        glUniform1i(glGetUniformLocation(shader_id, ShaderMaterialUniformIdentifier("shininess_map").c_str()), 2);
        glBindTexture(GL_TEXTURE_2D, shininess_map.TexId());
    } else {
        glUniform1i(glGetUniformLocation(shader_id, ShaderMaterialUniformIdentifier("shininess_map").c_str()), 5);
    }
}

std::string BasicMaterial::ShaderMaterialUniformIdentifier(const std::string& member_name) const
{
    return material_uniform_name + "." + member_name;
}
