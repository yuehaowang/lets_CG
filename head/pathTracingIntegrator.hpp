#pragma once
#include "integrator.hpp"
#include "material.hpp"
#include "config.hpp"
#include <cmath>
#include <iostream>
#include <omp.h>

extern Config conf;


class PathTracingIntegrator : public Integrator
{
public:
	PathTracingIntegrator(Scene* scene, Camera* camera)
		: Integrator(scene, camera)
	{
		std::cout << "max depth: " << conf.max_depth << std::endl;
		std::cout << "number of samples: " << conf.num_samples << std::endl;
		std::cout << "light power: " << conf.light_power << std::endl;

		std::cout << "max number of threads: " << omp_get_max_threads() << std::endl;
		omp_set_num_threads(conf.num_threads);
		#pragma omp parallel
		{
			#pragma omp single
			{
				std::cout << "number of using threads: " << omp_get_num_threads() << std::endl;
			}
		}
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
				for (int t = 0; t < conf.num_samples; ++t) {
					L += radiance(ray);
				}
				camera->setPixel(dx, dy, L / conf.num_samples);
			}
		}
	}

	Eigen::Vector3f directLighting(Interaction& isect)
	{
		Eigen::Vector3f L(0, 0, 0);

		if (scene->lights.size() == 0) {
			return L;
		}

		Light* l = scene->lights[0];

		/**
		 * Sample light source
		 */
		{
			/* Sample a point from the area light and find its PDF w.r.t area */
			float light_pdf;
			Eigen::Vector3f light_pos = l->sampleSurfacePos(&light_pdf);
			/* Displacement of the incoming delta light */
			Eigen::Vector3f diff = light_pos - isect.entryPoint;
			/* Direction of the incoming light */
			Eigen::Vector3f wi = diff.normalized();
			isect.inputDir = wi;

			/* Test visibility */
			Interaction vis_isect;
			Ray light_ray = Ray(light_pos, -wi, DELTA, diff.norm() - DELTA);
			bool visibility = !scene->intersection(&light_ray, vis_isect);
			if (visibility) {
				/* Included angle between incoming light and the surface normal */
				float cos_theta_i = wi.dot(isect.normal);
				/* PDF transformation from area to solid angle */
				light_pdf *= diff.squaredNorm() / abs(cos_theta_i);
				/* Radiance emitted from the sampled light */
				Eigen::Vector3f Ld = l->emission(-wi) * abs(cos_theta_i);

				if (abs(light_pdf) > EPSILON && abs(cos_theta_i) > EPSILON) {
					/* Find BRDF and compute its PDF */
					BRDF* brdf = (BRDF*)isect.material;
					float brdf_pdf;
					Eigen::Vector3f f = brdf->eval(isect, &brdf_pdf);

					/* Compute the light contribution according to MIS */
					float w = mathext::power_heuristic(1, light_pdf, 1, brdf_pdf);
					L += f.cwiseProduct(Ld) * w / light_pdf;
				}
			}
		}


		/**
		 * Sample BRDF
		 */
		{
			BRDF* brdf = (BRDF*)isect.material;
			/* Sample BRDF to get a incoming light ray */
			float brdf_pdf = brdf->sample(isect);
			Eigen::Vector3f f = brdf->eval(isect);
			/* Check whether the incoming light ray hits the light source */
			Interaction light_isect;
			float light_hit_t = -1;
			Ray light_ray = Ray(isect.entryPoint, isect.inputDir, DELTA);
			if (abs(brdf_pdf) > EPSILON && l->isHit(&light_ray, &light_hit_t)) {
				/* Check sheltering of the light ray */
				Interaction vis_isect;
				bool visibility = !scene->intersection(&light_ray, vis_isect) || (vis_isect.entryDist >= light_hit_t);
				if (visibility) {
					/* Included angle between incoming light and the surface normal */
					float cos_theta_i = isect.inputDir.dot(isect.normal);
					/* Compute PDF of the incoming light w.r.t solid angle */
					float light_pdf = 0.0f;
					if (!brdf->isSpecular) {
						light_pdf = l->sampleSurfacePdf() * light_hit_t / abs(cos_theta_i);
					}
					/* Radiance of the incoming light */
					Eigen::Vector3f Ld = l->emission(-isect.inputDir) * abs(cos_theta_i);
					/* Compute the light contribution according to the MIS */
					float w = mathext::power_heuristic(1, brdf_pdf, 1, light_pdf);
					L += f.cwiseProduct(Ld) * w / brdf_pdf;
				}
			}
		}
		
		return L;
	}

	Eigen::Vector3f radiance(Ray ray) override
	{
		Eigen::Vector3f L(0, 0, 0);
		Eigen::Vector3f beta(1.0f, 1.0f, 1.0f);
		bool specularBounce = false;
    	for (int bounces = 0; bounces < conf.max_depth; ++bounces) {
			Interaction isect;
			bool found_intersection = scene->intersection(&ray, isect);
			if (!found_intersection) {
				break;
			}

			if ((bounces == 0 || specularBounce) && isect.lightId >= 0) {
				/* Handle light sources */
				L += beta.cwiseProduct(scene->lights[isect.lightId]->emission(ray.m_Dir));
			}

			/* Ignore reflected rays hitting the light source */
			if (isect.lightId >= 0) {
				break;
			}

			isect.outputDir = -ray.m_Dir;

			/* Perform direct lighting */
			L += beta.cwiseProduct(directLighting(isect));
	
			/* Sample BRDF to get the next path */
			BRDF* brdf = (BRDF*)isect.material;
			float brdf_pdf = brdf->sample(isect);
			if (abs(brdf_pdf) < EPSILON) {
				break;
			}
			specularBounce = brdf->isSpecular;
			beta = beta.cwiseProduct(brdf->eval(isect)) * abs(isect.inputDir.dot(isect.normal)) / brdf_pdf;
			ray = Ray(isect.entryPoint, isect.inputDir, DELTA);
		}

		return L;
	}
};	



