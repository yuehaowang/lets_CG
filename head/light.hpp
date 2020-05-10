#pragma once
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <utility>
#include <cmath>
#include "ray.hpp"
#include "interaction.hpp"
#include "mathext.hpp"


class Light
{
public:
	Light(Eigen::Vector3f pos, Eigen::Vector3f color)
		: m_Pos(std::move(pos)),
		m_Color(std::move(color))
	{
	}
	virtual ~Light() = default;

	// Sample a point on the light's surface, if light is not delta light
	// Set PDF and the surface position
	// Return color of light
	virtual Eigen::Vector3f SampleSurfacePos(Eigen::Vector3f& sampled_lightPos, float* pdf = nullptr) = 0;

	// Determine if light is hit, if light is not delta light
	virtual bool isHit(Ray* ray) = 0;

	Eigen::Vector3f emission()
	{
		return m_Color;
	}
	
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
		Eigen::Vector3f r = Eigen::Vector3f(1.0f, 0.0f, 0.0f),
		Eigen::Vector3f n = Eigen::Vector3f(0.0f, -1.0f, 0.0f))
	: Light(pos, color), radius(r.norm()), normal(n.normalized()), right(r.normalized())
	{
		
	}
	
	Eigen::Vector3f SampleSurfacePos(Eigen::Vector3f& sampled_lightPos, float* pdf = nullptr) override
	{
		Eigen::Vector2f sample = sampler::disk(radius);
		/* Convert polar to cartesian coordinate */	
		sampled_lightPos = m_Pos + sample.x() * (Eigen::AngleAxisf(sample.y(), normal) * right);

		if (pdf) {
			*pdf = pdf::disk_xy(radius);
		}

		return emission();
	}
	
	/* 
	 * Reference: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
	 */
	bool isHit(Ray * ray) override
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
		return (isect - m_Pos).norm() <= radius;
	}
};