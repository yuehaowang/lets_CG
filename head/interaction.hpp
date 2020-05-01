#pragma once
#include "Eigen/Dense"
class Interaction
{
public:
	Interaction() : entryDist(-1), exitDist(-1) {}
	
	// distance (in units of t) to intersection point
	float entryDist;
	// distance (in units of t) to the second intersection point(if existed)
	float exitDist;
	// position of intersection point
	Eigen::Vector3f entryPoint;
	// normal of intersection point
	Eigen::Vector3f normal;
	// barycentric coordinate of intersection point(if existed)
	Eigen::Vector2f uv;
	// color of intersection point
	Eigen::Vector3f surfaceColor;
	// wi input direction
	Eigen::Vector3f inputDir;
	// w0 output direction
	Eigen::Vector3f outputDir;
	// material of intersected object
	void * material = NULL;
	// light's ID if hit
	int lightId=-1;
};
