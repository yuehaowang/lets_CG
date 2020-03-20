#include "displayobject.h"


unsigned int DisplayObject::max_object_id = 0;

DisplayObject::DisplayObject()
{
    id = max_object_id++;
}

DisplayObject::~DisplayObject()
{

}

const Mat4x4<GLfloat>* DisplayObject::Transform() const
{
    return &transform;
}

unsigned int DisplayObject::Id() const
{
    return id;
}

void DisplayObject::Translate(GLfloat x, GLfloat y, GLfloat z)
{
    transform.Translate(x, y, z);
}

void DisplayObject::Scale(GLfloat sx, GLfloat sy, GLfloat sz)
{
    transform.Scale(sx, sy, sz);
}

void DisplayObject::Rotate(GLfloat euler_x, GLfloat euler_y, GLfloat euler_z)
{
    transform.Rotate(euler_x, euler_y, euler_z);
}

Vec3<GLfloat> DisplayObject::Position() const
{
    return transform * Vec3<GLfloat>(0.0f, 0.0f, 0.0f);
}