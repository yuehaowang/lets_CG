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

    Eigen::Vector3f refract(Eigen::Vector3f v1, Eigen::Vector3f n, float ior = 1.0f)
    {
        float cos_theta_i = v1.dot(n); 
        float eta = 1 / ior;
        if (cos_theta_i < 0) {
            eta = 1 / eta;
            n *= -1;
            cos_theta_i *= -1;
        }
        float c = sqrt(1 - eta * eta * (1 - cos_theta_i * cos_theta_i));
        return c < 0 ? Eigen::Vector3f(0, 0, 0) : (eta * (-v1) + (eta * cos_theta_i - c) * n);
    }

    float power_heuristic(int n_f, float p_f, int n_g, float p_g, float beta = 2)
    {
        float c_f = (float)n_f * p_f, c_g = (float)n_g * p_g;
        return pow(c_f, beta) / (pow(c_f, beta) + pow(c_g, beta));
    }
}
