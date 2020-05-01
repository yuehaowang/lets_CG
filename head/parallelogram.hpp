#pragma once
#include "shape.hpp"

class Parallelogram : public Shape
{
public:
	Eigen::Vector3f p0;
	Eigen::Vector3f s0, s1;
	Eigen::Vector3f normal;
	float s0_len, s1_len;

	Parallelogram(Eigen::Vector3f p0, Eigen::Vector3f s0, Eigen::Vector3f s1, Eigen::Vector3f normal, Eigen::Vector3f color) : Shape(color), p0(p0), normal(normal.normalized())
	{
		s0_len = s0.norm();
		s1_len = s1.norm();
		this->s0 = s0.normalized();
		this->s1 = s1.normalized();
		Eigen::Vector3f p1 = p0 + s0 + s1;
		m_BoundingBox.lb = p0.cwiseMin(p1);
		m_BoundingBox.ub = p0.cwiseMax(p1);
	}

	bool rayIntersection(Interaction& interaction, const Ray& ray) override
	{
		if (ray.m_Dir.dot(normal) == 0)
		{
			return false;
		}
		
		float t = (p0 - ray.m_Ori).dot(normal) / ray.m_Dir.dot(normal);
		Eigen::Vector3f p0_p = ray.getPoint(t) - p0;
		float q0 = p0_p.dot(s0) / s0_len;
		float q1 = p0_p.dot(s1) / s1_len;
		// std::cout << "a:" << (p0 - ray.m_Ori).dot(normal) << " b:" << ray.m_Dir.dot(normal) << std::endl;
		// std::cout << "t:" << t << " q0:" << q0 << " q1:" << q1 << std::endl;
		if (q0 >= 0 && q0 <= s0.norm() && q1 >= 0 && q1 <= s1.norm() && t >= ray.m_fMin && t <= ray.m_fMax)
		{
			interaction.entryDist = t;
			interaction.exitDist = t;
			interaction.normal = normal;
			interaction.entryPoint = ray.getPoint(t);
			interaction.surfaceColor = color;
			interaction.uv[0] = q0;
			interaction.uv[1] = q1;
			return true;
		}
		return false;
	}
};
