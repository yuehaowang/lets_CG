#define TRANSLATION_STEP 0.01f
#define ROTATION_STEP 0.05f
#define LIMIT_ROTATION_PITCH 60.0f

#include <vector>
#include <iterator>
#include "glyk/shader.h"
#include "glyk/window.h"
#include "glyk/mat4x4.h"
#include "glyk/vec3.h"
#include "glyk/mesh.h"
#include "glyk/material.h"
#include "glyk/camera.h"
#include "glyk/light.h"
#include "glyk/scene.h"
#include "glyk/gl_header_files.h"
#include "glyk/geometry.h"


const float TRIANGLE_VERTICES[9] = {
    -0.5, -0.5, 0.0,
    0.0, 0.5, 0.0,
    0.5, -0.5, 0.0
};

const float TRIANGLE_NORMALS[9] = {
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0
};

const float TETRAHEDRON_VERTICES[36] = {
    0.0, 0.5, 0.0,
    0.0, 0.0, 0.5,
    0.43, 0.0, -0.25,

    0.0, 0.5, 0.0,
    -0.43, 0.0, -0.25,
    0.0, 0.0, 0.5,

    0.0, 0.5, 0.0,
    0.43, 0.0, -0.25,
    -0.43, 0.0, -0.25,

    0.0, 0.0, 0.5,
    -0.43, 0.0, -0.25,
    0.43, 0.0, -0.25
};

const float TETRAHEDRON_NORMALS[36] = {
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,

    -1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,

    -1.0, 1.0, -1.0,
    -1.0, 1.0, -1.0,
    -1.0, 1.0, -1.0,

    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0
};


///////////////// Main Window /////////////////

class MainWindow : public Window
{

private:

    Vec3f old_mouse_pos;
    Vec3f camera_rotation;
    Scene main_scene;
    PerspectiveCamera cam;
    BasicMaterial* mat1;
    BasicMaterial* mat2;

public:

    MainWindow()
        : Window("Lighting", 800, 600)
        , cam(45, (float)WindowWidth() / (float)WindowHeight(), 0.1, 100)
        , old_mouse_pos(0, 0, 0)
    {
        /* Cameras */
        cam.Translate(0, 0, 5);
        main_scene.SetMainCamera(&cam);

        /* Lights */
        DirectionalLight* light1 = new DirectionalLight(
            Vec3f(0.05f, 0.05f, 0.05f),
            Vec3f(0.4f, 0.4f, 0.4f),
            Vec3f(0.4f, 0.6f, 0.8f)
        );
        light1->Rotate(180, 0, 0);
        main_scene.Add(light1);

        DirectionalLight* light2 = new DirectionalLight(
            Vec3f(0.08f, 0.08f, 0.08f),
            Vec3f(1.0f, 1.0f, 1.0f),
            Vec3f(0.8f, 0.5f, 0.2f)
        );
        light2->Rotate(45, 0, 0);
        main_scene.Add(light2);

        DirectionalLight* light3 = new DirectionalLight(
            Vec3f(0.1f, 0.1f, 0.1f),
            Vec3f(0.4f, 0.5f, 0.4f),
            Vec3f(0.6f, 0.7f, 0.6f)
        );
        light3->Rotate(0, 90, 0);
        main_scene.Add(light3);


        /* Materials */
        mat1 = new BasicMaterial(
            "glyk/shaders/standard",
            Vec3f(1.0f, 0.5f, 0.31f),
            Vec3f(0.5f, 0.5f, 0.5f),
            12.0f
        );

        mat2 = new BasicMaterial(
            "glyk/shaders/standard",
            Vec3f(1.0f, 0.7f, 0.31f),
            Vec3f(0.8f, 0.8f, 0.8f),
            32.0f
        );

        /* Create objects */
        CreateTriangle();
        CreateTetrahedron();
        CreateBox();
        CreateBall();
    }

