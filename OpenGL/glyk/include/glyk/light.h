#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <iostream>
#include "glyk/gl_header_files.h"
#include "glyk/vec3.h"
#include "glyk/invisibleobject3d.h"
#include "glyk/mesh.h"
#include "glyk/material.h"
#include "glyk/indicator.h"


class Scene;


/***************** Light *****************/

class Light : public InvisibleObject3D
{

public:

    enum LightType {
        Directional = 0,
        Point
    };

protected:

    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
    LightType type;

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
    LightType Type() const;
    virtual void ShowIndicator() = 0;

};


/***************** DirectionalLight *****************/

class DirectionalLight : public Light
{

protected:

    virtual void PipeUniformData(GLuint shader_id, unsigned int light_index);
    virtual std::string ShaderLightsUniformIdentifier(unsigned int light_index, const std::string& member_name);
    virtual std::string ShaderLightCountUniformIdentifier();

public:

    static std::string lights_uniform_name;

    DirectionalLight(const Vec3f& a, const Vec3f& d, const Vec3f& s);
    virtual void ShowIndicator();

};


/***************** PointLight *****************/

class PointLight : public Light
{

protected:

    float constant;
    float linear;
    float quadratic;

    virtual void PipeUniformData(GLuint shader_id, unsigned int light_index);
    virtual std::string ShaderLightsUniformIdentifier(unsigned int light_index, const std::string& member_name);
    virtual std::string ShaderLightCountUniformIdentifier();

public:

    static std::string lights_uniform_name;

    PointLight(
        const Vec3f& a, const Vec3f& d, const Vec3f& s,
        float quadratic = 0.0f, float linear = 0.0f, float constant = 1.0f);
    virtual void ShowIndicator();

};


#endif
