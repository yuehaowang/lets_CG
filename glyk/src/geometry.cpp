#include <cmath>
#include "glyk/geometry.h"
#include "glyk/mat4x4.h"


/***************** Geometry *****************/

Geometry::Geometry()
{

}

Geometry::Geometry(
    const std::vector<float>& vert,
    const std::vector<float>& norm,
    const std::vector<float>& texc,
    const std::vector<unsigned int>& index)
: vertex_data(vert)
, normal_data(norm)
, texcoord_data(texc)
, index_data(index)
{

}

Geometry::~Geometry()
{

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

const std::vector<unsigned int>& Geometry::IndexData() const
{
    return index_data;
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

BoxGeometry::BoxGeometry()
{
    vertex_data = std::vector<float>(VERTICES, VERTICES + sizeof(VERTICES) / sizeof(VERTICES[0]));
    normal_data = std::vector<float>(NORMALS, NORMALS + sizeof(NORMALS) / sizeof(NORMALS[0]));
}


/***************** SphereGeometry *****************/

std::vector<float> SphereGeometry::VERTICES;
std::vector<float> SphereGeometry::NORMALS;
std::vector<unsigned int> SphereGeometry::INDICES;


SphereGeometry::SphereGeometry()
{
    if (VERTICES.size() == 0) {
        GenerateSphere(VERTICES, NORMALS, INDICES, 1.0f, 50);
    }

    vertex_data = VERTICES;
    normal_data = NORMALS;
    index_data = INDICES;
}

void SphereGeometry::GenerateSphere(
    std::vector<float>& vert,
    std::vector<float>& norm,
    std::vector<unsigned int>& index,
    float radius, unsigned int divi_count)
{
    float init_azimuth = 0.0f;
    float init_zenith = -90.0f;

    float max_azimuth = 360.0f;
    float max_zenith = 90.0f;

    float d_azimuth = (max_azimuth - init_azimuth) / divi_count;
    float d_zenith = (max_zenith - init_zenith) / divi_count;

    for (unsigned int i = 0; i < divi_count; i++) {
        float zenith = d_zenith * i + init_zenith;

        float y = radius * sinf(zenith * PI / 180);
        float proj_xz = radius * cosf(zenith * PI / 180);

        for (unsigned int j = 0; j < divi_count; j++) {
            float azimuth = d_azimuth * j + init_azimuth;

            float x = proj_xz * sinf(azimuth * PI / 180);
            float z = proj_xz * cosf(azimuth * PI / 180);

            vert.push_back(x);
            vert.push_back(y);
            vert.push_back(z);

            norm.push_back(x / radius);
            norm.push_back(y / radius);
            norm.push_back(z / radius);

            index.push_back(i * divi_count + j);
            index.push_back((i + 1) * divi_count + j);
            index.push_back(i * divi_count + (j + 1) % divi_count);
            index.push_back(i * divi_count + (j + 1) % divi_count); 
            index.push_back((i + 1) * divi_count + j);
            index.push_back((i + 1) * divi_count + (j + 1) % divi_count);
        }
    }
}
