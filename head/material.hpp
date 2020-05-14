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
	
	/* Evaluate the BRDF
	 * The information in @Interaction contains ray's direction, normal
	 * and other information that you might need
	 */
	virtual Eigen::Vector3f eval(Interaction &_interact, float* pdf = nullptr) = 0;
	
	/* Sample a direction based on the BRDF
	 * The sampled direction is stored in @Interaction
	 * The PDF of this direction is returned
	 */
	virtual float sample(Interaction &_interact) = 0;
	
	/* Mark if the BRDF is specular */
	bool isSpecular;
};


class IdealDiffuse : public BRDF
{
public:
	
	Eigen::Vector3f eval(Interaction& _interact, float* pdf = nullptr)
	{
		if (pdf) {
			float cos_theta_i = _interact.inputDir.dot(_interact.normal);
			float cos_theta_o = _interact.outputDir.dot(_interact.normal);
			*pdf = abs(cos_theta_i) * PI_INV;
		}
		return _interact.surfaceColor * PI_INV;
	}

	float sample(Interaction& _interact)
	{
		Eigen::Vector2f d = mathext::disk(1.0f);
		float sin_theta = d[0], cos_theta = sqrt(1 - (sin_theta * sin_theta));
		float cos_phi = cos(d[1]), sin_phi = sin(d[1]);

		Eigen::Vector3f wi(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
		Eigen::Matrix3f rot = mathext::rot_align_normal(_interact.normal);
		_interact.inputDir = rot * wi;
	
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
		bool flag_reflect = false;
		Eigen::Vector3f v2 = mathext::reflect(_interact.outputDir, _interact.normal);
		if ((v2 - _interact.inputDir).norm() < DELTA) {
			flag_reflect = true;
		}
		if (pdf) {
			*pdf = flag_reflect ? 1.0f : 0.0f;
		}

		return flag_reflect ? _interact.surfaceColor : Eigen::Vector3f(0, 0, 0);
	};

	float sample(Interaction& _interact)
	{
		_interact.inputDir = mathext::reflect(_interact.outputDir, _interact.normal);

		return 1.0f;
	};
};