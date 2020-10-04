#pragma once
#include <string>
#include <vector>
#include "Eigen/Dense"
#include "stb_image_write.h"
inline float clamp(float x) { return x < 0 ? 0 : x > 1 ? 1 : x; }
inline unsigned char toInt(float x)
{
    unsigned char c(pow(clamp(x), 1 / 2.2) * 255 + .5);
    return c;
}
class Film
{
public:
    Film(const Eigen::Vector2i &res)
    {
        m_Res = res;
        pixelSamples.resize(m_Res.x() * m_Res.y(), Eigen::Vector3f(1, 1, 1));
    }

    float getAspectRatio()
    {
        return static_cast<float>(m_Res.x()) / m_Res.y();
    }

    Eigen::Vector2i m_Res;
    std::vector<Eigen::Vector3f> pixelSamples;

    void write(std::string outputPath)
    {
        std::vector<unsigned char> outputData;
        outputData.reserve(int(m_Res.x() * m_Res.y() * 3));
        for (Eigen::Vector3f v : pixelSamples)
        {
            for (int i = 0; i < 3; i++)
            {
                outputData.push_back(toInt(v[i]));
            }
        }
        stbi_flip_vertically_on_write(true);
        stbi_write_png(outputPath.c_str(), m_Res.x(), m_Res.y(), 3, outputData.data(), 0);
    }
};
