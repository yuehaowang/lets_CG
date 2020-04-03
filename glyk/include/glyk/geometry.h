#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "glyk/gl_header_files.h"
#include "glyk/vec3.h"
#include <vector>


/***************** Geometry *****************/

class Geometry
{

protected:

    std::vector<float> vertex_data;
    std::vector<float> normal_data;
    std::vector<float> texcoord_data;
    std::vector<float> TBN_data;
    std::vector<unsigned int> index_data;

public:

    Geometry();
    Geometry(
        const std::vector<float>& vert,
        const std::vector<float>& norm,
        const std::vector<float>& texc,
        const std::vector<unsigned int>& index);
    Geometry(
        const std::vector<float>& vert,
        const std::vector<float>& norm,
        const std::vector<float>& texc);
    Geometry(
        const std::vector<float>& vert,
        const std::vector<float>& norm);
    Geometry(const std::vector<float>& vert);
    virtual ~Geometry();
    void GenerateTBN(float epsilon = 0.00001);
    const std::vector<float>& VertexData() const;
    const std::vector<float>& NormalData() const;
    const std::vector<float>& TexCoordData() const;
    const std::vector<float>& TBNData() const;
    const std::vector<unsigned int>& IndexData() const;

};


/***************** BoxGeometry *****************/

class BoxGeometry : public Geometry
{

protected:

    static float VERTICES[108];
    static float NORMALS[108];
    static float TEXCOORDS[72];

public:

    BoxGeometry(bool gen_TBN = false);

};


/***************** SphereGeometry *****************/

class SphereGeometry : public Geometry
{

protected:

    static std::vector<float> VERTICES;
    static std::vector<float> NORMALS;
    static std::vector<float> TEXCOORDS;
    static std::vector<unsigned int> INDICES;

public:

    SphereGeometry(bool gen_TBN = false);
    static void GenerateSphere(
        std::vector<float>& vert,
        std::vector<float>& norm,
        std::vector<float>& texc,
        float radius, unsigned int divi_count);

};


#endif
