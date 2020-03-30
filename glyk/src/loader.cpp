#include <fstream>
#include <sstream>
#include <iostream>
#include "glyk/loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "thirdparty/tiny_obj_loader.h"


/***************** Image *****************/

Image::Image()
: data(NULL)
, width(0)
, height(0)
, channels(0)
{

}

Image::Image(unsigned char* d, int w, int h, int ch)
: data(d)
, width(w)
, height(h)
, channels(ch)
{

}

bool Image::IsNull()
{
    return data == NULL;
}

void Image::Free()
{
    stbi_image_free(data);
}


/***************** Loader *****************/

std::string Loader::LoadPlainText(const std::string& file_path)
{
    std::string txt;
    std::ifstream stream(file_path, std::ios::in);

    if (stream.is_open()) {
        std::stringstream sstr;

        sstr << stream.rdbuf();
        txt = sstr.str();

        stream.close();
    } else {
        printf("Failed to open %s\n", file_path.c_str());
    }

    return txt;
}

Image Loader::LoadImage(const std::string& path)
{
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    return Image(data, width, height, channels);
    return Image();
}

std::vector<Geometry> Loader::LoadModel(const std::string& path)
{
    std::vector<Geometry> res;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
    if (!success) {
        printf("Failed to open %s: %s\n", path.c_str(), err.c_str());
    }
    if (!warn.empty()) {
        printf("Warning at loading %s: %s\n", path.c_str(), warn.c_str());
    }

    for (size_t i = 0; i < shapes.size(); i++) {
        tinyobj::shape_t* sh = &shapes[i];
        tinyobj::mesh_t* mesh = &(sh->mesh);

        std::vector<float> vert;
        std::vector<float> norm;
        std::vector<float> texc;

        size_t index_offset = 0;
        for (size_t f = 0; f < mesh->num_face_vertices.size(); f++) {
            int fv = mesh->num_face_vertices[f];
            if (fv > 3) {
                printf("Warning at loading %s: faces with more than 3 vertices are not supported.", path.c_str());
                break;
            }
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = mesh->indices[index_offset + v];

                vert.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                vert.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                vert.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

                norm.push_back(attrib.normals[3 * idx.normal_index + 0]);
                norm.push_back(attrib.normals[3 * idx.normal_index + 1]);
                norm.push_back(attrib.normals[3 * idx.normal_index + 2]);

                texc.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                texc.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
            }
            index_offset += fv;
        }

        Geometry geom(vert, norm, texc);
        res.push_back(geom);
    }

    return res;
}
