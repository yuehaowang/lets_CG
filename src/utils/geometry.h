#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "gl_header_files.h"
#include <vector>


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

    static GLfloat VERTICES[108];
    static GLfloat NORMALS[108];

public:

    BoxGeometry();

};


/***************** SphereGeometry *****************/

class SphereGeometry : public Geometry
{

protected:

    static std::vector<GLfloat> VERTICES;
    static std::vector<GLfloat> NORMALS;
    static std::vector<unsigned int> INDICES;

    void GenerateSphere(
        std::vector<GLfloat>& vert,
        std::vector<GLfloat>& norm,
        std::vector<unsigned int>& index,
        float radius, unsigned int divi_count);

public:

    SphereGeometry();

};


#endif
