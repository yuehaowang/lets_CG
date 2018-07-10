#define ROTATION_STEP 50

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>

#include "utils/gl_header_files.h"
#include "utils/window.h"
#include "utils/shader.h"
#include "utils/loader.h"
#include "utils/image.h"
#include "utils/mat4x4.h"
#include "utils/vec3.h"
#include "utils/glm_debug.h"


class MainWindow : public Window
{

private:

    Shader shader;
    GLuint VAO;
    GLuint vertex_buffer;
    GLuint texture0;

    static const GLfloat g_cube_vertices_data[180];

public:

    MainWindow()
        : Window("Cube Texture", 600, 600)
        , shader("src/shaders/cube_texture.vs", "src/shaders/cube_texture.fs")
    {
        glGenVertexArrays(1, &VAO);
        
        glBindVertexArray(VAO);

        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_vertices_data), g_cube_vertices_data, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
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
        
        Image img = Loader::LoadImage("resource/avatar.png");

        if (img.data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
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
            GLfloat angle = (GLfloat)glfwGetTime() * ROTATION_STEP * (i + 1);

            Mat4x4<GLfloat> trans_mat;
            trans_mat.Rotate(angle, angle, angle);
            trans_mat.Translate(-3.5 + i * 1.8, 0, 0);
            trans_mat.LookAt(Vec3<GLfloat>(0, 0, 5), Vec3<GLfloat>(0, 0, 0), Vec3<GLfloat>(0, 1, 0));
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

const GLfloat MainWindow::g_cube_vertices_data[180] = {
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
