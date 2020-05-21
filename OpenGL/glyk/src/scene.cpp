#include "glyk/scene.h"
#include "glyk/vec3.h"


Scene::RenderMeshFlag operator| (Scene::RenderMeshFlag f1, Scene::RenderMeshFlag f2)
{
    return static_cast<Scene::RenderMeshFlag>(static_cast<int>(f1) | static_cast<int>(f2));
}

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
    unsigned int dir_light_index = 0, pt_light_index = 0;

    for (it = light_list.begin(); it != light_list.end(); it++) {
        Light* light = *it;
        if (light->Type() == Light::Directional) {
            light->PipeUniformData(shader_id, dir_light_index++);
        } else if (light->Type() == Light::Point) {
            light->PipeUniformData(shader_id, pt_light_index++);
        }
    }
}

void Scene::RenderMesh(Mesh* m, RenderMeshFlag flag)
{
    GLuint shader_id = m->ShaderId();
    glUseProgram(shader_id);
    PrepareMesh(shader_id, m);
    if (flag & View) {
        PrepareCamera(shader_id);
    }
    if (flag & Lights) {
        PrepareLights(shader_id);
    }
    m->Render();
}

void Scene::Render()
{
    for (std::vector<Mesh*>::iterator it = mesh_list.begin(); it != mesh_list.end(); it++) {
        Mesh* m = *it;
        RenderMesh(m, View | Lights);
    }

    for (std::vector<Light*>::iterator it = light_list.begin(); it != light_list.end(); it++) {
        Light* l = *it;

        std::vector<Indicator*>::iterator ind_it;
        for (ind_it = l->indicators.begin(); ind_it != l->indicators.end(); ind_it++) {
            Indicator* ind = *ind_it;
            l->BindIndicator(ind);
            RenderMesh(ind->mesh, View);
        }
    }
}
