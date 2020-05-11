#pragma once
#include "Eigen/Dense"
#include "interaction.hpp"
#include "mathext.hpp"

class BRDF
{
public:
	BRDF()
	{
		isSpecular = false;
	}
	
	// Evaluate the BRDF
	// The information in @Interaction contains ray's direction, normal
	// and other information that you might need
	virtual Eigen::Vector3f eval(Interaction &_interact, float* pdf = nullptr) = 0;
	
	// Sample a direction based on the BRDF
	// The sampled direction is stored in @Interaction
	// The PDF of this direction is returned
	virtual float sample(Interaction &_interact) = 0;
	
	// Mark if the BRDF is specular
	bool isSpecular;
};


class IdealDiffuse : public BRDF
{
public:
	
	Eigen::Vector3f eval(Interaction& _interact, float* pdf = nullptr)
	{
		// TODO
		if (pdf) {
			float cos_theta_i = _interact.inputDir.dot(_interact.normal);
			float cos_theta_o = _interact.outputDir.dot(_interact.normal);
			*pdf = (cos_theta_i * cos_theta_o < 0) ? 0 : (abs(cos_theta_i) * PI_INV);
		}
		return _interact.surfaceColor * PI_INV;
	}

	float sample(Interaction& _interact)
	{
		// TODO
		Eigen::Vector2f d = sampler::disk(1.0f);
		float sin_theta = d[0], cos_theta = sqrt(1 - (sin_theta * sin_theta));
		float cos_phi = cos(d[1]), sin_phi = sin(d[1]);

		Eigen::Vector3f wi(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
		

		// Eigen::Vector2f sph_coords = sampler::cosine_weighted_hemisphere(1.0f);
		// Eigen::Vector3f cart_coords = transform::polar2cartesian(1.0f, sph_coords);
		Eigen::Matrix3f rot = transform::rot_align_normal(_interact.normal);
		_interact.inputDir = rot * wi;

		// std::cout << wi.transpose() << " --- ";

		// std::cout << "sphcoords: " << sph_coords.transpose() << " --- " << _interact.normal.transpose() << "-- " << pdf::cosine_weighted_hemisphere_w(sph_coords.x());
		// if (abs(cos_theta - _interact.inputDir.dot(_interact.normal)) > 0.0001)
		// {
		// 	std::cout << _interact.inputDir.dot(_interact.normal) << "!=" << cos_theta << std::endl;
		// }
		// if (abs(_interact.inputDir.norm() - 1) > 0.0001) {
		// 	std::cout << _interact.inputDir.norm() << std::endl;
		// }

		// return pdf::cosine_weighted_hemisphere_w(sph_coords.x());
	
		return abs(cos_theta * PI_INV);
	}
};

class IdealSpecular : public BRDF
{
public:

	IdealSpecular() {
		isSpecular = true;
	}

	Eigen::Vector3f eval(Interaction &_interact, float* pdf = nullptr)
	{
		// TODO
		return _interact.surfaceColor;
	};

	float sample(Interaction& _interact)
	{
		// TODO
		_interact.inputDir = -2 * _interact.normal.dot(_interact.outputDir) * _interact.normal + _interact.outputDir;

		return 1.0f;
	};
};