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
    BasicMaterial mat;

    void Initialize(
        const BasicMaterial& material,
        const std::vector<GLfloat>& vertex_data,
        const std::vector<GLfloat>& normal_data,
        const std::vector<unsigned int>& index_data
    );
    void PrepareRender();
    void Render();

    friend class Scene; 

public:

    Mesh(
        const BasicMaterial& material,
        const std::vector<GLfloat>& vertex_data,
        const std::vector<GLfloat>& normal_data,
        const std::vector<unsigned int>& index_data
    );
    Mesh(
        const BasicMaterial& material,
        const std::vector<GLfloat>& vertex_data,
        const std::vector<GLfloat>& normal_data
    );
    Mesh(
        const BasicMaterial& material,
        const std::vector<GLfloat>& vertex_data
    );
    virtual ~Mesh();

};


#endif
