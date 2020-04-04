#ifndef _MESH_H_
#define _MESH_H_

#include <string>
#include <vector>
#include "glyk/gl_header_files.h"
#include "glyk/object3d.h"
#include "glyk/mat4x4.h"
#include "glyk/vec3.h"
#include "glyk/shader.h"
#include "glyk/material.h"
#include "glyk/geometry.h"


class Mesh : public Object3D
{

protected:

    enum DrawMode {
        None = 0x00000000,
        Arrays,
        Elements
    };

    Shader shader;
    DrawMode draw_mode;
    GLenum draw_primitives;
    unsigned int draw_count;
    GLuint VAO;
    GLuint vertex_buffer;
    GLuint normal_buffer;
    GLuint texcoord_buffer;
    GLuint TBN_buffer;
    GLuint EBO; 
    const Material* material;

    void Initialize(
        const Material* mat,
        const std::vector<float>& vertex_data,
        const std::vector<float>& normal_data,
        const std::vector<float>& texcoord_data,
        const std::vector<float>& TBN_data,
        const std::vector<unsigned int>& index_data,
        Geometry::PrimitivesType p
    );
    void Render();
    void PrepareMaterial(GLuint shader_id);
    void PipeUniformData(GLuint shader_id);
    std::string ShaderMeshUniformIdentifier(const std::string& member_name);

    friend class Scene;

public:

    static std::string mesh_uniform_name;

    Mesh(const Material* mat, const Geometry& geom);
    virtual ~Mesh();
    GLuint ShaderId() const;
    void SetMaterial(const Material* mat);

};


#endif
