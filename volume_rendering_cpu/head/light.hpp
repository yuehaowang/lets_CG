#pragma once
#include "ray.hpp"
#include "sampler.hpp"
#include <Eigen/Dense>
#include <cmath>
#include <utility>

class Light
{
public:
    enum LightType
    {
        None = 0,
        Point,
        Area
    };
    Light(Eigen::Vector3f vector3_f, Eigen::Vector3f vector3_f1, LightType t)
        : m_Pos(vector3_f), m_Color(vector3_f1), m_Type(t){};
    virtual Eigen::Vector3f Sample(Sampler *sampler, Eigen::Vector3f &sampled_lightPos, float &pdf) = 0;
    virtual bool hitted(Ray *ray) = 0;
    Eigen::Vector3f m_Pos;
    Eigen::Vector3f m_Color;
    LightType m_Type;
};

class PointLight : public Light
{
public:
    float m_K_quadric;
    float m_K_linear;
    float m_K_constant;

    PointLight(Eigen::Vector3f pos, Eigen::Vector3f color, float quadratic = 0.0f, float linear = 0.0f, float constant = 1.0f)
        : Light(pos, color, Point), m_K_quadric(quadratic), m_K_linear(linear), m_K_constant(constant){};
    Eigen::Vector3f Sample(Sampler *sampler, Eigen::Vector3f &sampled_lightPos, float &pdf)
    {
        sampled_lightPos = m_Pos;
        pdf = 1;
        return m_Color;
    }
    bool hitted(Ray *ray)
    {
        const float threshold_angle_diff = 2.0f / 180.0f * 3.1415926f;
        return (m_Pos - ray->m_Ori).normalized().dot(ray->m_Dir) >= std::cos(threshold_angle_diff);
    }
};

class AreaCircleLight : public Light
{
public:
    AreaCircleLight(Eigen::Vector3f vector3_f, Eigen::Vector3f vector3_f1) : Light(vector3_f, vector3_f1, Area){};
    float radius = 1.0f;
    Eigen::Vector3f normal = Eigen::Vector3f(0, -1, 0);
    Eigen::Vector3f Sample(Sampler *sampler, Eigen::Vector3f &sampled_lightPos, float &pdf)
    {
        float r = std::sqrt(sampler->next()) * this->radius;
        float theta = sampler->next() * 2 * 3.1415926f;
        sampled_lightPos = m_Pos + Eigen::Vector3f(r * std::cos(theta), 0, r * std::sin(theta));
        pdf = 1.0f / (3.1415926f * this->radius * this->radius);
        return m_Color;
    }
    bool hitted(Ray *ray)
    {
        float t = normal.dot(m_Pos - ray->m_Ori) / (normal.dot(ray->m_Dir));
        Eigen::Vector3f hitted_p = ray->m_Ori + t * ray->m_Dir.normalized();
        float d = (hitted_p - m_Pos).norm();
        return ((t > 0) && (d <= this->radius));
    }
};