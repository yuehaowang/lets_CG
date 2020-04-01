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

Geometry::Geometry(
    const std::vector<float>& vert,
    const std::vector<float>& norm,
    const std::vector<float>& texc)
: vertex_data(vert)
, normal_data(norm)
, texcoord_data(texc)
{

}

Geometry::Geometry(
    const std::vector<float>& vert,
    const std::vector<float>& norm)
: vertex_data(vert)
, normal_data(norm)
{

}

Geometry::Geometry(const std::vector<float>& vert)
: vertex_data(vert)
{

}

Geometry::~Geometry()
{

}

void Geometry::GenerateTBN()
{
    TBN_data.clear();

    unsigned int vert_num = vertex_data.size() / 3;
    for (int i = 0; i < vert_num; i += 3) {
        unsigned int vi = i * 3;
        Vec3f pos1(vertex_data[vi], vertex_data[vi + 1], vertex_data[vi + 2]);
        Vec3f pos2(vertex_data[vi + 3], vertex_data[vi + 4], vertex_data[vi + 5]);
        Vec3f pos3(vertex_data[vi + 6], vertex_data[vi + 7], vertex_data[vi + 8]);
        unsigned int ti = i * 2;
        Vec3f uv1(texcoord_data[ti], texcoord_data[ti + 1], 0.0f);
        Vec3f uv2(texcoord_data[ti + 2], texcoord_data[ti + 3], 0.0f);
        Vec3f uv3(texcoord_data[ti + 4], texcoord_data[ti + 5], 0.0f);
        
        Vec3f edge1 = pos2 - pos1;
        Vec3f edge2 = pos3 - pos1;
        Vec3f deltaUV1 = uv2 - uv1;
        Vec3f deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        Vec3f tangent(
            f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        );
        tangent.Normalize();

        for (int j = 0; j < 3; j++) {
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
