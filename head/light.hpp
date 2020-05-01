#pragma once
#include <Eigen/Dense>
#include <utility>
#include <cmath>
#include "ray.hpp"
#include "interaction.hpp"


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
	virtual Eigen::Vector3f SampleSurfacePos(Eigen::Vector3f& sampled_lightPos, float& pdf)=0;

	// Determine if light is hit, if light is not delta light
	virtual bool isHit(Ray* ray) = 0;
	
	Eigen::Vector3f m_Pos;
	Eigen::Vector3f m_Color;
};

class AreaLight:public Light
{
public:
	AreaLight(Eigen::Vector3f pos, Eigen::Vector3f color) : Light(pos, color)
	{
	}
	
	Eigen::Vector3f SampleSurfacePos(Eigen::Vector3f& sampled_lightPos, float & pdf) override {
		// TODO
	};
	
	bool isHit(Ray * ray) override {
		// TODO
	};
};