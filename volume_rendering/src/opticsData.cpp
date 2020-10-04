#include "opticsData.h"

Eigen::Vector3f opticsData::getColor()
{
    return this->color;
};

Eigen::Vector3f opticsData::getOpacity() { return Eigen::Vector3f::Ones() - this->transparency; };
