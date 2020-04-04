#include "glyk/object3d.h"


unsigned int Object3D::max_object_id = 0;

Object3D::Object3D()
: id(max_object_id++)
{

}

Object3D::~Object3D()
{

}

const Mat4x4f& Object3D::Transform() const
{
    return transform;
}

void Object3D::SetTransform(const Mat4x4f& mat)
{
    transform = mat;
}

unsigned int Object3D::Id() const
{
    return id;
}

void Object3D::Translate(float x, float y, float z)
{
    Translate(Vec3f(x, y, z));
}

void Object3D::Translate(const Vec3f& t)
{
    Mat4x4f m;
    m.Translate(t.x, t.y, t.z);
    transform *= m;
}

void Object3D::Scale(float sx, float sy, float sz)
{
    Scale(Vec3f(sx, sy, sz));
}

void Object3D::Scale(const Vec3f& s)
{
    Mat4x4f m;
    m.Scale(s.x, s.y, s.z);
    transform *= m;
}

void Object3D::Rotate(float euler_x, float euler_y, float euler_z)
{
    Rotate(Vec3f(euler_x, euler_y, euler_z));
}

void Object3D::Rotate(const Vec3f& e)
{
    Mat4x4f m;
    m.Rotate(e.x, e.y, e.z);
    transform *= m;
}

Vec3f Object3D::Position() const
{
    return transform * Vec3f(0.0f, 0.0f, 0.0f);
}

Vec3f Object3D::Forward() const
{
    return (transform * Vec3f(0.0f, 0.0f, 1.0f)) - Position();
}

Vec3f Object3D::Up() const
{
    return (transform * Vec3f(0.0f, 1.0f, 0.0f)) - Position();
}

Vec3f Object3D::Right() const
{
    return (transform * Vec3f(1.0f, 0.0f, 0.0f)) - Position();
}
