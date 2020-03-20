#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include "gl_header_files.h"
#include "camera.h"
#include "mesh.h"
#include "light.h"


class Scene
{

protected:

    std::vector<Mesh*> mesh_list;
    std::vector<Light*> light_list;
    Camera* main_camera;
    void PrepareCamera(Mesh* m);
    void PrepareLights(Mesh* m);

public:

    Scene();
    virtual ~Scene();
    void Add(Mesh* obj);
    void Add(Light* light);
    void SetMainCamera(Camera* cam);
    void Render();

};


#endif
