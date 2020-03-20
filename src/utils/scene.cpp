#include "scene.h"
#include "vec3.h"

Scene::Scene()
: main_camera(NULL)
{

}

Scene::~Scene()
{
    for (std::vector<Mesh*>::iterator it = mesh_list.begin(); it != mesh_list.end(); it++) {
        delete *it;
    }
    mesh_list.clear();

    for (std::vector<Light*>::iterator it = light_list.begin(); it != light_list.end(); it++) {
        delete *it;
    }
    light_list.clear();
}

void Scene::Add(Mesh* obj)
{
    mesh_list.push_back(obj);
}

void Scene::Add(Light* light)
{
    light_list.push_back(light);
}

void Scene::SetMainCamera(Camera* cam)
{
    if (!cam) {
        return;
    }

    main_camera = cam;
}

void Scene::PrepareCamera(Mesh* m)
{
    if (!main_camera) {
        return;
    }

    glUniformMatrix4fv(
        glGetUniformLocation(m->shader.Id(), "transform.view"), 1,
        GL_TRUE, main_camera->View()->Ptr()
    );
    glUniformMatrix4fv(
        glGetUniformLocation(m->shader.Id(), "transform.projection"), 1,
        GL_TRUE, main_camera->Projection()->Ptr()
    );

    Vec3<GLfloat> cam_pos = main_camera->Position();
    glUniform3f(
        glGetUniformLocation(m->shader.Id(), "camera_pos"),
        cam_pos.x, cam_pos.y, cam_pos.z
    );
}

void Scene::PrepareLights(Mesh* m)
{
    GLuint shader_id = m->shader.Id();

    std::vector<Light*>::iterator it;
    unsigned int light_index = 0;

    for (it = light_list.begin(); it != light_list.end(); it++, light_index++) {
        Light* light = *it;
        light->PipeData(shader_id, light_index);
    }
}

void Scene::Render()
{
    
    for (std::vector<Mesh*>::iterator it = mesh_list.begin(); it != mesh_list.end(); it++) {
        Mesh* m = *it;
        m->PrepareRender();
        PrepareCamera(m);
        PrepareLights(m);
        m->Render();
    }
}
