#include "mathext.hpp"

int main()
{
    Eigen::Matrix3f rot1 = transform::rot_align_normal(Eigen::Vector3f(0, -1.0f, 0));
    std::cout << (rot1 * Eigen::Vector3f(0, 1.0f, 0)).transpose() << std::endl;
    std::cout << (rot1 * Eigen::Vector3f(0, 1.0f, 1.0f)).transpose() << std::endl;
    std::cout << (rot1 * Eigen::Vector3f(1.0f, 0, 0)).transpose() << std::endl;

    std::cout << "=============" << std::endl;

    Eigen::Matrix3f rot2 = transform::rot_align_normal(Eigen::Vector3f(0, 1.0f, 1.0f).normalized());
    std::cout << (rot2 * Eigen::Vector3f(0, 1.0f, 0)).transpose() << std::endl;
    std::cout << (rot2 * Eigen::Vector3f(0, 0, 1.0f)).transpose() << std::endl;
    std::cout << (rot2 * Eigen::Vector3f(0, 1.0f, 1.0f)).transpose() << std::endl;
    std::cout << (rot2 * Eigen::Vector3f(1.0f, 0, 0)).transpose() << std::endl;

    std::cout << "=============" << std::endl;

    Eigen::Matrix3f rot3 = transform::rot_align_normal(Eigen::Vector3f(0, 0, 1.0f));
    std::cout << rot3 << std::endl;

    return 0;
}