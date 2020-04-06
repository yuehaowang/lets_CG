#define TRANSLATION_STEP 0.01f
#define ROTATION_STEP 0.05f
#define LIMIT_ROTATION_PITCH 60.0f

#include <vector>
#include <cmath>
#include "glyk/shader.h"
#include "glyk/window.h"
#include "glyk/mat4x4.h"
#include "glyk/vec3.h"
#include "glyk/mesh.h"
#include "glyk/material.h"
#include "glyk/camera.h"
#include "glyk/light.h"
#include "glyk/loader.h"
#include "glyk/scene.h"
#include "glyk/gl_header_files.h"
#include "glyk/geometry.h"
#include "glyk/texture.h"


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
    BasicMaterial* mat3;
    BasicMaterial* mat4;
    DirectionalLight* light1;
    PointLight* light2;
    PointLight* light3;
    PointLight* light4;

public:

    MainWindow()
        : Window("Model Texture", 800, 600)
        , cam(45, (float)WindowWidth() / (float)WindowHeight(), 0.1, 100)
        , old_mouse_pos(0, 0, 0)
        , light1(NULL)
        , light2(NULL)
        , light3(NULL)
        , light4(NULL)
    {
        /* Cameras */
        cam.Translate(0, 0, 10);
        main_scene.SetMainCamera(&cam);

        /* Lights */
        light1 = new DirectionalLight(
            Vec3f(0.05f, 0.05f, 0.05f),
            Vec3f(0.4f, 0.4f, 0.4f),
            Vec3f(0.4f, 0.6f, 0.8f)
        );
        light1->Translate(0, 3, 0);
        light1->Rotate(45, 0, 0);
        main_scene.Add(light1);
        light1->ShowIndicator();

        light2 = new PointLight(
            Vec3f(0.08f, 0.08f, 0.08f),
            Vec3f(1.0f, 1.0f, 1.0f),
            Vec3f(0.8f, 0.5f, 0.2f),
            0.1, 0.3, 1.0
        );
        light2->Translate(2, 0, 1);
        main_scene.Add(light2);
        light2->ShowIndicator();

        light3 = new PointLight(
            Vec3f(0.1f, 0.1f, 0.1f),
            Vec3f(0.4f, 0.5f, 0.4f),
            Vec3f(0.6f, 0.7f, 0.6f),
            0.0, 0.0, 1.0
        );
        light3->Translate(-2, 1, 0);
        main_scene.Add(light3);
        light3->ShowIndicator();

        light4 = new PointLight(
            Vec3f(0.1f, 0.1f, 0.1f),
            Vec3f(0.2f, 1.0f, 0.5f),
            Vec3f(0.2f, 0.2f, 0.2f),
            0.5, 0.3, 1.0
        );
        light4->Translate(2.3, 0, -4);
        main_scene.Add(light4);
        light4->ShowIndicator();

        /* Materials */
        mat1 = new BasicMaterial(
            "glyk/shaders/standard",
            Texture("model_texture/resources/cat_tex.jpg"),
            Texture("model_texture/resources/cat_normal.png")
        );

        mat2 = new BasicMaterial(
            "glyk/shaders/standard",
            Texture("model_texture/resources/pikachu.jpg"),
            Texture("model_texture/resources/pikachu.jpg"),
            32.0f,
            Texture("model_texture/resources/wall_normal.jpg")
        );

        mat3 = new BasicMaterial(
            "glyk/shaders/standard",
            Texture("model_texture/resources/awesomeface.jpg"),
            Texture("model_texture/resources/awesomeface.jpg"),
            12.0f,
            Texture("model_texture/resources/wall_normal.jpg")
        );

        mat4 = new BasicMaterial(
            "glyk/shaders/standard",
            Vec3f(1.0f, 0.5f, 0.31f),
            Vec3f(0.5f, 0.5f, 0.5f),
            12.0f,
            Texture("model_texture/resources/wall_normal.jpg")
        );

        /* Create objects */
        CreateBox();
        CreateBall();
        LoadPikachuModel();
        LoadCatModel();
    }

    void CreateBox()
    {
        Mesh* box = new Mesh(mat3, BoxGeometry(true));
        box->Translate(4.5, 0, 1.5);
        box->Scale(2.0, 2.0, 2.0);
        main_scene.Add(box);
    }

    void CreateBall()
    {
        Mesh* sphere = new Mesh(mat4, SphereGeometry(true));
        sphere->Translate(-4.5, 0, 1);
        main_scene.Add(sphere);
    }

    void LoadCatModel()
    {
        std::vector<Geometry> models = Loader::LoadModel(
            "model_texture/resources/cat.obj",
            Loader::ComputeTBN
        );

        Mesh* cat = new Mesh(mat1, models[0]);
        cat->Translate(4, -3, -5);
        cat->Scale(0.1, 0.1, 0.1);
        cat->Rotate(-90, 0, 0);
        main_scene.Add(cat);
    }

    void LoadPikachuModel()
    {
        std::vector<Geometry> models = Loader::LoadModel(
            "model_texture/resources/pikachu.obj",
            Loader::FlipTexcoordV
        );
        models[0].GenerateTBN(0.001);

        Mesh* pikachu = new Mesh(mat2, models[0]);
        pikachu->Rotate(-45, 0, 0);
        pikachu->Scale(0.5, 0.5, 0.5);
        pikachu->Translate(-1, -5, -4.5);
        main_scene.Add(pikachu);
    }

    void OnStart()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        /* Enabled MSAA */
        glfwWindowHint(GLFW_SAMPLES, 4);
        glEnable(GL_MULTISAMPLE);

        /* Hide mouse cursor */
        glfwSetInputMode(WindowHandler(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void OnUpdate()
    {
        MouseControl();
        KeyControl();
        MoveLight();

        main_scene.Render();
    }

    void OnDestroy()
    {
        delete mat1;
        delete mat2;
        delete mat3;
        delete mat4;
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
        /* Control main camera */
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

    void MoveLight()
    {
        if (!light2) {
            return;
        }
        light2->Translate(0.1 * sin(glfwGetTime() * 5), 0.0f, 0.1 * cos(glfwGetTime() * 5));
        light4->Translate(0.05 * cos(glfwGetTime() * 5), 0.0f, 0.0);
    }

};

int main()
{
    MainWindow main_window;
    main_window.Run();

    return 0;
}