#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <iostream>
#include "gl_header_files.h"
#include "vec3.h"
#include "displayobject.h"


class Scene;


/***************** Light *****************/

class Light : public DisplayObject
{

protected:

    Vec3<GLfloat> ambient;
    Vec3<GLfloat> diffuse;
    Vec3<GLfloat> specular;
    
    virtual void PipeData(GLuint shader_id, unsigned int light_index);
    virtual std::string ShaderGetStructUniformIdentifier(unsigned int light_index, const std::string& member_name) = 0;
    virtual std::string ShaderGetCountUniformIdentifier() = 0;

    friend class Scene;

public:

    Light();
    Light(const Vec3<GLfloat>& a, const Vec3<GLfloat>& d, const Vec3<GLfloat>& s);
    virtual ~Light();
    const Vec3<GLfloat> Ambient() const;
    const Vec3<GLfloat> Diffuse() const;
    const Vec3<GLfloat> Specular() const;
    void SetAmbient(const Vec3<GLfloat>& v);
    void SetDiffuse(const Vec3<GLfloat>& v);
    void SetSpecular(const Vec3<GLfloat>& v);

};


/***************** DirectionalLight *****************/

class DirectionalLight : public Light
{

protected:

    Vec3<GLfloat> direction;
    static std::string lights_uniform_name;

    void PipeData(GLuint shader_id, unsigned int light_index);
    std::string ShaderGetStructUniformIdentifier(unsigned int light_index, const std::string& member_name);
    std::string ShaderGetCountUniformIdentifier();

public:

    DirectionalLight(const Vec3<GLfloat>& a, const Vec3<GLfloat>& d, const Vec3<GLfloat>& s, const Vec3<GLfloat>& dir);
    const Vec3<GLfloat> Direction() const;
    void SetDirection(const Vec3<GLfloat>& dir);

};


#endif
