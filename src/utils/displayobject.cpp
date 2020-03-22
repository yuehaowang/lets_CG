#include "displayobject.h"


unsigned int DisplayObject::max_object_id = 0;

DisplayObject::DisplayObject()
: id(max_object_id++)
{

}

DisplayObject::~DisplayObject()
{

}

const Mat4x4f& DisplayObject::Transform() const
{
    return transform;
}

unsigned int DisplayObject::Id() const
{
    return id;
}

void DisplayObject::Translate(float x, float y, float z)
{
    Translate(Vec3f(x, y, z));
}

void DisplayObject::Translate(const Vec3f& t)
{
    Mat4x4f m;
    m.Translate(t.x, t.y, t.z);
    transform *= m;

}

void DisplayObject::Scale(float sx, float sy, float sz)
{
    Scale(Vec3f(sx, sy, sz));
}

void DisplayObject::Scale(const Vec3f& s)
{
    Mat4x4f m;
    m.Scale(s.x, s.y, s.z);
    transform *= m;
}

void DisplayObject::Rotate(float euler_x, float euler_y, float euler_z)
{
    Rotate(Vec3f(euler_x, euler_y, euler_z));
}

void DisplayObject::Rotate(const Vec3f& e)
{
    Mat4x4f m;
    m.Rotate(e.x, e.y, e.z);
    transform *= m;
}

Vec3f DisplayObject::Position() const
{
    return transform * Vec3f(0.0f, 0.0f, 0.0f);
}

Vec3f DisplayObject::Forward() const
{
    return (transform * Vec3f(0.0f, 0.0f, 1.0f)) - Position();
}

Vec3f DisplayObject::Up() const
{
    return (transform * Vec3f(0.0f, 1.0f, 0.0f)) - Position();
}

Vec3f DisplayObject::Right() const
{
    return (transform * Vec3f(1.0f, 0.0f, 0.0f)) - Position();
}
