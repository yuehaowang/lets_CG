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
			*pdf = (cos_theta_i * cos_theta_o < 0) ? 0 : (abs(cos_theta_i) / PI);
		}
		return _interact.surfaceColor;
	}

	float sample(Interaction& _interact)
	{
		// TODO
		Eigen::Vector2f sph_coords = sampler::cosine_weighted_hemisphere(1.0f);
		Eigen::Vector3f cart_coords = transform::polar2cartesian(1.0f, sph_coords);
		Eigen::Matrix3f rot = transform::rot_align_normal(_interact.normal);
		_interact.inputDir = -(rot * cart_coords);

		// std::cout << "sphcoords: " << sph_coords.transpose() << " --- ";

		return pdf::cosine_weighted_hemisphere_w(sph_coords.x());
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