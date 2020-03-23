#include "glyk/window.h"
#include "glyk/shader.h"
#include "glyk/gl_header_files.h"


class MainWindow : public Window
{

private:

    Shader shader;

    static const float g_vertex_data[9];

public:

    MainWindow()
        : Window("Triangle", 600, 600)
        , shader("triangle/shaders/triangle.vs", "triangle/shaders/triangle.fs")
    {
        GLuint VAO;
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        GLuint VBO;
        glGenBuffers(1, &VBO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_data), g_vertex_data, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
    }

    void OnUpdate()
    {
        glUseProgram(shader.Id());

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

};

const float MainWindow::g_vertex_data[9] = {
    -1.0, -1.0, 0,
    0, 1.0, 0,
    1.0, -1.0, 0
};

int main()
{
    MainWindow main_window;
    main_window.Run();

    return 0;
}