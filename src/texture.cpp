#include <iostream>

#include "gl_header_files.h"
#include "window.h"
#include "shader.h"
#include "loader.h"
#include "image.h"
#include "mat4x4.h"
#include "vec3.h"


class MainWindow : public Window
{

private:

    Shader shader;
    GLuint VAO;
    GLuint vertex_buf;
    GLuint element_buf;
    GLuint texture0;
    GLuint texture1;
    GLuint cube_vertex_buf;

    static const GLfloat g_vertices_data[32];
    static const GLuint g_elements_data[6];

public:

    MainWindow()
        : Window("Texture", 800, 600)
        , shader("src/shaders/texture.vs", "src/shaders/texture.fs")
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &vertex_buf);
        glGenBuffers(1, &element_buf);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices_data), g_vertices_data, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buf);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_elements_data), g_elements_data, GL_STATIC_DRAW);
    
        CreateTexture(0, &texture0, "resource/wall.jpg");
        CreateTexture(1, &texture1, "resource/avatar.png");
    }

    void CreateTexture(GLuint texture_id, GLuint* tex, const std::string& img_path)
    {
        glGenTextures(1, tex);

        glActiveTexture(GL_TEXTURE0 + texture_id);
        
        glBindTexture(GL_TEXTURE_2D, *tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load an image
        Image img = Loader::LoadImage(img_path);

        if (img.data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        // free the image
        img.Free();
    }

    void OnStart()
    {
        glClearColor(0.9, 0.9, 0.9, 1);
    }

    void OnUpdate()
    {
        glUseProgram(shader.Id());

        Mat4x4<GLfloat> trans_mat;
        trans_mat.Perspective(Vec3<GLfloat>(0, 0, -1));
        trans_mat.Projection(Vec3<GLfloat>(1, 1, 1));
        trans_mat.Rotate(-40.0, 0.0, 0.0);

        glUniformMatrix4fv(glGetUniformLocation(shader.Id(), "trans"), 1, GL_FALSE, trans_mat.Ptr());

        glUniform1i(glGetUniformLocation(shader.Id(), "texture0"), 0);
        glUniform1i(glGetUniformLocation(shader.Id(), "texture1"), 1);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

};

const GLfloat MainWindow::g_vertices_data[32] = {
    // positions          // colors          // texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.5f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.5f, 1.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // top left 
};

const GLuint MainWindow::g_elements_data[6] = {  
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

int main()
{
    MainWindow main_window;
    main_window.Run();

    return 0;
}