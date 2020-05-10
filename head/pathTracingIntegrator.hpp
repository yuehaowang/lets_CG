#pragma once
#include "integrator.hpp"
#include "material.hpp"
#include <cmath>
#include <omp.h>

#define MAX_DEPTH 5
#define NUM_SAMPLES 20

class PathTracingIntegrator : public Integrator
{
public:
	PathTracingIntegrator(Scene* scene, Camera* camera)
		: Integrator(scene, camera)
	{
	}

	void render() override
	{
		int dx, dy;
		int w = camera->m_Film.m_Res.x(), h = camera->m_Film.m_Res.y();
		#pragma omp parallel
		{
			#pragma omp parallel for
			for (dx = 0; dx < w; dx++)
			{
				for (dy = 0; dy < h; dy++)
				{
					/* Generate a ray from the camera to a pixel */
					Ray ray = camera->generateRay(dx, dy);
					/* Sample multiple times for a pixel */
					Eigen::Vector3f L(0, 0, 0);
					for (int t = 0; t < NUM_SAMPLES; ++t) {
						L += radiance(ray);
					}
					camera->setPixel(dx, dy, L / NUM_SAMPLES);
				}
			}
		}
	}

	Eigen::Vector3f uniformSampleLight(Interaction& isect)
	{
		Eigen::Vector3f L(0, 0, 0);

		if (scene->lights.size() == 0) {
			return L;
		}

		/* Sample an area light */
		Light* l = scene->lights[0];
		/* Sample a delta light from the area light and find its PDF */
		float light_pdf;
		Eigen::Vector3f light_pos;
		Eigen::Vector3f Ld = l->SampleSurfacePos(light_pos, &light_pdf);
		/* Displacement of the incoming delta light */
		Eigen::Vector3f diff = isect.entryPoint - light_pos;
		/* Direction of the incoming light */
		Eigen::Vector3f wi = diff.normalized();

		/* Test visibility */
		Interaction vis_isect;
		Ray light_ray = Ray(light_pos, wi, DELTA, diff.norm() - DELTA);
		bool visibility = !scene->intersection(&light_ray, vis_isect);
		if (visibility) {
			/* Included angle between incoming light and the surface normal */
			float cos_theta_i = (-wi).dot(isect.normal);
			/* PDF transformation from area to solid angle */
			light_pdf *= diff.squaredNorm() / cos_theta_i;
			/* Perform geometric term on the incoming radiance */
			Ld *= abs(cos_theta_i);

			BRDF* brdf = (BRDF*)isect.material;
			float brdf_pdf;
			Eigen::Vector3f f = brdf->eval(isect, &brdf_pdf);

			/* Add the delta light contribution according to the PDF */
			L += f.cwiseProduct(Ld) / (light_pdf);
		}

		return L;
	}

	Eigen::Vector3f radiance(Ray ray) override
	{
		Eigen::Vector3f L(0, 0, 0);
		Eigen::Vector3f beta(1.0f, 1.0f, 1.0f);
		bool specularBounce = false;
    	for (int bounces = 0; bounces < MAX_DEPTH; ++bounces) {
			Interaction isect;
			bool found_intersection = scene->intersection(&ray, isect);
			if (!found_intersection) {
				break;
			}

			if (bounces == 0) {
				/* Handle light sources */
				if (isect.lightId >= 0) {
					L += beta.cwiseProduct(scene->lights[isect.lightId]->emission());
				}
			} else {
				// std::cout << isect.surfaceColor.transpose() << std::endl;
			}

			/* Perform direct lighting */
			L += beta.cwiseProduct(uniformSampleLight(isect));

			/* Sample BRDF to get the next path */
			isect.outputDir = -ray.m_Dir;
			BRDF* brdf = (BRDF*)isect.material;
			float brdf_pdf = brdf->sample(isect);
			beta = beta.cwiseProduct(brdf->eval(isect)) * abs((-isect.inputDir).dot(isect.normal)) / brdf_pdf;
			ray = Ray(isect.entryPoint, -isect.inputDir);
			// std::cout << -isect.inputDir.transpose() << " --- " << isect.normal.transpose() << " --- " << brdf_pdf << std::endl;
		}

		return L;
	}
};	



