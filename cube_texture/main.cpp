#define ROTATION_STEP 50

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>

#include "glyk/gl_header_files.h"
#include "glyk/window.h"
#include "glyk/shader.h"
#include "glyk/loader.h"
#include "glyk/mat4x4.h"
#include "glyk/vec3.h"
#include "glyk/image.h"
#include "glyk/glm_debug.h"


class MainWindow : public Window
{

private:

    Shader shader;
    GLuint VAO;
    GLuint vertex_buffer;
    GLuint texture0;

    static const float g_cube_vertices_data[180];

public:

    MainWindow()
        : Window("Cube Texture", 600, 600)
        , shader("cube_texture/shaders/cube_texture.vs", "cube_texture/shaders/cube_texture.fs")
    {
        glGenVertexArrays(1, &VAO);
        
        glBindVertexArray(VAO);

        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_vertices_data), g_cube_vertices_data, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    
        CreateTexture();
    }

    void CreateTexture()
    {
        glGenTextures(1, &texture0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        Image img = Loader::LoadImage("resources/avatar.png");

        if (!img.IsNull()) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.Width(), img.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.Data());
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        img.Free();
    }

    void OnStart()
    {
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_CULL_FACE);
    }

    void OnUpdate()
    {
        glUseProgram(shader.Id());

        for (int i = 0; i < 5; i++) {
            float angle = (float)glfwGetTime() * ROTATION_STEP * (i + 1);

            Mat4x4f trans_mat;
            trans_mat.Rotate(angle, angle, angle);
            trans_mat.Translate(-3.5 + i * 1.8, 0, 0);
            trans_mat.LookAt(Vec3f(0, 0, 5), Vec3f(0, 0, 0), Vec3f(0, 1, 0));
            trans_mat.Perspective(90, 1, 1, 100);

            glUniformMatrix4fv(glGetUniformLocation(shader.Id(), "trans"), 1, GL_TRUE, trans_mat.Ptr());

/*****************************************************************
 * Equivalent implementation of matrix transformation by GLM:

            glm::mat4 v = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 p;
            p = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 100.0f);

            glm::mat4 m = glm::mat4(1.0f);
            m = glm::translate(m, glm::vec3(-3.5 + i * 1.8, 0.0f, 0.0f));
            m = glm::rotate(m, glm::radians((float)angle), glm::vec3(0.0, 0.0, 1.0));
            m = glm::rotate(m, glm::radians((float)angle), glm::vec3(0.0, 1.0, 0.0));
            m = glm::rotate(m, glm::radians((float)angle), glm::vec3(1.0, 0.0, 0.0));

            glm::mat4 mvp = p*v*m;
            
            glUniformMatrix4fv(glGetUniformLocation(shader.Id(), "trans"), 1, GL_FALSE, &mvp[0][0]);

*****************************************************************/

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

};

const float MainWindow::g_cube_vertices_data[180] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

int main()
{
    MainWindow main_window;
    main_window.Run();

    return 0;
}
