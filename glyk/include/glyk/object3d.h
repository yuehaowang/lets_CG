#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include <vector>
#include "glyk/gl_header_files.h"
#include "glyk/mat4x4.h"
#include "glyk/vec3.h"


class Object3D
{

protected:

    unsigned int id;
    static unsigned int max_object_id;
    Mat4x4f transform;

public:

    Object3D();
    virtual ~Object3D();
    unsigned int Id() const;
    const Mat4x4f& Transform() const;
    virtual void SetTransform(const Mat4x4f& mat);
    virtual void Translate(float x, float y, float z);
    virtual void Translate(const Vec3f& t);
    virtual void Scale(float sx, float sy, float sz);
    virtual void Scale(const Vec3f& s);
    virtual void Rotate(float euler_x, float euler_y, float euler_z);
    virtual void Rotate(const Vec3f& e);
    virtual Vec3f Position() const;
    virtual Vec3f Forward() const;
    virtual Vec3f Up() const;
    virtual Vec3f Right() const;

};


#endif
