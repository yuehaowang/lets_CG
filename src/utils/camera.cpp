#include "camera.h"
#include "window.h"


/***************** Camera *****************/

Camera::Camera()
{

}

Camera::~Camera()
{

}

const Mat4x4<GLfloat>* Camera::View() const
{
    return &view;
}

const Mat4x4<GLfloat>* Camera::Projection() const
{
    return &projection;
}

void Camera::Translate(GLfloat x, GLfloat y, GLfloat z)
{
    DisplayObject::Translate(x, y, z);

    Mat4x4<GLfloat> m;
    m.Translate(-x, -y, -z);
    view = m * view;
}

void Camera::Scale(GLfloat sx, GLfloat sy, GLfloat sz)
{
    DisplayObject::Scale(sx, sy, sz);

    Mat4x4<GLfloat> m;
    m.Scale(1 / sx, 1 / sy, 1 / sz);
    view = m * view;
}

void Camera::Rotate(GLfloat euler_x, GLfloat euler_y, GLfloat euler_z)
{
    DisplayObject::Rotate(euler_x, euler_y, euler_z);

    Mat4x4<GLfloat> m;
    m.Rotate(-euler_x, -euler_y, -euler_z);
    view = m * view;
}


/***************** PerspectiveCamera *****************/

PerspectiveCamera::PerspectiveCamera(GLfloat fovv, GLfloat aspect_ratio, GLfloat near, GLfloat far)
{
    SetProjection(fovv, aspect_ratio, near, far);
}

void PerspectiveCamera::SetProjection(GLfloat fovv, GLfloat aspect_ratio, GLfloat near, GLfloat far)
{
    projection.Perspective(fovv, aspect_ratio, near, far);
}


/***************** OrthographicCamera *****************/

OrthographicCamera::OrthographicCamera(GLfloat fovv, GLfloat aspect_ratio, GLfloat near, GLfloat far)
{
    SetProjection(fovv, aspect_ratio, near, far);
}

void OrthographicCamera::SetProjection(GLfloat fovv, GLfloat aspect_ratio, GLfloat near, GLfloat far)
{
    projection.Orthographic(fovv, aspect_ratio, near, far);
}
