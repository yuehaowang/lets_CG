#ifndef _MESH_H_
#define _MESH_H_

#include <string>
#include <vector>
#include "gl_header_files.h"
#include "mat4x4.h"
#include "vec3.h"
#include "shader.h"
#include "displayobject.h"
#include "material.h"
#include "geometry.h"


class Scene;


class Mesh : public DisplayObject
{

protected:

    enum DrawMode {
        DRAW_ARRAYS = 10,
        DRAW_ELEMENTS
    };

    Shader shader;
    DrawMode draw_mode;
    unsigned int draw_count;
    GLuint VAO;
    GLuint vertex_buffer;
    GLuint normal_buffer;
    GLuint EBO; 
    const Material* material;

    void Initialize(
        const Material* mat,
        const std::vector<float>& vertex_data,
        const std::vector<float>& normal_data,
        const std::vector<unsigned int>& index_data
    );
    void Render();
    void PrepareMaterial(GLuint shader_id);
    void PipeUniformData(GLuint shader_id);
    std::string ShaderMeshUniformIdentifier(const std::string& member_name);

    friend class Scene; 

public:

    static std::string mesh_uniform_name;

    Mesh(
        const Material* mat,
        const std::vector<float>& vertex_data,
        const std::vector<float>& normal_data,
        const std::vector<unsigned int>& index_data);
    Mesh(
        const Material* mat,
        const std::vector<float>& vertex_data,
        const std::vector<float>& normal_data);
    Mesh(
        const Material* mat,
        const std::vector<float>& vertex_data);
    Mesh(const Material* mat, const Geometry& geom);
    virtual ~Mesh();
    GLuint ShaderId() const;
    void SetMaterial(const Material* mat);

};


#endif
