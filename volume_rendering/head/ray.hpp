#pragma once
#include "Eigen/Dense"
#include <limits>
//! @brief  Data structure representing a ray.
class Ray {
public:
    // constructor from a point and a direction
    // para 'ori'   :   original point of the ray
    // para 'dir'   :   direction of the ray , it's the programmer's responsibility to normalize it
    // para 'fmin'  :   the minimum range of the ray . It could be set a very small value to avoid false self intersection
    // para 'fmax'  :   the maximum range of the ray . A ray with 'fmax' not equal to 0 is actually a line segment, usually used for shadow ray.
    Ray(const Eigen::Vector3f& ori, const Eigen::Vector3f& dir, float fmin = 1e-5f, float fmax = std::numeric_limits<float>::max()) {
        m_Ori = ori;
        m_Dir = dir.normalized();
        m_fMin = fmin;
        m_fMax = fmax;
    }

    // function to get a point from the ray
    // para 't' :   the distance from the retrieve point if the direction of the ray is normalized.
    // result   :   A point ( o + t * d )
    Eigen::Vector3f getPoint(float t) const {
        return m_Ori + t * m_Dir;
    }
	
    // original point of the ray
    Eigen::Vector3f m_Ori;
    // direction of the ray
    Eigen::Vector3f m_Dir;

    // the maximum and minimum value in the ray
    float   m_fMin;
    float   m_fMax;
};
