#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "gl_header_files.h"
#include "mat4x4.h"
#include "displayobject.h"


/***************** Camera *****************/

class Scene;

class Camera : public DisplayObject
{

protected:

    Mat4x4<GLfloat> projection;

    virtual void PipeUniformData(GLuint shader_id);
    virtual std::string ShaderCameraUniformIdentifier(const std::string& member_name);

    friend class Scene;

public:

    static std::string camera_uniform_name;

    Camera();
    virtual ~Camera();
    Mat4x4<GLfloat> View() const;
    Mat4x4<GLfloat> Projection() const;
    virtual void SetProjection(GLfloat fovv, GLfloat aspect_ratio, GLfloat near, GLfloat far) = 0;

};


/***************** PerspectiveCamera *****************/

class PerspectiveCamera : public Camera
{

public:

    PerspectiveCamera();
    PerspectiveCamera(GLfloat fovv, GLfloat aspect_ratio, GLfloat near, GLfloat far);
    void SetProjection(GLfloat fovv, GLfloat aspect_ratio, GLfloat near, GLfloat far);

};


/***************** OrthographicCamera *****************/

class OrthographicCamera : public Camera
{

public:

    OrthographicCamera();
    OrthographicCamera(GLfloat fovv, GLfloat aspect_ratio, GLfloat near, GLfloat far);
    void SetProjection(GLfloat fovv, GLfloat aspect_ratio, GLfloat near, GLfloat far);

};


#endif
