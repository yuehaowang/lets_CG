#pragma once
#include <iostream>
#include <random>
#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#define EPSILON 1e-6f
#define DELTA 1e-4f
#define PI 3.14159265358979323846f
#define PI_INV 0.31830988618f


namespace sampler {
    std::default_random_engine random_generator;

    std::vector<float> unif(float a, float b, int N = 1)
    {
        std::vector<float> res;
        std::uniform_real_distribution<float> dis(a, b);

        for (int i = 0; i < N; i++) {
            res.push_back(dis(random_generator));
        }
        return res;
    }

    Eigen::Vector2f disk(float r)
    {
        // std::uniform_real_distribution<float> unif(0.0f, 1.0f);
        std::vector<float> u = unif(0.0f, 1.0f, 2);

        return Eigen::Vector2f(r * sqrt(u[0]), 2 * PI * u[1]);
    }

    Eigen::Vector2f cosine_weighted_hemisphere(float r)
    {
        Eigen::Vector2f disk_sample = disk(r);
        float a = disk_sample.x();
        float phi = disk_sample.y();
        float theta = asin(a / r);

        return Eigen::Vector2f(theta, phi);
    }
}

namespace pdf {
    float disk_xy(float r)
    {
        return 1 / (PI * r * r);
    }

    float disk_polar(float r, float r0)
    {
        return r0 / (PI * r * r);
    }

    float cosine_weighted_hemisphere_w(float theta)
    {
        return abs(cos(theta) * PI_INV);
    }

    float cosine_weighted_hemisphere_polar(float theta)
    {
        return abs(cos(theta) * sin(theta) * PI_INV);
    }
}

namespace transform {
    Eigen::Matrix3f rot_from_two_vectors(Eigen::Vector3f src, Eigen::Vector3f des)
    {
        // Eigen::Vector3f v = src.cross(des);
        // float angle = asin(v.norm());
        // Eigen::Vector3f axis = v.normalized();

        // return Eigen::AngleAxisf(angle, axis).toRotationMatrix();
        return Eigen::Quaternionf::FromTwoVectors(src, des).toRotationMatrix();
    }

    Eigen::Matrix3f rot_align_normal(Eigen::Vector3f n)
    {
        return rot_from_two_vectors(Eigen::Vector3f(0, 0, 1.0f), n);
    }

    Eigen::Vector3f polar2cartesian(float r, Eigen::Vector2f sph_coords)
    {
        return Eigen::Vector3f(
			r * sin(sph_coords.x()) * cos(sph_coords.y()),
			r * sin(sph_coords.x()) * sin(sph_coords.y()),
			r * cos(sph_coords.x())
		);
    }
}