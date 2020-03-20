#define TRANSLATION_STEP 0.01
#define ROTATION_STEP 0.1

#include <vector>
#include <iterator>
#include "utils/shader.h"
#include "utils/window.h"
#include "utils/mat4x4.h"
#include "utils/vec3.h"
#include "utils/mesh.h"
#include "utils/material.h"
#include "utils/camera.h"
#include "utils/light.h"
#include "utils/scene.h"
#include "utils/gl_header_files.h"


///////////////// Generate Sphere /////////////////

void generate_geometry(
    float radius, unsigned int sectorCount, unsigned int stackCount,
    std::vector<GLfloat>& vertices,
    std::vector<GLfloat>& normals,
    std::vector<unsigned int>& indices)
{
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);
        }
    }

    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}


///////////////// Box Vertices & Normals /////////////////

static GLfloat box_vertices[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

static GLfloat box_normals[] = {
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,

    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,

    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,

    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.01f
};


///////////////// Main Window /////////////////

class MainWindow : public Window
{

private:

    Vec3<GLfloat> old_mouse_pos;
    Scene main_scene;
    PerspectiveCamera cam;
    std::vector<Mesh*> mesh_list;

public:

    MainWindow()
        : Window("Lighting", 800, 800)
        , cam(45, (float)WindowWidth() / (float)WindowHeight(), 0.1, 100)
        , old_mouse_pos(0, 0, 0)
    {
        cam.Translate(0, 0, 5);
        main_scene.SetMainCamera(&cam);

        DirectionalLight* light1 = new DirectionalLight(
            Vec3<GLfloat>(0.05f, 0.05f, 0.05f),
            Vec3<GLfloat>(0.1f, 0.2f, 0.3f),
            Vec3<GLfloat>(0.5f, 0.6f, 0.8f),
            Vec3<GLfloat>(0.0f, -1.0f, -1.0f)
        );
        main_scene.Add(light1);

        DirectionalLight* light2 = new DirectionalLight(
            Vec3<GLfloat>(0.5f, 0.5f, 0.5f),
            Vec3<GLfloat>(0.1f, 0.5f, 0.1f),
            Vec3<GLfloat>(0.0f, 1.0f, 1.0f),
            Vec3<GLfloat>(1.0f, 0.0f, 0.0f)
        );
        main_scene.Add(light2);

        DirectionalLight* light3 = new DirectionalLight(
            Vec3<GLfloat>(0.1f, 0.1f, 0.1f),
            Vec3<GLfloat>(0.0f, 0.3f, 0.8f),
            Vec3<GLfloat>(0.0f, 1.0f, 1.0f),
            Vec3<GLfloat>(0.0f, -1.0f, 0.0f)
        );
        main_scene.Add(light3);

        CreateBox();
        CreateBall();
    }

    void CreateBox()
    {
        std::vector<GLfloat> vertices(box_vertices, box_vertices + sizeof(box_vertices) / sizeof(box_vertices[0]));
        std::vector<GLfloat> normals(box_normals, box_normals + sizeof(box_normals) / sizeof(box_normals[0]));

        BasicMaterial mat(
            "src/shaders/lighting",
            Vec3<GLfloat>(1.0f, 0.5f, 0.31f),
            Vec3<GLfloat>(0.5f, 0.5f, 0.5f),
            20.0f
        );
        Mesh* box = new Mesh(mat, vertices, normals);
        box->Translate(-1.5, 0, 0.5);
        main_scene.Add(box);
    }

    void CreateBall()
    {
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> normals;
        std::vector<unsigned int> indices;
        generate_geometry(1, 100, 100, vertices, normals, indices);

        BasicMaterial mat(
            "src/shaders/lighting",
            Vec3<GLfloat>(1.0f, 0.5f, 0.31f),
            Vec3<GLfloat>(0.5f, 0.5f, 0.5f),
            12.0f
        );
        Mesh* sphere = new Mesh(mat, vertices, normals, indices);
        sphere->Translate(1, 0, 0);
        main_scene.Add(sphere);
    }

    void OnStart()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        glfwSetInputMode(WindowHandler(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void OnUpdate()
    {
        MouseControl();
        KeyControl();

        main_scene.Render();
    }

    void OnDestroy()
    {
        for (std::vector<Mesh*>::iterator it = mesh_list.begin(); it != mesh_list.end(); it++) {
            delete *it;
        }
    }

    void MouseControl()
    {
        double mouse_x, mouse_y;
        glfwGetCursorPos(WindowHandler(), &mouse_x, &mouse_y);

        float rotate_y = (old_mouse_pos.x - mouse_x) * ROTATION_STEP * old_mouse_pos.z;
        float rotate_x = (old_mouse_pos.y - mouse_y) * ROTATION_STEP * old_mouse_pos.z;

        cam.Rotate(rotate_x, rotate_y, 0);

        old_mouse_pos.x = mouse_x;
        old_mouse_pos.y = mouse_y;
        old_mouse_pos.z = 1;
    }

    void KeyControl()
    {
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cam.Translate(-TRANSLATION_STEP, 0, 0);
        } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cam.Translate(TRANSLATION_STEP, 0, 0);
        } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cam.Translate(0, 0, TRANSLATION_STEP);
        } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cam.Translate(0, 0, -TRANSLATION_STEP);
        }
    }

};

int main()
{
    MainWindow main_window;
    main_window.Run();

    return 0;
}