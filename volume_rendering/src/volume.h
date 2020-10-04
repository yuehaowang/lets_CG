#pragma once
#include "Eigen/Dense"
#include "aabb.hpp"
#include "ray.hpp"
#include "volumeData.h"
#include "voxel.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Volume
{
private:
    std::vector<volumeData> raw_data;

public:
    Eigen::Vector3i size;
    Eigen::Vector3f size_physics;
    AABB bbox;
    double dx;
    float grad_maxnorm;
    bool is_null;
    Volume();
    ~Volume();
    Volume(std::string volumefile);
    bool getRayStartEnd(Ray &ray, float &t_start, float &t_end);
    void computeGradients();
    voxel getVoxel(Eigen::Vector3f position);
    volumeData &indexToData(Eigen::Vector3i index);
};