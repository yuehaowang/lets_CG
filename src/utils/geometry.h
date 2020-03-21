#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "gl_header_files.h"
#include "vector"


/***************** Geometry *****************/

class Geometry
{

protected:

    std::vector<GLfloat> vertex_data;
    std::vector<GLfloat> normal_data;
    std::vector<unsigned int> index_data;

public:

    Geometry();
    Geometry(
        std::vector<GLfloat> vert,
        std::vector<GLfloat> norm,
        std::vector<unsigned int> index);
    virtual ~Geometry();
    const std::vector<GLfloat>& VertexData() const;
    const std::vector<GLfloat>& NormalData() const;
    const std::vector<unsigned int>& IndexData() const;

};


/***************** BoxGeometry *****************/

class BoxGeometry : public Geometry
{

protected:

    static const GLfloat VERTICES[108];
    static const GLfloat NORMALS[108];

public:

    BoxGeometry();

};


#endif
