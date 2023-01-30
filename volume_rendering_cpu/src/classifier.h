#pragma once
#include "camera.hpp"
#include "config.hpp"
#include "opticsData.h"
#define TINYCOLORMAP_WITH_EIGEN
#include "tinycolormap.hpp"
#include "volume.h"
#include <iostream>

#define PI 3.14159265358979323846f
#define AMBIENT_SCALE 0.1f;
#define SPECULAR_SHININESS 32.0f
#define PHASE_FUNC_G 0.0f

extern Config conf;

class Classifier
{
public:
    virtual opticsData transfer(volumeData v_data, float dt, Camera *cam, std::vector<Light *> lights, float grad_max_norm = 1) = 0;
};

class myClassifier : public Classifier
{
public:
    static float phaseFunctionHenyeyGreenstein(float t, float g)
    {
        return (1 / (4 * PI)) * ((1 - g * g) / powf(1 + g * g - 2 * g * t, 1.5));
    }

    opticsData transfer(volumeData v_data, float dt, Camera *cam, std::vector<Light *> lights, float grad_max_norm = 1)
    {
        opticsData optics;
        float den = v_data.density;

        /* Retrieve color from a colormap. Reference: https://github.com/yuki-koyama/tinycolormap */
        Eigen::Vector3f K_m = tinycolormap::GetColor(den, conf.colormap).ConvertToEigen().cast<float>();
        /* Compute view-in direction */
        Eigen::Vector3f view_dir = (cam->m_Pos - v_data.position).normalized();
        /* Compute normal */
        Eigen::Vector3f normal = v_data.gradient.normalized();
        /* Phong shading for each light */
        Eigen::Vector3f q(0, 0, 0);
        for (auto li : lights)
        {
            /* Ambient */
            Eigen::Vector3f C_a = li->m_Color * AMBIENT_SCALE;

            /* Compute light direction, light path distance, etc. */
            Eigen::Vector3f pos_diff = li->m_Pos - v_data.position;
            float li_dist = pos_diff.norm();
            Eigen::Vector3f li_dir = pos_diff / li_dist;

            /* Diffusion */
            float diff = std::max(normal.dot(li_dir), 0.0f);
            Eigen::Vector3f C_d = diff * li->m_Color;

            /* Specular */
            Eigen::Vector3f halfway_dir = (view_dir + li_dir).normalized();
            float spec = pow(std::max(normal.dot(halfway_dir), 0.0f), SPECULAR_SHININESS);
            Eigen::Vector3f C_s = spec * li->m_Color;

            /* Compute attenuation for point lights */
            float attenuation = 1.0f;
            if (li->m_Type == Light::Point)
            {
                PointLight *pt_li = (PointLight *)li;
                attenuation = 1.0f / (pt_li->m_K_constant + pt_li->m_K_linear * li_dist + pt_li->m_K_quadric * (li_dist * li_dist));
            }

            /* Emission */
            q += (C_a + C_d + C_s).cwiseProduct(K_m) * attenuation;

            if (!conf.disable_inscattering)
            {
                /* In-scattering */
                q += phaseFunctionHenyeyGreenstein(-view_dir.dot(li_dir), PHASE_FUNC_G) * li->m_Color;
            }
        }

        optics.transparency = Eigen::Vector3f::Ones() * exp(-den * dt);
        optics.color = q * den * dt;

        return optics;
    }
};