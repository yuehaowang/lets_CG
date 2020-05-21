#include <iostream>

#include "glyk/gl_header_files.h"
#include "glyk/window.h"
#include "glyk/shader.h"
#include "glyk/image.h"
#include "glyk/loader.h"
#include "glyk/mat4x4.h"
#include "glyk/vec3.h"


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

    static const float g_vertices_data[32];
    static const GLuint g_elements_data[6];

public:

    MainWindow()
        : Window("Texture", 800, 600)
        , shader("texture/shaders/texture.vs", "texture/shaders/texture.fs")
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &vertex_buf);
        glGenBuffers(1, &element_buf);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices_data), g_vertices_data, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buf);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_elements_data), g_elements_data, GL_STATIC_DRAW);
    
        CreateTexture(0, &texture0, "resources/wall.jpg");
        CreateTexture(1, &texture1, "resources/avatar.png");
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

        if (!img.IsNull()) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.Width(), img.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.Data());
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

        Mat4x4f trans_mat;
        trans_mat.Rotate(-30.0, 0.0, 0.0);
        trans_mat.LookAt(Vec3f(0, 0, 1.5), Vec3f(0, 0, 0), Vec3f(0, 1, 0));
        trans_mat.Perspective(90, 4 / 3, 1, 100);

        glUniformMatrix4fv(glGetUniformLocation(shader.Id(), "trans"), 1, GL_TRUE, trans_mat.Ptr());

        glUniform1i(glGetUniformLocation(shader.Id(), "texture0"), 0);
        glUniform1i(glGetUniformLocation(shader.Id(), "texture1"), 1);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

};

const float MainWindow::g_vertices_data[32] = {
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