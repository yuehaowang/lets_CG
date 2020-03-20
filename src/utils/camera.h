#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "gl_header_files.h"
#include "mat4x4.h"
#include "displayobject.h"


/***************** Camera *****************/

class Camera : public DisplayObject
{

protected:

    Mat4x4<GLfloat> view;
    Mat4x4<GLfloat> projection;

public:

    Camera();
    virtual ~Camera();
    const Mat4x4<GLfloat>* View() const;
    const Mat4x4<GLfloat>* Projection() const;
    void Translate(GLfloat x, GLfloat y, GLfloat z);
    void Scale(GLfloat sx, GLfloat sy, GLfloat sz);
    void Rotate(GLfloat euler_x, GLfloat euler_y, GLfloat euler_z);
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
