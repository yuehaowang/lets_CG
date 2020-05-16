#pragma once
#include "Eigen/Dense"
#include "interaction.hpp"
#include "mathext.hpp"

class BRDF
{
public:
	enum Flag {
		REFLECTION_DIFFUSE = 0x00000001,
		REFLECTION_SEPULAR = 0x00000010,
		REFRACTION = 0x00000100
	};

	BRDF(Flag f)
	{
		flag = f;
		isDelta = (flag & REFLECTION_SEPULAR) || (flag & REFRACTION);
	}
	
	/* Evaluate the BRDF
	 * The information in @Interaction contains ray's direction, normal
	 * and other information that you might need
	 */
	virtual Eigen::Vector3f eval(Interaction& _interact, float* pdf = nullptr) = 0;
	
	/* Sample a direction based on the BRDF
	 * The sampled direction is stored in @Interaction
	 * The PDF of this direction is returned
	 */
	virtual float sample(Interaction &_interact) = 0;
	
	/* Mark if the BRDF is specular */
	bool isDelta;

	Flag flag;
};

BRDF::Flag operator| (BRDF::Flag f1, BRDF::Flag f2)
{
    return static_cast<BRDF::Flag>(static_cast<int>(f1) | static_cast<int>(f2));
}


class IdealDiffuse : public BRDF
{
public:

	IdealDiffuse()
	: BRDF(REFLECTION_DIFFUSE)
	{

	}
	
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

	IdealSpecular()
	: BRDF(REFLECTION_SEPULAR)
	{

	}

	Eigen::Vector3f eval(Interaction& _interact, float* pdf = nullptr)
	{
		bool is_reflect = false;
		Eigen::Vector3f v2 = mathext::reflect(_interact.outputDir, _interact.normal);
		if ((v2 - _interact.inputDir).norm() < DELTA) {
			is_reflect = true;
		}
		if (pdf) {
			*pdf = is_reflect ? 1.0f : 0.0f;
		}

		return is_reflect ? _interact.surfaceColor : Eigen::Vector3f(0, 0, 0);
	};

	float sample(Interaction& _interact)
	{
		_interact.inputDir = mathext::reflect(_interact.outputDir, _interact.normal);

		return 1.0f;
	}
};

#define IOR_DIAMOND 2.417f
#define IOR_AMBER	1.550f
#define IOR_WATER   1.333f
#define IOR_GLASS   1.520f
#define IOR_VACUUM  1.000f

class IdealRefraction : public BRDF
{
public:

	float ior;

	IdealRefraction(float idx_refract = 1.0f)
	: BRDF(REFRACTION)
	, ior(idx_refract)
	{

	}

	Eigen::Vector3f eval(Interaction& _interact, float* pdf = nullptr)
	{
		bool is_refract = false;
		Eigen::Vector3f v2 = mathext::refract(_interact.outputDir, _interact.normal, ior);
		if ((v2 - _interact.inputDir).norm() < DELTA) {
			is_refract = true;
		}
		if (pdf) {
			*pdf = is_refract ? 1.0f : 0.0f;
		}

		return is_refract ? _interact.surfaceColor : Eigen::Vector3f(0, 0, 0);
	}

	float sample(Interaction& _interact)
	{
		_interact.inputDir = mathext::refract(_interact.outputDir, _interact.normal, ior);

		return 1.0f;
	}
};


class FresnelBlend : public BRDF
{
public:

	float index_refraction;

	IdealSpecular specular_brdf;
	IdealRefraction refract_brdf;

	FresnelBlend(float ior = 1.0f)
	: BRDF(REFLECTION_SEPULAR | REFRACTION)
	, refract_brdf(ior)
	{
	
	}

	float fresnel_R_eff(const Eigen::Vector3f& I, const Eigen::Vector3f& N) const
	{
		/* Effective reflectivity */
		float R_eff = 1.0f;
		/* Included angle of incidence and the normal */
		float cosi = I.dot(N);
		/* IOR of mediums */
		float eta_i, eta_t; 
		if (cosi > 0) {
			eta_i = 1.0f;
			eta_t = refract_brdf.ior;
		} else {
			cosi *= -1;
			eta_i = refract_brdf.ior;
			eta_t = 1.0f;
		}
		/* Refraction angle */
		float sint = (eta_i / eta_t) * sqrt(1 - cosi * cosi); 
		/* Once refraction angle >= 1, consider totally internal reflection */
		if (sint < 1) { 
			float cost = sqrt(1 - sint * sint); 
			float Rs = ((eta_t * cosi) - (eta_i * cost)) / ((eta_t * cosi) + (eta_i * cost)); 
			float Rp = ((eta_i * cosi) - (eta_t * cost)) / ((eta_i * cosi) + (eta_t * cost)); 
			R_eff = (Rs * Rs + Rp * Rp) / 2; 
		}

		return R_eff;
	}

	Eigen::Vector3f eval(Interaction& _interact, float* pdf = nullptr)
	{
		float R_eff = fresnel_R_eff(_interact.outputDir, _interact.normal);
		Eigen::Vector2f effs(R_eff, 1 - R_eff);
		Eigen::Vector2f pdfs;
		Eigen::MatrixXf fs(3, 2);
		fs.col(0) << specular_brdf.eval(_interact, &pdfs[0]);
		fs.col(1) << refract_brdf.eval(_interact, &pdfs[1]);

		Eigen::Vector3f::Index j;
		pdfs.maxCoeff(&j);

		if (pdf) {
			*pdf = pdfs(j) * effs(j);
		}

		return fs.col(j);
	}

	float sample(Interaction& _interact)
	{
		float R_eff = fresnel_R_eff(_interact.outputDir, _interact.normal);
		/* Sample according to reflection and transmittance effects */
		float u = mathext::unif(0, 1.0f)[0];
		if (u > R_eff) {
			return refract_brdf.sample(_interact) * (1 - R_eff);
		} else {
			return specular_brdf.sample(_interact) * R_eff;
		}
	}
};