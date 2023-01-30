#pragma once
#include "Eigen/Dense"
#include <string>
#include <vector>

class volumeData
{
public:
    Eigen::Vector3f position;
    float density;
    Eigen::Vector3f gradient;
    inline volumeData(){};
    inline volumeData(float x, float y, float z, float den, Eigen::Vector3f grad = Eigen::Vector3f(0, 0, 0))
    {
        this->position = Eigen::Vector3f(x, y, z);
        this->density = den;
        this->gradient = grad;
    };
    inline ~volumeData(){};
};
