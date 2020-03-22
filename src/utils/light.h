#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <iostream>
#include "gl_header_files.h"
#include "vec3.h"
#include "object3d.h"


class Scene;


/***************** Light *****************/

class Light : public Object3D
{

protected:

    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
    
    virtual void PipeUniformData(GLuint shader_id, unsigned int light_index);
    virtual std::string ShaderLightsUniformIdentifier(unsigned int light_index, const std::string& member_name) = 0;
    virtual std::string ShaderLightCountUniformIdentifier() = 0;

    friend class Scene;

public:

    Light();
    Light(const Vec3f& a, const Vec3f& d, const Vec3f& s);
    virtual ~Light();
    Vec3f Ambient() const;
    Vec3f Diffuse() const;
    Vec3f Specular() const;
    void SetAmbient(const Vec3f& v);
    void SetDiffuse(const Vec3f& v);
    void SetSpecular(const Vec3f& v);

};


/***************** DirectionalLight *****************/

class DirectionalLight : public Light
{

protected:

    void PipeUniformData(GLuint shader_id, unsigned int light_index);
    std::string ShaderLightsUniformIdentifier(unsigned int light_index, const std::string& member_name);
    std::string ShaderLightCountUniformIdentifier();

public:

    static std::string lights_uniform_name;

    DirectionalLight(const Vec3f& a, const Vec3f& d, const Vec3f& s);

};


#endif
