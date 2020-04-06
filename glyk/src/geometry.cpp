#include <cmath>
#include "glyk/geometry.h"
#include "glyk/vec3.h"
#include "glyk/mat4x4.h"


/***************** Geometry *****************/

Geometry::Geometry()
: primitives(Triangles)
{

}

Geometry::Geometry(
    const std::vector<float>& vert,
    const std::vector<float>& norm,
    const std::vector<float>& texc,
    const std::vector<unsigned int>& index,
    PrimitivesType p)
: vertex_data(vert)
, normal_data(norm)
, texcoord_data(texc)
, index_data(index)
, primitives(p)
{

}

Geometry::Geometry(
    const std::vector<float>& vert,
    const std::vector<float>& norm,
    const std::vector<float>& texc,
    PrimitivesType p)
: vertex_data(vert)
, normal_data(norm)
, texcoord_data(texc)
, primitives(p)
{

}

Geometry::Geometry(
    const std::vector<float>& vert,
    const std::vector<float>& norm,
    PrimitivesType p)
: vertex_data(vert)
, normal_data(norm)
, primitives(p)
{

}

Geometry::Geometry(const std::vector<float>& vert, PrimitivesType p)
: vertex_data(vert)
, primitives(p)
{

}

Geometry::~Geometry()
{

}

void Geometry::GenerateTBN(float epsilon)
{
    TBN_data.clear();

    if (primitives != Triangles) {
        return;
    }

    unsigned int vert_num = vertex_data.size() / 3;
    for (unsigned int i = 0; i < vert_num; i += 3) {
        unsigned int vi = i * 3;
        Vec3f vert1(vertex_data[vi], vertex_data[vi + 1], vertex_data[vi + 2]);
        Vec3f vert2(vertex_data[vi + 3], vertex_data[vi + 4], vertex_data[vi + 5]);
        Vec3f vert3(vertex_data[vi + 6], vertex_data[vi + 7], vertex_data[vi + 8]);
        unsigned int ti = i * 2;
        Vec3f tex1(texcoord_data[ti], texcoord_data[ti + 1], 0.0f);
        Vec3f tex2(texcoord_data[ti + 2], texcoord_data[ti + 3], 0.0f);
        Vec3f tex3(texcoord_data[ti + 4], texcoord_data[ti + 5], 0.0f);
        
        Vec3f e1 = vert2 - vert1;
        Vec3f e2 = vert3 - vert1;
        Vec3f dt_12 = tex2 - tex1;
        Vec3f dt_13 = tex3 - tex1;

        Vec3f tangent;

        float det = (dt_12.x * dt_13.y - dt_13.x * dt_12.y);

        if (abs(det) < epsilon) {
            tangent.x = tangent.y = tangent.z = 0;
        } else {
            float f = 1.0f / det;
            tangent = Vec3f(
                f * (dt_13.y * e1.x - dt_12.y * e2.x),
                f * (dt_13.y * e1.y - dt_12.y * e2.y),
                f * (dt_13.y * e1.z - dt_12.y * e2.z)
            );
            tangent.Normalize();
        }

        for (unsigned int j = 0; j < 3; j++) {
            TBN_data.push_back(tangent.x);
            TBN_data.push_back(tangent.y);
            TBN_data.push_back(tangent.z);
        }
    }
}

const std::vector<float>& Geometry::VertexData() const
{
    return vertex_data;
}

const std::vector<float>& Geometry::NormalData() const
{
    return normal_data;
}

const std::vector<float>& Geometry::TexCoordData() const
{
    return texcoord_data;
}

const std::vector<float>& Geometry::TBNData() const
{
    return TBN_data;
}

const std::vector<unsigned int>& Geometry::IndexData() const
{
    return index_data;
}

Geometry::PrimitivesType Geometry::Primitives() const
{
    return primitives;
}


/***************** BoxGeometry *****************/

float BoxGeometry::VERTICES[108] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

float BoxGeometry::NORMALS[108] = {
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,

    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,

    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,

    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.01f
};

float BoxGeometry::TEXCOORDS[72] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,

    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,

    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,

    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f
};

BoxGeometry::BoxGeometry(bool gen_TBN)
: Geometry(
    std::vector<float>(VERTICES, VERTICES + sizeof(VERTICES) / sizeof(VERTICES[0])),
    std::vector<float>(NORMALS, NORMALS + sizeof(NORMALS) / sizeof(NORMALS[0])),
    std::vector<float>(TEXCOORDS, TEXCOORDS + sizeof(TEXCOORDS) / sizeof(TEXCOORDS[0])))

