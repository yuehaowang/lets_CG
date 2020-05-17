#pragma once
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <utility>
#include <cmath>
#include "ray.hpp"
#include "interaction.hpp"
#include "mathext.hpp"
#include "config.hpp"


extern Config conf;


class Light
{
public:
    Light(Eigen::Vector3f pos, Eigen::Vector3f color)
        : m_Pos(std::move(pos)),
        m_Color(std::move(color))
    {
    }
    virtual ~Light() = default;

    /* Sample a point on the light's surface, if light is not delta light
     * Set PDF and the surface position
     * Return color of light
     */
    virtual Eigen::Vector3f sampleSurfacePos(float* pdf = nullptr) = 0;

    virtual float sampleSurfacePdf() = 0;

    virtual bool isHit(Ray* ray, float* hit_t = nullptr) = 0;

    virtual Eigen::Vector3f emission(Eigen::Vector3f d) = 0;
    
    Eigen::Vector3f m_Pos;
    Eigen::Vector3f m_Color;
};

class AreaLight:public Light
{
public:
    float radius;
    Eigen::Vector3f normal;
    Eigen::Vector3f right;

    AreaLight(Eigen::Vector3f pos, Eigen::Vector3f color,
        float r = 1.0f, Eigen::Vector3f n = Eigen::Vector3f(0.0f, -1.0f, 0.0f))
    : Light(pos, color), radius(r), normal(n.normalized())
    {
        right = mathext::rot_align_normal(normal) * Eigen::Vector3f(1.0f, 0.0f, 0.0f);
    }

    float sampleSurfacePdf()
    {
        return PI_INV / (radius * radius);
    }

    Eigen::Vector3f sampleSurfacePos(float* pdf = nullptr) override
    {
        Eigen::Vector2f sample = mathext::disk(radius);
        /* Convert polar to cartesian coordinate */	
        Eigen::Vector3f sampled_light_pos = m_Pos + (Eigen::AngleAxisf(sample.y(), normal) * (sample.x() * right));

        /* Compute PDF of sampling a point in the area light */
        if (pdf) {
            *pdf = sampleSurfacePdf();
        }

        return sampled_light_pos;
    }

    Eigen::Vector3f emission(Eigen::Vector3f d) override
    {
        return conf.light_power * m_Color * abs(d.dot(normal));
    }
    
    /* 
     * Reference: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
     */
    bool isHit(Ray* ray, float* hit_t = nullptr) override
    {
        float d = ray->m_Dir.dot(normal);

        if (abs(d) < EPSILON) {
            return false;
        }

        float t = (m_Pos - ray->m_Ori).dot(normal) / d;
        if (t < 0) {
            return false;
        }

        Eigen::Vector3f isect = ray->getPoint(t);
        bool is_hit = (isect - m_Pos).norm() <= radius;

        if (is_hit && hit_t) {
            *hit_t = t;
        }

        return is_hit;
    }
};