    void CreateTriangle()
    {
        std::vector<float> vert = std::vector<float>(
            TRIANGLE_VERTICES,
            TRIANGLE_VERTICES + (sizeof(TRIANGLE_VERTICES) / sizeof(TRIANGLE_VERTICES[0]))
        );
        std::vector<float> norm = std::vector<float>(
            TRIANGLE_NORMALS,
            TRIANGLE_NORMALS + (sizeof(TRIANGLE_NORMALS) / sizeof(TRIANGLE_NORMALS[0]))
        );

        Geometry geom(vert, norm);

        Mesh* tri = new Mesh(mat2, geom);
        tri->Translate(-3.5, 0, 1.3);
        tri->Rotate(0, 30, 0);
        main_scene.Add(tri);
    }

    void CreateTetrahedron()
    {
        std::vector<float> vert = std::vector<float>(
            TETRAHEDRON_VERTICES,
            TETRAHEDRON_VERTICES + (sizeof(TETRAHEDRON_VERTICES) / sizeof(TETRAHEDRON_VERTICES[0]))
        );
        std::vector<float> norm = std::vector<float>(
            TETRAHEDRON_NORMALS,
            TETRAHEDRON_NORMALS + (sizeof(TETRAHEDRON_NORMALS) / sizeof(TETRAHEDRON_NORMALS[0]))
        );

        Geometry geom(vert, norm);

        Mesh* tetra = new Mesh(mat2, geom);
        tetra->Translate(2.0, -0.5, 3);
        tetra->Scale(1, 1.3, 1);
        tetra->Rotate(0, 60, 0);
        main_scene.Add(tetra);
    }

    void CreateBox()
    {
        Mesh* box = new Mesh(mat2, BoxGeometry());
        box->Translate(-1.5, 0, 0.5);
        main_scene.Add(box);
    }

    void CreateBall()
    {
        Mesh* sphere = new Mesh(mat1, SphereGeometry());
        sphere->Translate(1, 0, 0);
        main_scene.Add(sphere);
    }

    void OnStart()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        /* Enabled MSAA */
        glfwWindowHint(GLFW_SAMPLES, 4);
        glEnable(GL_MULTISAMPLE);

        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

        /* Hide mouse cursor */
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
        delete mat1;
        delete mat2;
    }

    void MouseControl()
    {
        double mouse_x, mouse_y;
        glfwGetCursorPos(WindowHandler(), &mouse_x, &mouse_y);

        float rotate_y = (old_mouse_pos.x - mouse_x) * ROTATION_STEP * old_mouse_pos.z;
        float rotate_x = (mouse_y - old_mouse_pos.y) * ROTATION_STEP * old_mouse_pos.z;

        camera_rotation += Vec3f(rotate_x, rotate_y, 0);
        if (camera_rotation.x > LIMIT_ROTATION_PITCH - 1) {
            camera_rotation.x = LIMIT_ROTATION_PITCH;
        } else if (camera_rotation.x < -LIMIT_ROTATION_PITCH + 1) {
            camera_rotation.x = -LIMIT_ROTATION_PITCH;
        }

        Vec3f front;
        front.x = cos((camera_rotation.x) * PI / 180) * sin((camera_rotation.y) * PI / 180);
        front.y = sin((camera_rotation.x) * PI / 180);
        front.z = cos((camera_rotation.y) * PI / 180) * cos((camera_rotation.x) * PI / 180);
        cam.LookAt(front);

        old_mouse_pos.x = mouse_x;
        old_mouse_pos.y = mouse_y;
        old_mouse_pos.z = 1;
    }

    void KeyControl()
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cam.Translate(0, 0, -TRANSLATION_STEP);
        } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cam.Translate(0, 0, TRANSLATION_STEP);
        }
        
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cam.Translate(TRANSLATION_STEP, 0, 0);
        } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cam.Translate(-TRANSLATION_STEP, 0, 0);
        }
    }

};

int main()
{
    MainWindow main_window;
    main_window.Run();

    return 0;
}