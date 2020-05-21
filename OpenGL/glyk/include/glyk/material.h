#ifndef _MATERIAL_H_
#define _MATERIAL_H_


#include <string>
#include "glyk/gl_header_files.h"
#include "glyk/vec3.h"
#include "glyk/texture.h"


class Mesh;


/***************** Material *****************/

class Material
{

protected:

    static GLuint empty_texture_id;
    std::string shader_name;
    virtual void PipeUniformData(GLuint shader_id) const = 0;
    virtual std::string ShaderMaterialUniformIdentifier(const std::string& member_name) const = 0;

    friend class Mesh;

public:

    Material();
    Material(const std::string& shader_name);
    virtual ~Material();
    std::string ShaderName() const;

};


/***************** BasicMaterial *****************/

class BasicMaterial : public Material
{
    
protected:

    Vec3f diffuse;
    Vec3f specular;
    float shininess;
    Texture diffuse_map;
    Texture specular_map;
    Texture shininess_map;
    Texture normal_map;

    virtual void PipeUniformData(GLuint shader_id) const;
    virtual std::string ShaderMaterialUniformIdentifier(const std::string& member_name) const;

public:

    static std::string material_uniform_name;

    BasicMaterial();
    BasicMaterial(const std::string& shader_name);
    BasicMaterial(
        const std::string& shader_name,
        const Vec3f& diffuse,
        const Vec3f& specular,
        float shininess);
    BasicMaterial(
        const std::string& shader_name,
        const Vec3f& diffuse,
        const Vec3f& specular,
        float shininess,
        const Texture& normal_map);
    BasicMaterial(
        const std::string& shader_name,
        const Vec3f& diffuse);
    BasicMaterial(
        const std::string& shader_name,
        const Vec3f& diffuse,
        const Texture& normal_map);
    BasicMaterial(
        const std::string& shader_name,
        const Texture& diffuse_map,
        const Texture& specular_map,
        float shininess);
    BasicMaterial(
        const std::string& shader_name,
        const Texture& diffuse_map,
        const Texture& specular_map,
        float shininess,
        const Texture& normal_map);
    BasicMaterial(
        const std::string& shader_name,
        const Texture& diffuse_map,
        const Texture& specular_map,
        const Texture& shininess_map);
    BasicMaterial(
        const std::string& shader_name,
        const Texture& diffuse_map,
        const Texture& specular_map,
        const Texture& shininess_map,
        const Texture& normal_map);
    BasicMaterial(
        const std::string& shader_name,
        const Texture& diffuse_map);
    BasicMaterial(
        const std::string& shader_name,
        const Texture& diffuse_map,
        const Texture& normal_map);
    Vec3f Diffuse() const;
    Vec3f Specular() const;
    float Shininess() const;
    Texture DiffuseMap() const;
    Texture SpecularMap() const;
    Texture ShininessMap() const;
    Texture NormalMap() const;

};


#endif
