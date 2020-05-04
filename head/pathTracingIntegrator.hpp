#pragma once
#include "integrator.hpp"
#include "material.hpp"
#include <cmath>
// #include <omp.h>

class PathTracingIntegrator : public Integrator
{
public:
	PathTracingIntegrator(Scene* scene, Camera* camera)
		: Integrator(scene, camera)
	{
	}

	// main render loop
	void render() override
	{
		// TODO: Generate ray and trace here
		// Don't forget to call @Camera.setPixel() at the end
	}

	// radiance of a specific point
	Eigen::Vector3f radiance(Interaction * interaction, Ray * ray) override
	{
		// TODO: Calculate color here
	}
};	



