#include <fstream>
#include <sstream>
#include <iostream>
#include "glyk/loader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "thirdparty/tiny_obj_loader.h"


Loader::PreprocessFlag operator| (Loader::PreprocessFlag f1, Loader::PreprocessFlag f2)
{
    return static_cast<Loader::PreprocessFlag>(static_cast<int>(f1) | static_cast<int>(f2));
}

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
    Image img;
    img.Load(path);

    return img;
}

std::vector<Geometry> Loader::LoadModel(const std::string& path, PreprocessFlag flag)
{
    printf("-- Loading model %s...\n", path.c_str());

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

                float texcoord_x = attrib.texcoords[2 * idx.texcoord_index + 0];
                float texcoord_y = attrib.texcoords[2 * idx.texcoord_index + 1];
                if (flag & FlipTexcoordU) {
                    texcoord_x = 1.0 - texcoord_x;
                }
                if (flag & FlipTexcoordV) {
                    texcoord_y = 1.0 - texcoord_y;
                }
                texc.push_back(texcoord_x);
                texc.push_back(texcoord_y);
            }
            index_offset += fv;
        }

        Geometry geom(vert, norm, texc);
        if (flag & ComputeTBN) {
            geom.GenerateTBN();
        }
        res.push_back(geom);
    }

    return res;
}