{
    if (gen_TBN) {
        GenerateTBN();
    }
}


/***************** SphereGeometry *****************/

std::vector<float> SphereGeometry::VERTICES;
std::vector<float> SphereGeometry::NORMALS;
std::vector<float> SphereGeometry::TEXCOORDS;

SphereGeometry::SphereGeometry(bool gen_TBN)
{
    if (VERTICES.size() == 0) {
        GenerateSphere(VERTICES, NORMALS, TEXCOORDS, 1.0f, 100);
    }

    vertex_data = VERTICES;
    normal_data = NORMALS;
    texcoord_data = TEXCOORDS;
    primitives = Triangles;

    if (gen_TBN) {
        GenerateTBN();
    }
}

void SphereGeometry::GenerateSphere(
    std::vector<float>& vert,
    std::vector<float>& norm,
    std::vector<float>& texc,
    float radius, unsigned int divi_count)
{
    float init_azimuth = 0.0f;
    float init_zenith = -90.0f;

    float max_azimuth = 360.0f;
    float max_zenith = 90.0f;

    float d_azimuth = (max_azimuth - init_azimuth) / divi_count;
    float d_zenith = (max_zenith - init_zenith) / divi_count;

    std::vector<float> vert_, norm_, texc_;
    std::vector<unsigned int> index_;

    for (unsigned int i = 0; i <= divi_count; i++) {
        float zenith = d_zenith * i + init_zenith;

        float y = radius * sinf(zenith * PI / 180);
        float proj_xz = radius * cosf(zenith * PI / 180);

        for (unsigned int j = 0; j < divi_count; j++) {
            float azimuth = d_azimuth * j + init_azimuth;

            float x = proj_xz * sinf(azimuth * PI / 180);
            float z = proj_xz * cosf(azimuth * PI / 180);

            vert_.push_back(x);
            vert_.push_back(y);
            vert_.push_back(z);

            norm_.push_back(x / radius);
            norm_.push_back(y / radius);
            norm_.push_back(z / radius);

            texc_.push_back((float)j / divi_count);
            texc_.push_back((float)i / divi_count);

            if (i != divi_count) {
                index_.push_back(i * divi_count + j);
                index_.push_back((i + 1) * divi_count + j);
                index_.push_back(i * divi_count + (j + 1) % divi_count);
                index_.push_back(i * divi_count + (j + 1) % divi_count); 
                index_.push_back((i + 1) * divi_count + j);
                index_.push_back((i + 1) * divi_count + (j + 1) % divi_count);
            }
        }
    }

    std::vector<unsigned int>::iterator it; 
    for (it = index_.begin(); it != index_.end(); it += 3) {
        int p1_row_index = *it % divi_count;

        for (unsigned int i = 0; i < 3; i++) {
            vert.push_back(vert_[*(it + i) * 3]);
            vert.push_back(vert_[*(it + i) * 3 + 1]);
            vert.push_back(vert_[*(it + i) * 3 + 2]);

            norm.push_back(norm_[*(it + i) * 3]);
            norm.push_back(norm_[*(it + i) * 3 + 1]);
            norm.push_back(norm_[*(it + i) * 3 + 2]);

            int curr_row_index = *(it + i) % divi_count;
            if (p1_row_index - curr_row_index == divi_count - 1) {
                texc.push_back(texc_[*(it + i) * 2] + 1.0f);
            } else if (p1_row_index - curr_row_index == -(divi_count - 1)) {
                texc.push_back(texc_[*(it + i) * 2] - 1.0f);
            } else {
                texc.push_back(texc_[*(it + i) * 2]);
            }
            texc.push_back(texc_[*(it + i) * 2 + 1]);
        }
    }
}


/***************** QuadGeometry *****************/

float QuadGeometry::VERTICES[18] = {
    -0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,

    0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
};

float QuadGeometry::NORMALS[18] = {  
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f
};

float QuadGeometry::TEXCOORDS[12] = {  
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,

    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f
};

QuadGeometry::QuadGeometry(bool gen_TBN)
: Geometry(
    std::vector<float>(VERTICES, VERTICES + sizeof(VERTICES) / sizeof(VERTICES[0])),
    std::vector<float>(NORMALS, NORMALS + sizeof(NORMALS) / sizeof(NORMALS[0])),
    std::vector<float>(TEXCOORDS, TEXCOORDS + sizeof(TEXCOORDS) / sizeof(TEXCOORDS[0])))
{
    if (gen_TBN) {
        GenerateTBN();
    }
}
