#ifndef _DISPLAYOBJECT_H_
#define _DISPLAYOBJECT_H_

#include "gl_header_files.h"
#include "mat4x4.h"
#include "vec3.h"


class DisplayObject
{

protected:

    Mat4x4<GLfloat> transform;
    unsigned int id;
    static unsigned int max_object_id;

public:

    DisplayObject();
    virtual ~DisplayObject();
    unsigned int Id() const;
    const Mat4x4<GLfloat>* Transform() const;
    void Translate(GLfloat x, GLfloat y, GLfloat z);
    void Scale(GLfloat sx, GLfloat sy, GLfloat sz);
    void Rotate(GLfloat euler_x, GLfloat euler_y, GLfloat euler_z);
    Vec3<GLfloat> Position() const;

};


#endif
