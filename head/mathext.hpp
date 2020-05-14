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


namespace mathext {
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
        std::vector<float> u = unif(0.0f, 1.0f, 2);

        return Eigen::Vector2f(r * sqrt(u[0]), 2 * PI * u[1]);
    }

    Eigen::Matrix3f rot_from_two_vectors(Eigen::Vector3f src, Eigen::Vector3f des)
    {
        return Eigen::Quaternionf::FromTwoVectors(src, des).toRotationMatrix();
    }

    Eigen::Matrix3f rot_align_normal(Eigen::Vector3f n)
    {
        return rot_from_two_vectors(Eigen::Vector3f(0.0f, 0.0f, 1.0f), n);
    }

    Eigen::Vector3f reflect(Eigen::Vector3f v1, Eigen::Vector3f n)
    {
        return 2 * n.dot(v1) * n - v1;
    }

    float power_heuristic(int n_f, float p_f, int n_g, float p_g, float beta = 2)
    {
        float c_f = (float)n_f * p_f, c_g = (float)n_g * p_g;
        return pow(c_f, beta) / (pow(c_f, beta) + pow(c_g, beta));
    }
}
