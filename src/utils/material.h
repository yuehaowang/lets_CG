#ifndef _MATERIAL_H_
#define _MATERIAL_H_


#include <string>
#include "gl_header_files.h"
#include "vec3.h"


struct BasicMaterial
{
    std::string shader_name;
    Vec3<GLfloat> diffuse;
    Vec3<GLfloat> specular;
    GLfloat shininess;

    BasicMaterial()
    : shader_name("")
    , diffuse(Vec3<GLfloat>(0.0f, 0.0f, 0.0f))
    , specular(Vec3<GLfloat>(0.0f, 0.0f, 0.0f))
    , shininess(0)
    {

    }

    BasicMaterial(
        const std::string& shader_name,
        const Vec3<GLfloat>& diffuse,
        const Vec3<GLfloat>& specular,
        GLfloat shininess)
    : shader_name(shader_name)
    , diffuse(diffuse)
    , specular(specular)
    , shininess(shininess)
    {

    }
};


#endif
