#include <string>
#include <vector>
#include "mesh.h"


Mesh::Mesh(
    const BasicMaterial& material,
    const std::vector<GLfloat>& vertex_data,
    const std::vector<GLfloat>& normal_data,
    const std::vector<unsigned int>& index_data)
: DisplayObject()
{
    Initialize(material, vertex_data, normal_data, index_data);
}

Mesh::Mesh(
    const BasicMaterial& material,
    const std::vector<GLfloat>& vertex_data,
    const std::vector<GLfloat>& normal_data)
: DisplayObject()
{
    Initialize(material, vertex_data, normal_data, std::vector<unsigned int>());
}

Mesh::Mesh(
    const BasicMaterial& material,
    const std::vector<GLfloat>& vertex_data)
: DisplayObject()
{
    Initialize(material, vertex_data, std::vector<GLfloat>(), std::vector<unsigned int>());
}

void Mesh::Initialize(
        const BasicMaterial& material,
        const std::vector<GLfloat>& vertex_data,
        const std::vector<GLfloat>& normal_data,
        const std::vector<unsigned int>& index_data)
{
    shader = Shader(material.shader_name + ".vs", material.shader_name + ".fs");
    shader.EmployProgram();

    mat = material;

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

Mesh::~Mesh()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &normal_buffer);
    glDeleteVertexArrays(1, &VAO);

    shader.DismissProgram();
}

void Mesh::PrepareRender()
{
    glUseProgram(shader.Id());

    glUniformMatrix4fv(
        glGetUniformLocation(shader.Id(), "transform.model"), 1,
        GL_TRUE, transform.Ptr()
    );

    glUniform3f(
        glGetUniformLocation(shader.Id(), "material.diffuse"),
        mat.diffuse.x, mat.diffuse.y, mat.diffuse.z
    );
    glUniform3f(
        glGetUniformLocation(shader.Id(), "material.specular"),
        mat.specular.x, mat.specular.y, mat.specular.z
    );
    glUniform1f(
        glGetUniformLocation(shader.Id(), "material.shininess"),
        mat.shininess
    );
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
