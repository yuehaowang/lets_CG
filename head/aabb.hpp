#pragma once
#include <utility>
#include "ray.hpp"

class AABB {
public:
	AABB() {}
	
	AABB(float lb_x, float lb_y, float lb_z, float ub_x, float ub_y, float ub_z) {
		lb = Eigen::Vector3f(lb_x, lb_y, lb_z);
		ub = Eigen::Vector3f(ub_x, ub_y, ub_z);
	}

	AABB(Eigen::Vector3f lb, Eigen::Vector3f ub) : lb(std::move(lb)), ub(std::move(ub)) {	}

	AABB(const Eigen::Vector3f& pos, float radius) {
		Eigen::Vector3f r(radius, radius, radius);
		lb = pos - r;
		ub = pos + r;
	}

	AABB(const Eigen::Vector3f& v1, const Eigen::Vector3f& v2, const Eigen::Vector3f& v3) {
		lb = v1.cwiseMin(v2).cwiseMin(v3);
		ub = v1.cwiseMax(v2).cwiseMax(v3);
	}

	AABB(const AABB& a, const AABB& b) {
		lb = Eigen::Vector3f(a.lb.cwiseMin(b.lb));
		ub = Eigen::Vector3f(a.ub.cwiseMax(b.ub));
	}

	Eigen::Vector3f getCenter() const {
		return (lb + ub) / 2;
	}

	float getDist(int c) const {
		return ub[c] - lb[c];
	}

	float getVolume() const {
		return getDist(2) * getDist(1) * getDist(0);
	}

	bool checkOverlap(const AABB& a) const {
		return ((a.lb[0] >= this->lb[0] && a.lb[0] <= this->ub[0]) || (this->lb[0] >= a.lb[0] && this->lb[0] <= a.ub[0])) &&
			((a.lb[1] >= this->lb[1] && a.lb[1] <= this->ub[1]) || (this->lb[1] >= a.lb[1] && this->lb[1] <= a.ub[1])) &&
			((a.lb[2] >= this->lb[2] && a.lb[2] <= this->ub[2]) || (this->lb[2] >= a.lb[2] && this->lb[2] <= a.ub[2]));

	}

	float diagonalLength() const {
		return (ub - lb).norm();
	}

	bool rayIntersection(const Ray& ray, float& tmin, float& tmax) {
		float dirFracX = (ray.m_Dir[0] == 0.0) ? 1.0e32 : 1.0f / ray.m_Dir[0];
		float dirFracY = (ray.m_Dir[1] == 0.0) ? 1.0e32 : 1.0f / ray.m_Dir[1];
		float dirFracZ = (ray.m_Dir[2] == 0.0) ? 1.0e32 : 1.0f / ray.m_Dir[2];

		float tx1 = (lb[0] - ray.m_Ori[0]) * dirFracX;
		float tx2 = (ub[0] - ray.m_Ori[0]) * dirFracX;
		float ty1 = (lb[1] - ray.m_Ori[1]) * dirFracY;
		float ty2 = (ub[1] - ray.m_Ori[1]) * dirFracY;
		float tz1 = (lb[2] - ray.m_Ori[2]) * dirFracZ;
		float tz2 = (ub[2] - ray.m_Ori[2]) * dirFracZ;

		tmin = std::max(std::max(std::min(tx1, tx2), std::min(ty1, ty2)), std::min(tz1, tz2));
		tmax = std::min(std::min(std::max(tx1, tx2), std::max(ty1, ty2)), std::max(tz1, tz2));

		// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
		if (tmax < 0) {
			return false;
		}

		return tmax >= tmin;
	}
	
	// bottom-left and upper-right corner of the box
	Eigen::Vector3f lb;
	Eigen::Vector3f ub;
};