#ifndef _DISPLAYOBJECT_H_
#define _DISPLAYOBJECT_H_

#include "gl_header_files.h"
#include "mat4x4.h"
#include "vec3.h"


class DisplayObject
{

protected:

    unsigned int id;
    static unsigned int max_object_id;
    Vec3<GLfloat> translation;
    Vec3<GLfloat> rotation;
    Vec3<GLfloat> scaling;

public:

    DisplayObject();
    virtual ~DisplayObject();
    unsigned int Id() const;
    Mat4x4<GLfloat> Transform() const;
    virtual void Translate(GLfloat x, GLfloat y, GLfloat z);
    virtual void Translate(const Vec3<GLfloat>& t);
    virtual void Scale(GLfloat sx, GLfloat sy, GLfloat sz);
    virtual void Scale(const Vec3<GLfloat>& s);
    virtual void Rotate(GLfloat euler_x, GLfloat euler_y, GLfloat euler_z);
    virtual void Rotate(const Vec3<GLfloat>& e);
    virtual void SetPosition(GLfloat x, GLfloat y, GLfloat z);
    virtual void SetPosition(GLfloat x, GLfloat y);
    virtual void SetPosition(GLfloat x);
    virtual void SetPosition(const Vec3<GLfloat>& t);
    virtual void SetScaling(GLfloat sx, GLfloat sy, GLfloat sz);
    virtual void SetScaling(GLfloat sx, GLfloat sy);
    virtual void SetScaling(GLfloat sx);
    virtual void SetScaling(const Vec3<GLfloat>& s);
    virtual void SetRotation(GLfloat euler_x, GLfloat euler_y, GLfloat euler_z);
    virtual void SetRotation(GLfloat euler_x, GLfloat euler_y);
    virtual void SetRotation(GLfloat euler_x);
    virtual void SetRotation(const Vec3<GLfloat>& e);
    virtual Vec3<GLfloat> Position() const;
    virtual Vec3<GLfloat> Rotation() const;
    virtual Vec3<GLfloat> Scaling() const;
    virtual Vec3<GLfloat> Forward() const;
    virtual Vec3<GLfloat> Up() const;
    virtual Vec3<GLfloat> Right() const;

};


#endif
