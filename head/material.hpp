#pragma once
#include "Eigen/Dense"
#include "interaction.hpp"

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
	virtual Eigen::Vector3f eval(Interaction &_interact) = 0;
	
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
	
	Eigen::Vector3f eval(Interaction& _interact)
	{
		// TODO
	};

	float sample(Interaction& _interact)
	{
		// TODO
	};
};

class IdealSpecular : public BRDF
{
public:

	IdealSpecular() {
		isSpecular = true;
	}

	Eigen::Vector3f eval(Interaction &_interact)
	{
		// TODO
	};

	float sample(Interaction& _interact)
	{
		// TODO
	};
};