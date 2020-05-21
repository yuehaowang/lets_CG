#pragma once
#include <string>
#include <vector>
#include <map>
//#include "../3rdLibs/glm/glm/glm.hpp"
#include "Eigen/Dense"

//////////////////////////////////////////////////////////////////////////////////////////////
// Realize loadOBJ.                                                                         //
// You may need the loadOBJ_index function.                                                 //
// Explaination for loadOBJ_index:                                                          //
//      path: path of obj file.                                                             //
//      out_vertices: store pos dictionary                                                  //
//      out_uvs: store uv dictionary                                                        //
//      out_normals: store normal dictionary                                                //
//      out_v_index: index of pos                                                           //
//      out_vt_index: index of uv                                                           //
//      out_vn_index: index of normal                                                       //
//                                                                                          //
//      take out_v_index for example: it should be :                                        //
//      pos1  pos2  pos3                                                                    //
//         0,    4,    6,  // first triangle, begin from 0                                  //
//         3,    5,    8,  // seconde                                                       //
//         ...                                                                              //
//////////////////////////////////////////////////////////////////////////////////////////////
bool loadOBJ(
    const char* path,
    std::vector < Eigen::Vector3f >& out_vertices,
    std::vector < Eigen::Vector2f >& out_uvs,
    std::vector < Eigen::Vector3f >& out_normals
);



bool loadOBJ(const char* path, std::vector<Eigen::Vector3f>& out_vertices, std::vector<Eigen::Vector2f>& out_uvs, std::vector<Eigen::Vector3f >& out_normals)
{
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< Eigen::Vector3f > temp_vertices;
    std::vector< Eigen::Vector2f > temp_uvs;
    std::vector< Eigen::Vector3f > temp_normals;
    printf("%s\n", path);
    FILE* file;
    file = fopen(path, "r");
    if (file == NULL) {
        printf("Cannot open the file !\n");
        return false;
    }

    while (1) {

        char lineHeader[64];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader,64);
        //printf("%s\n", lineHeader);
        if (res == EOF)
            break;
        if (strcmp(lineHeader, "v") == 0) {
            Eigen::Vector3f vertex;
            fscanf(file, "%f %f %f\n", &vertex(0), &vertex(1), &vertex(2));
            temp_vertices.push_back(vertex);
            //printf("v\n");
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            Eigen::Vector2f uv;
            fscanf(file, "%f %f\n", &uv(0), &uv(1));
            temp_uvs.push_back(uv);
            //printf("vt\n");
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            Eigen::Vector3f normal;
            fscanf(file, "%f %f %f\n", &normal(0), &normal(1), &normal(2));
            temp_normals.push_back(normal);
            //printf("vn\n");
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            //printf("f\n");
        }
    }
    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        Eigen::Vector3f vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < uvIndices.size(); i++) {
        unsigned int uvIndex = uvIndices[i];
        Eigen::Vector2f uv = temp_uvs[uvIndex - 1];
        out_uvs.push_back(uv);
    }
    for (unsigned int i = 0; i < normalIndices.size(); i++) {
        unsigned int normalIndex = normalIndices[i];
        Eigen::Vector3f normal = temp_normals[normalIndex - 1];
        out_normals.push_back(normal);
    }
    return true;
}

bool loadOBJ_index(
    const char* path,
    std::vector < Eigen::Vector3f >& out_vertices,
    std::vector < Eigen::Vector2f >& out_uvs,
    std::vector < Eigen::Vector3f >& out_normals,
    std::vector < int >& out_v_index,
    std::vector < int >& out_vt_index,
    std::vector < int >& out_vn_index
);



bool loadOBJ_index(const char* path, 
    std::vector<Eigen::Vector3f>& out_vertices, std::vector<Eigen::Vector2f>& out_uvs, std::vector<Eigen::Vector3f >& out_normals, 
    std::vector < int >& out_v_index, std::vector < int >& out_vt_index, std::vector < int >& out_vn_index)
{
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< Eigen::Vector3f > temp_vertices;
    std::vector< Eigen::Vector2f > temp_uvs;
    std::vector< Eigen::Vector3f > temp_normals;
    printf("%s\n", path);
    FILE* file;
    file = fopen(path, "r");
    if (file == NULL) {
        printf("Cannot open the file !\n");
        return false;
    }

    while (1) {

        char lineHeader[64];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader, 64);
        //printf("%s\n", lineHeader);
        if (res == EOF)
            break;
        if (strcmp(lineHeader, "v") == 0) {
            Eigen::Vector3f vertex;
            fscanf(file, "%f %f %f\n", &vertex(0), &vertex(1), &vertex(2));
            out_vertices.push_back(vertex);
            //printf("v\n");
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            Eigen::Vector2f uv;
            fscanf(file, "%f %f\n", &uv(0), &uv(1));
            out_uvs.push_back(uv);
            //printf("vt\n");
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            Eigen::Vector3f normal;
            fscanf(file, "%f %f %f\n", &normal(0), &normal(1), &normal(2));
            out_normals.push_back(normal);
            //printf("vn\n");
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            for (int j = 0; j < 3; j++)
            {
                out_v_index.push_back(vertexIndex[j]-1);
                out_vt_index.push_back(uvIndex[j]-1);
                out_vn_index.push_back(normalIndex[j]-1);
            }
            
            //printf("f\n");
        }
    }
    
    return true;
}




