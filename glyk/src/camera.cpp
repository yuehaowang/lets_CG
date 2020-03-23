#include "glyk/camera.h"


/***************** Camera *****************/

std::string Camera::camera_uniform_name = "camera";

Camera::Camera()
{

}

Camera::~Camera()
{

}

const Mat4x4f& Camera::View() const
{
    return view;
}

const Mat4x4f& Camera::Projection() const
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

    Vec3f cam_pos = Position();
    glUniform3f(
        glGetUniformLocation(shader_id, ShaderCameraUniformIdentifier("position").c_str()),
        cam_pos.x, cam_pos.y, cam_pos.z
    );
}

void Camera::Translate(float x, float y, float z)
{
    Translate(Vec3f(x, y, z));
}

void Camera::Translate(const Vec3f& t)
{
    Object3D::Translate(t);

    view.Translate(-t.x, -t.y, -t.z);
}

void Camera::Scale(float sx, float sy, float sz)
{
    Scale(Vec3f(sx, sy, sz));
}

void Camera::Scale(const Vec3f& s)
{
    Object3D::Scale(s);

    view.Scale(1 / s.x, 1 / s.y, 1 / s.z);
}

void Camera::Rotate(float euler_x, float euler_y, float euler_z)
{
    Rotate(Vec3f(euler_x, euler_y, euler_z));
}

void Camera::Rotate(const Vec3f& e)
{
    Object3D::Rotate(e);

    view.Rotate(-e.x, -e.y, -e.z);
}

void Camera::LookAt(const Vec3f& f)
{
    Vec3f front = f;
    front.Normalize();
    Vec3f right = Vec3f(0.0f, 1.0f, 0.0f) * front;
    right.Normalize();
    Vec3f up = front * right;
    up.Normalize();

    Vec3f pos = Position();
    Mat4x4f orientation_mat = Mat4x4f((float[]){
        right.x,  up.x,  front.x, 0,
        right.y,  up.y,  front.y, 0,
        right.z,  up.z,  front.z, 0,
        0,        0,     0,     1
    });
    transform = orientation_mat;
    transform.Translate(pos.x, pos.y, pos.z);
    
    Mat4x4f m;
    m.Eye(pos, front, up);
    view = m;
}


/***************** PerspectiveCamera *****************/

PerspectiveCamera::PerspectiveCamera(float fovv, float aspect_ratio, float near, float far)
{
    SetProjection(fovv, aspect_ratio, near, far);
}

void PerspectiveCamera::SetProjection(float fovv, float aspect_ratio, float near, float far)
{
    projection.Perspective(fovv, aspect_ratio, near, far);
}


/***************** OrthographicCamera *****************/

OrthographicCamera::OrthographicCamera(float fovv, float aspect_ratio, float near, float far)
{
    SetProjection(fovv, aspect_ratio, near, far);
}

void OrthographicCamera::SetProjection(float fovv, float aspect_ratio, float near, float far)
{
    projection.Orthographic(fovv, aspect_ratio, near, far);
}
