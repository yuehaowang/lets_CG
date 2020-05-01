#pragma once

#include <utility>


#include "Eigen/Dense"
#include "film.hpp"
#include "ray.hpp"

#define M_PIf 3.14159265358979323846f

class Camera {
public:
    Camera(const Eigen::Vector3f& pos, const Eigen::Vector3f& lookAt, const Eigen::Vector3f& up, float verticalFov, const Eigen::Vector2i& filmRes) : m_Film(filmRes)
	{
        m_Pos = pos;
        m_Forward = (lookAt - pos).normalized();
        m_Right = m_Forward.cross(up);
        m_Up = m_Right.cross(m_Forward);

        float vlen = tanf(0.5f * verticalFov * M_PIf / 180.0f);
        m_Up *= vlen;
        float ulen = vlen * m_Film.getAspectRatio();
        m_Right *= ulen;
    }

	// Generate camera ray
	Ray generateRay(float dx, float dy)
	{
        Eigen::Array2f t(dx, dy);
        Eigen::Array2f r(m_Film.m_Res.x(), m_Film.m_Res.y());
        t = t / r * 2 - 1;
        return {m_Pos, t.x() * m_Right + t.y() * m_Up + m_Forward};
    }

    void setPixel(int dx, int dy, Eigen::Vector3f value)
    {
        m_Film.pixelSamples[dy * m_Film.m_Res.x() + dx] = std::move(value);
    }
	
    Eigen::Vector3f m_Pos;
	
    Eigen::Vector3f m_Forward;
    Eigen::Vector3f m_Right;
    Eigen::Vector3f m_Up;
	
    Film m_Film;
};
