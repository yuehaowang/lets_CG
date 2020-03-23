#include "glyk/scene.h"
#include "glyk/vec3.h"


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

void Scene::PrepareMesh(GLuint shader_id, Mesh* mesh)
{
    if (!mesh) {
        return;
    }
    mesh->PipeUniformData(shader_id);
}

void Scene::PrepareCamera(GLuint shader_id)
{
    if (!main_camera) {
        return;
    }
    main_camera->PipeUniformData(shader_id);
}

void Scene::PrepareLights(GLuint shader_id)
{
    std::vector<Light*>::iterator it;
    unsigned int light_index = 0;

    for (it = light_list.begin(); it != light_list.end(); it++, light_index++) {
        Light* light = *it;
        light->PipeUniformData(shader_id, light_index);
    }
}

void Scene::Render()
{
    for (std::vector<Mesh*>::iterator it = mesh_list.begin(); it != mesh_list.end(); it++) {
        Mesh* m = *it;
        GLuint shader_id = m->ShaderId();
        glUseProgram(shader_id);
        PrepareMesh(shader_id, m);
        PrepareCamera(shader_id);
        PrepareLights(shader_id);
        m->Render();
    }
}
