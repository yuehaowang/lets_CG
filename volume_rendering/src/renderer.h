#pragma once
#include "camera.hpp"
#include "classifier.h"
#include "compositor.h"
#include "interpolator.h"
#include "light.hpp"
#include "opticsData.h"
#include "volume.h"
#include <iostream>
#include <omp.h>

class Renderer
{
private:
    opticsData getOpticsData(const Ray &ray, float t, float dt);

public:
    Camera *camera = NULL;
    std::vector<Light *> lights;
    Volume *volume = NULL;
    Classifier *classifier = NULL;
    Interpolator *interpolator = NULL;
    void setCamera(Camera *cam);
    void addLight(Light *light);
    void setVolume(Volume *vol);
    void setClassifier(Classifier *classifier);
    void setInterpolator(Interpolator *interpolator);

    void renderFrontToBack(std::string imgPath);
    void renderBackToFront(std::string imgPath);
};
