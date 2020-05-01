#pragma once
#include "scene.hpp"
#include "camera.hpp"
#include "interaction.hpp"

class Integrator
{
public:
	Scene* scene;
	Camera* camera;
	
	Integrator(Scene* scene, Camera* camera) : scene(scene), camera(camera) {}
	virtual ~Integrator() = default;
	
	virtual void render() = 0;
	virtual Eigen::Vector3f radiance(Interaction * interaction, Ray* ray) = 0;
};
