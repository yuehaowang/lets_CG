#pragma once
#include "integrator.hpp"
#include "material.hpp"
#include <cmath>
#include <iostream>
#include <omp.h>

#define MAX_DEPTH 5
#define NUM_SAMPLES 30


class PathTracingIntegrator : public Integrator
{
public:
	PathTracingIntegrator(Scene* scene, Camera* camera)
		: Integrator(scene, camera)
	{
		int n_threads = omp_get_max_threads();
		omp_set_num_threads(n_threads);
		std::cout << "number of threads: " << n_threads << std::endl;

		std::cout << "max depth: " << MAX_DEPTH << std::endl;
		std::cout << "number of samples: " << NUM_SAMPLES << std::endl;
	}

	void render() override
	{
		int dx, dy;
		int w = camera->m_Film.m_Res.x(), h = camera->m_Film.m_Res.y();

		#pragma omp parallel for private(dy)
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
		Eigen::Vector3f diff = light_pos - isect.entryPoint;
		/* Direction of the incoming light */
		Eigen::Vector3f wi = diff.normalized();

		/* Test visibility */
		Interaction vis_isect;
		Ray light_ray = Ray(light_pos, -wi, DELTA, diff.norm() - DELTA);
		bool visibility = !scene->intersection(&light_ray, vis_isect) && l->isEmissive(wi);
		if (visibility) {
			/* Included angle between incoming light and the surface normal */
			float cos_theta_i = wi.dot(isect.normal);
			/* PDF transformation from area to solid angle */
			light_pdf *= diff.squaredNorm() / cos_theta_i;
			/* Perform geometric term on the incoming radiance */
			Ld *= abs(cos_theta_i);

			if (abs(light_pdf) > EPSILON && abs(cos_theta_i) > DELTA) {
				BRDF* brdf = (BRDF*)isect.material;
				float brdf_pdf;
				Eigen::Vector3f f = brdf->eval(isect, &brdf_pdf);

				/* Add the delta light contribution according to the PDF */
				L += f.cwiseProduct(Ld) / (light_pdf);
			}

		// if ((isect.entryPoint - light_pos).norm() > 1 && PI * L.x() >= 0.7 && PI * L.y() >= 0.7 && PI * L.z() >= 0.7 && isect.normal.y() == -1.0f) {
		// 	std::cout << wi.transpose() << " -- " << cos_theta_i << " - " << diff.squaredNorm() << "--" << light_pdf << std::endl;
		// }

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

			if (isect.lightId >= 0) {
				break;
			}

			/* Perform direct lighting */
			L += beta.cwiseProduct(uniformSampleLight(isect));
	
			/* Sample BRDF to get the next path */
			isect.outputDir = -ray.m_Dir;
			BRDF* brdf = (BRDF*)isect.material;
			float brdf_pdf = brdf->sample(isect);
			if (abs(brdf_pdf) < EPSILON) {
				break;
			}
			// std::cout << beta.transpose() << " | ";
			beta = beta.cwiseProduct(brdf->eval(isect)) * abs(isect.inputDir.dot(isect.normal)) / brdf_pdf;
			// std::cout << beta.transpose() << " " << brdf->eval(isect).transpose() << " " << abs(isect.outputDir.dot(isect.normal)) << " " << brdf_pdf << std::endl;
			ray = Ray(isect.entryPoint, isect.inputDir, DELTA);
			// std::cout << isect.inputDir.transpose() << " --- " << isect.normal.transpose() << " --- " << brdf_pdf << std::endl;
		}

		return L;
	}
};	



