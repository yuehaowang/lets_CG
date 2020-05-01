#pragma once
#include <utility>
#include "aabb.hpp"
#include "interaction.hpp"
#include "material.hpp"

class Shape
{
public:
	Shape(Eigen::Vector3f color) : color(std::move(color)) {}
	Shape(AABB aabb, Eigen::Vector3f color) : m_BoundingBox(std::move(aabb)), color(std::move(color)) {}
	virtual ~Shape() = default;
	
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) = 0;

	AABB m_BoundingBox;
	Eigen::Vector3f color;
	BRDF* material = nullptr;
};
