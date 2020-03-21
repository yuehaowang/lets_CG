#include "camera.h"
#include "window.h"


/***************** Camera *****************/

std::string Camera::camera_uniform_name = "camera";

Camera::Camera()
{

}

Camera::~Camera()
{

}

Mat4x4<GLfloat> Camera::View() const
{
    Mat4x4<GLfloat> view;
    view.Translate(-translation.x, -translation.y, -translation.z);
    view.Rotate(-rotation.x, -rotation.y, -rotation.z);
    view.Scale(1 / scaling.x, 1 / scaling.y, 1 / scaling.z);

    return view;
}

Mat4x4<GLfloat> Camera::Projection() const
{
    return projection;
}

std::string Camera::ShaderCameraUniformIdentifier(const std::string& member_name)
{
    return camera_uniform_name + "." + member_name;
}

void Camera::PipeUniformData(GLuint shader_id)
{
    glUniformMatrix4fv(
        glGetUniformLocation(shader_id, ShaderCameraUniformIdentifier("view").c_str()), 1,
        GL_TRUE, View().Ptr()
    );
    glUniformMatrix4fv(
        glGetUniformLocation(shader_id, ShaderCameraUniformIdentifier("projection").c_str()), 1,
        GL_TRUE, projection.Ptr()
    );

    Vec3<GLfloat> cam_pos = Position();
    glUniform3f(
        glGetUniformLocation(shader_id, ShaderCameraUniformIdentifier("position").c_str()),
        cam_pos.x, cam_pos.y, cam_pos.z
    );
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
