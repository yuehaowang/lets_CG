#pragma once

#include <vector>
#include "Eigen/Dense"

class Film {
public:
    Film(const Eigen::Vector2i& res) {
        m_Res = res;
        pixelSamples.resize(m_Res.x() * m_Res.y(), Eigen::Vector3f(1, 1, 1));
    }

	float getAspectRatio() {
        return static_cast<float>(m_Res.x()) / m_Res.y();
    }

    Eigen::Vector2i m_Res;
    std::vector<Eigen::Vector3f> pixelSamples;
};
