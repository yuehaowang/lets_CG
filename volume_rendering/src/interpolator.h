#pragma once
#include "Eigen/Dense"
#include "volumeData.h"
#include <algorithm>
#include <vector>

/* Convert a float-point number to an 1-D vector */
#define f2vec(v) ((v) * (Eigen::VectorXf::Ones(1)))

class Interpolator
{
public:
    virtual volumeData interpolate(Eigen::Vector3f &p, const voxel &voxel) = 0;
};

class NearestNeighbourInterpolator : public Interpolator
{
    inline volumeData interpolate(Eigen::Vector3f &p, const voxel &voxel)
    {
        /* Reference: https://en.wikipedia.org/wiki/Nearest-neighbor_interpolation */
        auto cmp_dist = [p](const volumeData &v_a, const volumeData &v_b) { return (v_a.position - p).norm() < (v_b.position - p).norm(); };
        volumeData nn_vol_d = std::min({voxel.c100, voxel.c010, voxel.c000, voxel.c110, voxel.c001, voxel.c101, voxel.c011, voxel.c111}, cmp_dist);

        return volumeData(p.x(), p.y(), p.z(), nn_vol_d.density, nn_vol_d.gradient);
    };
};

class TrilinearInterpolator : public Interpolator
{
    inline volumeData interpolate(Eigen::Vector3f &p, const voxel &voxel)
    {
        volumeData c100_d = voxel.c100;
        volumeData c010_d = voxel.c010;
        volumeData c000_d = voxel.c000;
        volumeData c110_d = voxel.c110;
        volumeData c001_d = voxel.c001;
        volumeData c101_d = voxel.c101;
        volumeData c011_d = voxel.c011;
        volumeData c111_d = voxel.c111;

        /* Trilinear interpolation. Note: order of arguments matters.
           Reference: https://en.wikipedia.org/wiki/Trilinear_interpolation */
        float x = p.x(), y = p.y(), z = p.z();
        float x0 = c000_d.position.x(), y0 = c000_d.position.y(), z0 = c000_d.position.z();
        float x1 = c111_d.position.x(), y1 = c111_d.position.y(), z1 = c111_d.position.z();
        float xd = (x - x0) / (x1 - x0), yd = (y - y0) / (y1 - y0), zd = (z - z0) / (z1 - z0);
        auto tri_lerp = [xd, yd, zd](
                            Eigen::VectorXf c100, Eigen::VectorXf c010, Eigen::VectorXf c000, Eigen::VectorXf c110,
                            Eigen::VectorXf c001, Eigen::VectorXf c101, Eigen::VectorXf c011, Eigen::VectorXf c111) {
            Eigen::VectorXf c00 = c000 * (1 - xd) + c100 * xd;
            Eigen::VectorXf c01 = c001 * (1 - xd) + c101 * xd;
            Eigen::VectorXf c10 = c010 * (1 - xd) + c110 * xd;
            Eigen::VectorXf c11 = c011 * (1 - xd) + c111 * xd;
            Eigen::VectorXf c0 = c00 * (1 - yd) + c10 * yd;
            Eigen::VectorXf c1 = c01 * (1 - yd) + c11 * yd;
            Eigen::VectorXf c = c0 * (1 - zd) + c1 * zd;

            return c;
        };

        /* Interpolate density */
        float d = tri_lerp(
            f2vec(c100_d.density), f2vec(c010_d.density), f2vec(c000_d.density), f2vec(c110_d.density),
            f2vec(c001_d.density), f2vec(c101_d.density), f2vec(c011_d.density), f2vec(c111_d.density))(0);
        /* Interpolate gradient */
        Eigen::Vector3f grad = tri_lerp(
            c100_d.gradient, c010_d.gradient, c000_d.gradient, c110_d.gradient,
            c001_d.gradient, c101_d.gradient, c011_d.gradient, c111_d.gradient);

        return volumeData(p.x(), p.y(), p.z(), d, grad);
    };
};