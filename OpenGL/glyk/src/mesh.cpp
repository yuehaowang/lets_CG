#include "glyk/mesh.h"


std::string Mesh::mesh_uniform_name = "mesh";

Mesh::Mesh(const Material* mat, const Geometry& geom)
: Object3D()
{
    Initialize(
        mat, geom.VertexData(), geom.NormalData(), geom.TexCoordData(),
        geom.TBNData(), geom.IndexData(), geom.Primitives()
    );
}

Mesh::~Mesh()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &normal_buffer);
    glDeleteBuffers(1, &texcoord_buffer);
    glDeleteBuffers(1, &TBN_buffer);
    glDeleteVertexArrays(1, &VAO);

    shader.Dismiss();
}

void Mesh::Initialize(
        const Material* mat,
        const std::vector<float>& vertex_data,
        const std::vector<float>& normal_data,
        const std::vector<float>& texcoord_data,
        const std::vector<float>& TBN_data,
        const std::vector<unsigned int>& index_data,
        Geometry::PrimitivesType p)
{
    SetMaterial(mat);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), &vertex_data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, normal_data.size() * sizeof(float), &normal_data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    if (!normal_data.empty()) {
        glEnableVertexAttribArray(1);
    }

    glGenBuffers(1, &texcoord_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
    glBufferData(GL_ARRAY_BUFFER, texcoord_data.size() * sizeof(float), &texcoord_data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    if (!texcoord_data.empty()) {
        glEnableVertexAttribArray(2);
    }

    glGenBuffers(1, &TBN_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, TBN_buffer);
    glBufferData(GL_ARRAY_BUFFER, TBN_data.size() * sizeof(float), &TBN_data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    if (!TBN_data.empty()) {
        glEnableVertexAttribArray(3);
    }

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(unsigned int), &index_data[0], GL_STATIC_DRAW);

    switch (p) {
        case Geometry::Lines:
            draw_primitives = GL_LINES;
            break;
        case Geometry::Triangles:
            draw_primitives = GL_TRIANGLES;
            break;
        default:
            draw_primitives = GL_TRIANGLES;
            break;
    }

    if (index_data.size() > 0) {
        draw_mode = Elements;
        draw_count = index_data.size();
    } else {
        draw_mode = Arrays;
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
    if (draw_mode == Arrays) {
        glDrawArrays(draw_primitives, 0, draw_count);
    } else if (draw_mode == Elements) {
        glDrawElements(draw_primitives, draw_count, GL_UNSIGNED_INT, 0);
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
