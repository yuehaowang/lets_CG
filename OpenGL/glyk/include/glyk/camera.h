#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "glyk/gl_header_files.h"
#include "glyk/mat4x4.h"
#include "glyk/object3d.h"


/***************** Camera *****************/

class Scene;

class Camera : public Object3D
{

protected:

    Mat4x4f projection;
    Mat4x4f view;

    virtual void PipeUniformData(GLuint shader_id);
    virtual std::string ShaderCameraUniformIdentifier(const std::string& member_name);

    friend class Scene;

public:

    static std::string camera_uniform_name;

    Camera();
    virtual ~Camera();
    const Mat4x4f& View() const;
    const Mat4x4f& Projection() const;
    virtual void SetProjection(float fovv, float aspect_ratio, float near, float far) = 0;
    virtual void Translate(float x, float y, float z);
    virtual void Translate(const Vec3f& t);
    virtual void Scale(float sx, float sy, float sz);
    virtual void Scale(const Vec3f& s);
    virtual void Rotate(float euler_x, float euler_y, float euler_z);
    virtual void Rotate(const Vec3f& e);
    virtual void LookAt(const Vec3f& f, bool flip_up = false);
    
};


/***************** PerspectiveCamera *****************/

class PerspectiveCamera : public Camera
{

public:

    PerspectiveCamera();
    PerspectiveCamera(float fovv, float aspect_ratio, float near, float far);
    void SetProjection(float fovv, float aspect_ratio, float near, float far);

};


/***************** OrthographicCamera *****************/

class OrthographicCamera : public Camera
{

public:

    OrthographicCamera();
    OrthographicCamera(float fovv, float aspect_ratio, float near, float far);
    void SetProjection(float fovv, float aspect_ratio, float near, float far);

};


#endif
