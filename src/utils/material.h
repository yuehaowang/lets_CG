#ifndef _MATERIAL_H_
#define _MATERIAL_H_


#include <string>
#include "gl_header_files.h"
#include "vec3.h"


class Mesh;


/***************** Material *****************/

class Material
{

protected:

    std::string shader_name;
    virtual void PipeUniformData(GLuint shader_id) const = 0;
    virtual std::string ShaderMaterialUniformIdentifier(const std::string& member_name) const = 0;

    friend class Mesh;

public:

    Material();
    Material(const std::string& shader);
    virtual ~Material();
    std::string ShaderName() const;

};


/***************** BasicColorMaterial *****************/

class BasicColorMaterial : public Material
{
    
protected:

    Vec3<GLfloat> diffuse;
    Vec3<GLfloat> specular;
    GLfloat shininess;

    virtual void PipeUniformData(GLuint shader_id) const;
    virtual std::string ShaderMaterialUniformIdentifier(const std::string& member_name) const;

public:

    static std::string material_uniform_name;

    BasicColorMaterial();
    BasicColorMaterial(
        const std::string& shader_name,
        const Vec3<GLfloat>& diffuse,
        const Vec3<GLfloat>& specular,
        GLfloat shininess);
    Vec3<GLfloat> Diffuse() const;
    Vec3<GLfloat> Specular() const;
    GLfloat Shininess() const;

};


#endif
