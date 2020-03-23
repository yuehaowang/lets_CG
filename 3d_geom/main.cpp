#define TRANSLATION_STEP 1
#define ROTATION_STEP 0.5

#include "glyk/shader.h"
#include "glyk/window.h"
#include "glyk/mat4x4.h"
#include "glyk/vec3.h"
#include "glyk/gl_header_files.h"


class MainWindow : public Window
{

private:

    Shader shader;
    GLuint VAO;
    GLuint vertex_buffer;
    GLuint color_buffer;
    static const float g_vertex_buffer_data[18 * 16];
    static const float g_color_buffer_data[18 * 16];
    Vec3f translation;
    Vec3f rotation;

public:

    MainWindow()
        : Window("3D Geom", 800, 600)
        , shader("3d_geom/shaders/3d_geom.vs", "3d_geom/shaders/3d_geom.fs")
        , translation(0, 0, 0)
        , rotation(0, 0, 0)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &vertex_buffer);
        glGenBuffers(1, &color_buffer);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);
    }

    void OnStart()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_CULL_FACE);

        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    }

    void OnUpdate()
    {
        KeyControl();

        glUseProgram(shader.Id());

        GLuint trans_mat = glGetUniformLocation(shader.Id(), "trans");

        Mat4x4f m;
        m.Rotate(rotation.x, rotation.y, rotation.z);
        m.Translate(translation.x, translation.y, translation.z);

        Mat4x4f v;
        v.LookAt(Vec3f(0, 0, 400), Vec3f(0, 0, 0), Vec3f(0, 1, 0));
        
        Mat4x4f p;
        p.Perspective(90, (float)WindowWidth() / (float)WindowHeight(), 300, 800);

        glUniformMatrix4fv(trans_mat, 1, GL_TRUE, (p * v * m).Ptr());

        glDrawArrays(GL_TRIANGLES, 0, 6 * 16);
    }

    void OnDestory()
    {
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glDeleteBuffers(1, &vertex_buffer);
        glDeleteBuffers(1, &color_buffer);
        glDeleteVertexArrays(1, &VAO);

        glDeleteProgram(shader.Id());
    }

    void KeyControl()
    {
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            translation.x -= TRANSLATION_STEP;
        } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            translation.x += TRANSLATION_STEP;
        } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            translation.y -= TRANSLATION_STEP;
        } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            translation.y += TRANSLATION_STEP;
        } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            translation.z -= TRANSLATION_STEP;
        } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            translation.z += TRANSLATION_STEP;
        } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            rotation.x -= ROTATION_STEP;
        } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            rotation.x += ROTATION_STEP;
        } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            rotation.y -= ROTATION_STEP;
        } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            rotation.y += ROTATION_STEP;
        } else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
            rotation.z -= ROTATION_STEP;
        } else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
            rotation.z += ROTATION_STEP;
        }
    }

};

const float MainWindow::g_vertex_buffer_data[] = {
    // left column front
    0,   0,  0,
    0, 150,  0,
    30,   0,  0,
    0, 150,  0,
    30, 150,  0,
    30,   0,  0,

    // top rung front
    30,   0,  0,
    30,  30,  0,
    100,   0,  0,
    30,  30,  0,
    100,  30,  0,
    100,   0,  0,

    // middle rung front
    30,  60,  0,
    30,  90,  0,
    67,  60,  0,
    30,  90,  0,
    67,  90,  0,
    67,  60,  0,

    // left column back
    0,   0,  30,
    30,   0,  30,
    0, 150,  30,
    0, 150,  30,
    30,   0,  30,
    30, 150,  30,

    // top rung back
    30,   0,  30,
    100,   0,  30,
    30,  30,  30,
    30,  30,  30,
    100,   0,  30,
    100,  30,  30,

    // middle rung back
    30,  60,  30,
    67,  60,  30,
    30,  90,  30,
    30,  90,  30,
    67,  60,  30,
    67,  90,  30,

    // top
    0,   0,   0,
    100,   0,   0,
    100,   0,  30,
    0,   0,   0,
    100,   0,  30,
    0,   0,  30,

    // top rung right
    100,   0,   0,
    100,  30,   0,
    100,  30,  30,
    100,   0,   0,
    100,  30,  30,
    100,   0,  30,

    // under top rung
    30,   30,   0,
    30,   30,  30,
    100,  30,  30,
    30,   30,   0,
    100,  30,  30,
    100,  30,   0,

    // between top rung and middle
    30,   30,   0,
    30,   60,  30,
    30,   30,  30,
    30,   30,   0,
    30,   60,   0,
    30,   60,  30,

    // top of middle rung
    30,   60,   0,
    67,   60,  30,
    30,   60,  30,
    30,   60,   0,
    67,   60,   0,
    67,   60,  30,

    // right of middle rung
    67,   60,   0,
    67,   90,  30,
    67,   60,  30,
    67,   60,   0,
    67,   90,   0,
    67,   90,  30,

    // bottom of middle rung.
    30,   90,   0,
    30,   90,  30,
    67,   90,  30,
    30,   90,   0,
    67,   90,  30,
    67,   90,   0,

    // right of bottom
    30,   90,   0,
    30,  150,  30,
    30,   90,  30,
    30,   90,   0,
    30,  150,   0,
    30,  150,  30,

    // bottom
    0,   150,   0,
    0,   150,  30,
    30,  150,  30,
    0,   150,   0,
    30,  150,  30,
    30,  150,   0,

    // left side
    0,   0,   0,
    0,   0,  30,
    0, 150,  30,
    0,   0,   0,
    0, 150,  30,
    0, 150,   0
};

const float MainWindow::g_color_buffer_data[] = {
    // left column front
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,

    // top rung front
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,

    // middle rung front
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,
    200,  70, 120,

    // left column back
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,

    // top rung back
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,

    // middle rung back
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,
    80, 70, 200,

    // top
    70, 200, 210,
    70, 200, 210,
    70, 200, 210,
    70, 200, 210,
    70, 200, 210,
    70, 200, 210,

    // top rung right
    200, 200, 70,
    200, 200, 70,
    200, 200, 70,
    200, 200, 70,
    200, 200, 70,
    200, 200, 70,

    // under top rung
    210, 100, 70,
    210, 100, 70,
    210, 100, 70,
    210, 100, 70,
    210, 100, 70,
    210, 100, 70,

    // between top rung and middle
    210, 160, 70,
    210, 160, 70,
    210, 160, 70,
    210, 160, 70,
    210, 160, 70,
    210, 160, 70,

    // top of middle rung
    70, 180, 210,
    70, 180, 210,
    70, 180, 210,
    70, 180, 210,
    70, 180, 210,
    70, 180, 210,

    // right of middle rung
    100, 70, 210,
    100, 70, 210,
    100, 70, 210,
    100, 70, 210,
    100, 70, 210,
    100, 70, 210,

    // bottom of middle rung.
    76, 210, 100,
    76, 210, 100,
    76, 210, 100,
    76, 210, 100,
    76, 210, 100,
    76, 210, 100,

    // right of bottom
    140, 210, 80,
    140, 210, 80,
    140, 210, 80,
    140, 210, 80,
    140, 210, 80,
    140, 210, 80,

    // bottom
    90, 130, 110,
    90, 130, 110,
    90, 130, 110,
    90, 130, 110,
    90, 130, 110,
    90, 130, 110,

    // left side
    160, 160, 220,
    160, 160, 220,
    160, 160, 220,
    160, 160, 220,
    160, 160, 220,
    160, 160, 220
};

int main()
{
    MainWindow main_window;
    main_window.Run();

    return 0;
}