bool loadOBJ_without_norm(
    const char* path,
    std::vector < Eigen::Vector3f >& out_vertices,
    std::vector < Eigen::Vector2f >& out_uvs,
    std::vector< Eigen::Vector3f >& out_normals
    );

//bool loadOBJ_without_norm(const char* path, std::vector<Eigen::Vector3f>& out_vertices, std::vector<Eigen::Vector2f>& out_uvs, std::vector< Eigen::Vector3f >& out_normals)
//{
//    std::vector< unsigned int > vertexIndices, uvIndices;
//    std::vector< Eigen::Vector3f > temp_vertices;
//    std::vector< Eigen::Vector2f > temp_uvs;
//
//    printf("%s\n", path);
//    FILE* file;
//    fopen(&file, path, "r");
//    if (file == NULL) {
//        printf("Cannot open the file !\n");
//        return false;
//    }
//
//    while (1) {
//
//        char lineHeader[8192];
//        // read the first word of the line
//        int res = fscanf(file, "%s", &lineHeader,8192);
//        if (res == EOF)
//            break;
//        if (strcmp(lineHeader, "v") == 0) {
//            Eigen::Vector3f vertex;
//            fscanf(file, "%f %f %f\n", &vertex(0), &vertex(1), &vertex(2));
//            temp_vertices.push_back(vertex);
//        }
//        else if (strcmp(lineHeader, "vt") == 0) {
//            Eigen::Vector2f uv;
//            fscanf(file, "%f %f\n", &uv(0), &uv(1));
//            temp_uvs.push_back(uv);
//        }
//        else if (strcmp(lineHeader, "f") == 0) {
//            std::string vertex1, vertex2, vertex3;
//            unsigned int vertexIndex[3], uvIndex[3];
//            int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
//            if (matches != 6) {
//                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
//                printf("acture match:%d\n", matches);
//                return false;
//            }
//            vertexIndices.push_back(vertexIndex[0]);
//            vertexIndices.push_back(vertexIndex[1]);
//            vertexIndices.push_back(vertexIndex[2]);
//            uvIndices.push_back(uvIndex[0]);
//            uvIndices.push_back(uvIndex[1]);
//            uvIndices.push_back(uvIndex[2]);
//
//        }
//    }
//    // For each vertex of each triangle
//    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
//        unsigned int vertexIndex = vertexIndices[i];
//        Eigen::Vector3f vertex = temp_vertices[vertexIndex - 1];
//        out_vertices.push_back(vertex);
//    }
//    for (unsigned int i = 0; i < uvIndices.size(); i++) {
//        unsigned int uvIndex = uvIndices[i];
//        Eigen::Vector2f uv = temp_uvs[uvIndex - 1];
//        out_uvs.push_back(uv);
//    }
//    // record nearby face for each vertex
//    /*std::vector<index_list> nearby_face_ind;
//    for (int i = 0;i < temp_vertices.size();i++)
//    {
//        index_list temp_id_index_list = index_list();
//        nearby_face_ind.push_back(temp_id_index_list);
//    }
//    for (int i = 0;i < vertexIndices.size()/3;i++)
//    {
//        for (int j = 0;j < 3;j++)
//        {
//            int temp_index = vertexIndices[int(i * 3 + j)];
//            nearby_face_ind[temp_index].push(i);
//        }
//    }*/
//    std::vector<index_list*> nearby_face_ind;
//    for (int i = 0;i < temp_vertices.size();i++)
//    {
//        index_list* temp_id_index_list = new index_list();
//        nearby_face_ind.push_back(temp_id_index_list);
//    }
//
//    for (int i = 0;i < vertexIndices.size() / 3;i++)
//    {
//        for (int j = 0;j < 3;j++)
//        {
//            int temp_index = vertexIndices[int(i * 3 + j)]-1;
//            nearby_face_ind[temp_index]->push(i);
//        } 
//    }
//
//
//    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    //calculate normal here                                                                                     //
//    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Eigen::Vector3f temp_vec_1;
//    Eigen::Vector3f temp_vec_2;
//    Eigen::Vector3f fn_vec;
//    std::vector<Eigen::Vector3f> obj_face_norm;    
//    std::vector<Eigen::Vector3f> obj_vertex_norm;          //This is a normal diction
//
//    for (int i = 0;i < out_vertices.size() / 3;i++)
//    {
//        temp_vec_1 = out_vertices[int(3 * i + 1)] - out_vertices[int(3 * i + 0)];
//        temp_vec_2 = out_vertices[int(3 * i + 2)] - out_vertices[int(3 * i + 1)];
//        obj_face_norm.push_back(glm::normalize(glm::cross(temp_vec_1, temp_vec_2)));
//    }
//
//    for (int i = 0;i < nearby_face_ind.size();i++)
//    {
//        Eigen::Vector3f temp_sum_vec(0, 0, 0);
//        std::vector<int> temp_id_vec = nearby_face_ind[i]->get_id_vec();
//        for (int j = 0;j < temp_id_vec.size();j++)
//        {
//            temp_sum_vec += temp_id_vec[j];
//        }
//        obj_vertex_norm.push_back(temp_sum_vec);
//    }
//
//    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
//        unsigned int vertexIndex = vertexIndices[i];
//        Eigen::Vector3f vertex = obj_vertex_norm[vertexIndex - 1];
//        out_normals.push_back(vertex);
//    }
//    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    //calculate normal end                                                                                      //
//    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    return true;
//}