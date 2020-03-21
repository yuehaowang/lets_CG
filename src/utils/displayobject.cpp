#include "displayobject.h"


unsigned int DisplayObject::max_object_id = 0;

DisplayObject::DisplayObject()
: translation(0, 0, 0)
, rotation(0, 0, 0)
, scaling(1, 1, 1)
, id(max_object_id++)
{

}

DisplayObject::~DisplayObject()
{

}

Mat4x4<GLfloat> DisplayObject::Transform() const
{
    Mat4x4<GLfloat> transform;
    transform.Scale(scaling.x, scaling.y, scaling.z);
    transform.Rotate(rotation.x, rotation.y, rotation.z);
    transform.Translate(translation.x, translation.y, translation.z);

    return transform;
}

unsigned int DisplayObject::Id() const
{
    return id;
}

void DisplayObject::Translate(GLfloat x, GLfloat y, GLfloat z)
{
    Translate(Vec3<GLfloat>(x, y, z));
}

void DisplayObject::Translate(const Vec3<GLfloat>& t)
{
    translation += t;
}

void DisplayObject::Scale(GLfloat sx, GLfloat sy, GLfloat sz)
{
    Scale(Vec3<GLfloat>(sx, sy, sz));
}

void DisplayObject::Scale(const Vec3<GLfloat>& s)
{
    scaling.x *= s.x;
    scaling.y *= s.y;
    scaling.z *= s.z;
}

void DisplayObject::Rotate(GLfloat euler_x, GLfloat euler_y, GLfloat euler_z)
{
    Rotate(Vec3<GLfloat>(euler_x, euler_y, euler_z));
}

void DisplayObject::Rotate(const Vec3<GLfloat>& e)
{
    rotation += e;
}

void DisplayObject::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
    SetPosition(Vec3<GLfloat>(x, y, z));
}

void DisplayObject::SetPosition(GLfloat x, GLfloat y)
{
    SetPosition(Vec3<GLfloat>(x, y, translation.z));
}

void DisplayObject::SetPosition(GLfloat x)
{
    SetPosition(Vec3<GLfloat>(x, translation.y, translation.z));
}

void DisplayObject::SetPosition(const Vec3<GLfloat>& t)
{
    translation = t;
}

void DisplayObject::SetScaling(GLfloat sx, GLfloat sy, GLfloat sz)
{
    SetScaling(Vec3<GLfloat>(sx, sy, sz));
}

void DisplayObject::SetScaling(GLfloat sx, GLfloat sy)
{
    SetScaling(Vec3<GLfloat>(sx, sy, scaling.z));
}

void DisplayObject::SetScaling(GLfloat sx)
{
    SetScaling(Vec3<GLfloat>(sx, scaling.y, scaling.z));
}

void DisplayObject::SetScaling(const Vec3<GLfloat>& s)
{
    scaling = s;
}

void DisplayObject::SetRotation(GLfloat euler_x, GLfloat euler_y, GLfloat euler_z)
{
    SetRotation(Vec3<GLfloat>(euler_x, euler_y, euler_z));
}

void DisplayObject::SetRotation(GLfloat euler_x, GLfloat euler_y)
{
    SetRotation(Vec3<GLfloat>(euler_x, euler_y, rotation.z));
}

void DisplayObject::SetRotation(GLfloat euler_x)
{
    SetRotation(Vec3<GLfloat>(euler_x, rotation.y, rotation.z));
}

void DisplayObject::SetRotation(const Vec3<GLfloat>& e)
{
    rotation = e;
}

Vec3<GLfloat> DisplayObject::Position() const
{
    return translation;
}

Vec3<GLfloat> DisplayObject::Rotation() const
{
    return rotation;
}

Vec3<GLfloat> DisplayObject::Scaling() const
{
    return scaling;
}

Vec3<GLfloat> DisplayObject::Forward() const
{
    Mat4x4<GLfloat> mat;
    mat.Rotate(rotation.x, rotation.y, rotation.z);
    return mat * Vec3<GLfloat>(0.0f, 0.0f, 1.0f);
}

Vec3<GLfloat> DisplayObject::Up() const
{
    Mat4x4<GLfloat> mat;
    mat.Rotate(rotation.x, rotation.y, rotation.z);
    return mat * Vec3<GLfloat>(0.0f, 1.0f, 0.0f);
}

Vec3<GLfloat> DisplayObject::Right() const
{
    Mat4x4<GLfloat> mat;
    mat.Rotate(rotation.x, rotation.y, rotation.z);
    return mat * Vec3<GLfloat>(1.0f, 0.0f, 0.0f);
}
