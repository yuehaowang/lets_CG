#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include "glyk/gl_header_files.h"
#include "glyk/camera.h"
#include "glyk/mesh.h"
#include "glyk/light.h"


class Scene
{

protected:

    enum RenderMeshFlag {
        None = 0x00000000,
        Lights = 0x00000001,
        View = 0x00000002
    };

    std::vector<Mesh*> mesh_list;
    std::vector<Light*> light_list;
    Camera* main_camera;
    void PrepareMesh(GLuint shader_id, Mesh* m);
    void PrepareCamera(GLuint shader_id);
    void PrepareLights(GLuint shader_id);
    void RenderMesh(Mesh* m, RenderMeshFlag flag = None);

    friend RenderMeshFlag operator| (RenderMeshFlag f1, RenderMeshFlag f2);

public:

    Scene();
    virtual ~Scene();
    void Add(Mesh* obj);
    void Add(Light* light);
    void SetMainCamera(Camera* cam);
    void Render();

};

Scene::RenderMeshFlag operator| (Scene::RenderMeshFlag f1, Scene::RenderMeshFlag f2);


#endif
