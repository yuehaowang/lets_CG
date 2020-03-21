#include <string>
#include <vector>
#include "mesh.h"


std::string Mesh::mesh_uniform_name = "mesh";

Mesh::Mesh(
    const Material* mat,
    const std::vector<GLfloat>& vertex_data,
    const std::vector<GLfloat>& normal_data,
    const std::vector<unsigned int>& index_data)
: DisplayObject()
{
    Initialize(mat, vertex_data, normal_data, index_data);
}

Mesh::Mesh(
    const Material* mat,
    const std::vector<GLfloat>& vertex_data,
    const std::vector<GLfloat>& normal_data)
: DisplayObject()
{
    Initialize(mat, vertex_data, normal_data, std::vector<unsigned int>());
}

Mesh::Mesh(
    const Material* mat,
    const std::vector<GLfloat>& vertex_data)
: DisplayObject()
{
    Initialize(mat, vertex_data, std::vector<GLfloat>(), std::vector<unsigned int>());
}

Mesh::Mesh(const Material* mat, const Geometry& geom)
: DisplayObject()
{
    Initialize(mat, geom.VertexData(), geom.NormalData(), geom.IndexData());
}

Mesh::~Mesh()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &normal_buffer);
    glDeleteVertexArrays(1, &VAO);

    shader.Dismiss();
}

void Mesh::Initialize(
        const Material* mat,
        const std::vector<GLfloat>& vertex_data,
        const std::vector<GLfloat>& normal_data,
        const std::vector<unsigned int>& index_data)
{
    SetMaterial(mat);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(GLfloat), &vertex_data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, normal_data.size() * sizeof(GLfloat), &normal_data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, (normal_data.size() > 0) ? 3 : 0, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(unsigned int), &index_data[0], GL_STATIC_DRAW);

    if (index_data.size() > 0) {
        draw_mode = DRAW_ELEMENTS;
        draw_count = index_data.size();
    } else {
        draw_mode = DRAW_ARRAYS;
        draw_count = vertex_data.size() / 3;
    }
}

void Mesh::SetMaterial(const Material* mat)
{
    shader.Dismiss();
    shader = Shader(mat->ShaderName() + ".vs", mat->ShaderName() + ".fs");
    shader.Employ();

    material = mat;
}

void Mesh::Render()
{
    glBindVertexArray(VAO);
    if (draw_mode == DRAW_ARRAYS) {
        glDrawArrays(GL_TRIANGLES, 0, draw_count);
    } else {
        glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

std::string Mesh::ShaderMeshUniformIdentifier(const std::string& member_name)
{
    return mesh_uniform_name + "." + member_name;
}

void Mesh::PrepareMaterial(GLuint shader_id)
{
    if (!material) {
        return;
    }
    material->PipeUniformData(shader_id);
}

void Mesh::PipeUniformData(GLuint shader_id)
{
    glUniformMatrix4fv(
        glGetUniformLocation(shader_id, ShaderMeshUniformIdentifier("model").c_str()), 1,
        GL_TRUE, Transform().Ptr()
    );

    PrepareMaterial(shader_id);
}

GLuint Mesh::ShaderId() const
{
    return shader.Id();
}
