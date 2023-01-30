#include "compositor.h"

Compositor::Compositor(){};

Compositor::~Compositor(){};

void Compositor::compositeFrontToBack(Eigen::Vector3f &color_dst, Eigen::Vector3f &alpha_dst, Eigen::Vector3f color_src, Eigen::Vector3f alpha_src)
{
    color_dst = color_dst + (Eigen::Vector3f::Ones() - alpha_dst).cwiseProduct(color_src);
    alpha_dst = alpha_dst + (Eigen::Vector3f::Ones() - alpha_dst).cwiseProduct(alpha_src);
};

void Compositor::compositeBackToFront(Eigen::Vector3f &color_dst, Eigen::Vector3f color_src, Eigen::Vector3f alpha_src)
{
    color_dst = (Eigen::Vector3f::Ones() - alpha_src).cwiseProduct(color_dst) + color_src;
};
