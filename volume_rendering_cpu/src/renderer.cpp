#include "renderer.h"
#include "config.hpp"

extern Config conf;

void Renderer::setCamera(Camera *cam) { this->camera = cam; };

void Renderer::addLight(Light *light) { this->lights.push_back(light); };

void Renderer::setVolume(Volume *vol) { this->volume = vol; };

void Renderer::setClassifier(Classifier *classifier) { this->classifier = classifier; };

void Renderer::setInterpolator(Interpolator *interpolator) { this->interpolator = interpolator; };

opticsData Renderer::getOpticsData(const Ray &ray, float t, float dt)
{
    /* Get position of the sampled point */
    Eigen::Vector3f vo_p = ray.getPoint(t);
    /* Retrieve located voxel */
    voxel vo = this->volume->getVoxel(vo_p);
    /* Interpolate voxel data by its located voxel */
    volumeData vol_d = this->interpolator->interpolate(vo_p, vo);
    /* Get optical data by transfer function */
    opticsData opt_d = this->classifier->transfer(vol_d, dt, this->camera, this->lights, this->volume->grad_maxnorm);

    return opt_d;
};

void Renderer::renderFrontToBack(std::string imgPath)
{
    float dt = conf.sample_step;
#pragma omp parallel for
    for (int i = 0; i < camera->m_Film.m_Res.x() * camera->m_Film.m_Res.y(); i++)
    {
        int dy = i / camera->m_Film.m_Res.x();
        int dx = i - dy * camera->m_Film.m_Res.x();
        Eigen::Vector3f color(0, 0, 0);
        Eigen::Vector3f alpha(0, 0, 0);
        Ray ray = camera->generateRay((float)dx, (float)dy);
        float t_start, t_end;
        /* Integration calculation */
        if (this->volume->getRayStartEnd(ray, t_start, t_end))
        {
            float t = t_start;
            while (t <= t_end)
            {
                /* Get optical data at position t */
                opticsData opt_d = getOpticsData(ray, t, dt);
                /* Composition */
                Compositor::compositeFrontToBack(color, alpha, opt_d.getColor(), opt_d.getOpacity());

                t += dt;
            }
        }
        camera->setPixel(dx, dy, color);
    }
    camera->m_Film.write(imgPath);
};

void Renderer::renderBackToFront(std::string imgPath)
{
    float dt = conf.sample_step;
#pragma omp parallel for
    for (int i = 0; i < camera->m_Film.m_Res.x() * camera->m_Film.m_Res.y(); i++)
    {
        int dy = i / camera->m_Film.m_Res.x();
        int dx = i - dy * camera->m_Film.m_Res.x();
        Eigen::Vector3f color(0, 0, 0);
        Ray ray = camera->generateRay((float)dx, (float)dy);
        float t_start, t_end;
        /* Integration calculation */
        if (this->volume->getRayStartEnd(ray, t_start, t_end))
        {
            float t = t_end;
            while (t >= t_start)
            {
                /* Get optical data at position t */
                opticsData opt_d = getOpticsData(ray, t, dt);
                /* Composition */
                Compositor::compositeBackToFront(color, opt_d.getColor(), opt_d.getOpacity());

                t -= dt;
            }
        }
        camera->setPixel(dx, dy, color);
    }
    camera->m_Film.write(imgPath);
};