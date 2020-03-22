#define TRANSLATION_STEP 0.01f
#define ROTATION_STEP 0.05f
#define LIMIT_ROTATION_PITCH 60.0f

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
#include "utils/geometry.h"


///////////////// Main Window /////////////////

class MainWindow : public Window
{

private:

    Vec3f old_mouse_pos;
    Vec3f camera_rotation;
    Scene main_scene;
    PerspectiveCamera cam;
    std::vector<Mesh*> mesh_list;
    BasicColorMaterial* box_mat;
    BasicColorMaterial* sphere_mat;

public:

    MainWindow()
        : Window("Lighting", 800, 800)
        , cam(45, (float)WindowWidth() / (float)WindowHeight(), 0.1, 100)
        , old_mouse_pos(0, 0, 0)
    {
        /* Cameras */
        cam.Translate(0, 0, 5);
        main_scene.SetMainCamera(&cam);

        /* Lights */
        // Lightblue light directed to -Z
        DirectionalLight* light1 = new DirectionalLight(
            Vec3f(0.05f, 0.05f, 0.05f),
            Vec3f(0.1f, 0.2f, 0.8f),
            Vec3f(0.4f, 0.6f, 0.8f)
        );
        light1->Rotate(180, 0, 0);
        main_scene.Add(light1);

        // White light directed to -Y
        DirectionalLight* light2 = new DirectionalLight(
            Vec3f(0.08f, 0.08f, 0.08f),
            Vec3f(1.0f, 1.0f, 1.0f),
            Vec3f(0.8f, 0.1f, 0.2f)
        );
        light2->Rotate(90, 0, 0);
        main_scene.Add(light2);

        // Green light directed to X
        DirectionalLight* light3 = new DirectionalLight(
            Vec3f(0.1f, 0.1f, 0.1f),
            Vec3f(0.2f, 0.7f, 0.2f),
            Vec3f(0.1f, 0.7f, 0.2f)
        );
        light3->Rotate(0, 90, 0);
        main_scene.Add(light3);


        /* Materials */
        sphere_mat = new BasicColorMaterial(
            "src/shaders/lighting",
            Vec3f(1.0f, 0.5f, 0.31f),
            Vec3f(0.5f, 0.5f, 0.5f),
            12.0f
        );

        box_mat = new BasicColorMaterial(
            "src/shaders/lighting",
            Vec3f(1.0f, 0.5f, 0.31f),
            Vec3f(0.5f, 0.5f, 0.5f),
            32.0f
        );

        /* Create objects */
        CreateBox();
        CreateBall();
    }

    void CreateBox()
    {
        Mesh* box = new Mesh(box_mat, BoxGeometry());
        box->Translate(-1.5, 0, 0.5);
        main_scene.Add(box);
    }

    void CreateBall()
    {
        Mesh* sphere = new Mesh(sphere_mat, SphereGeometry());
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

        glClearColor(0.7f, 0.7f, 0.7f, 0.0f);

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
        for (std::vector<Mesh*>::iterator it = mesh_list.begin(); it != mesh_list.end(); it++) {
            delete *it;
        }

        delete box_mat;
        delete sphere_mat;
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