#include "glyk/mat4x4.h"
#include <iostream>


int main()
{
    ////////////////////////////////////////
    // matrix
    ////////////////////////////////////////

    float temp_m_arr[16] = {
        0, 1, 2, 4,
        9, 4, 7, 8,
        3, 6, 8, 0,
        0, 0, 0, 1
    };

    Mat4x4<float> m0((float*)temp_m_arr);
    m0.Transpose();
    std::cout << m0 << std::endl << std::endl;
    m0.Identity();
    std::cout << m0 << std::endl << std::endl;

    Mat4x4<float> m;
    std::cout << "scale" << std::endl;
    m.Scale(10.0f, 10.0f, 10.0f);

    Mat4x4<float> m2 = m;
    m2.Scale(10.0f, 10.0f, 10.0f);

    Mat4x4<float> m3(m * m2);

    Mat4x4<float> m4(0.1f * m);

    std::cout << "m" << std::endl << m << std::endl;
    std::cout << "m2" << std::endl << m2 << std::endl;
    std::cout << "m3" << std::endl << m3 << std::endl;
    std::cout << "m4 + m3" << std::endl << m4 + m3 << std::endl;

    std::cout << std::endl;

    ////////////////////////////////////////
    // vector
    ////////////////////////////////////////
    Vec3<float> v(1.0f, 2.0f, 3.0f);
    Vec3<float> v2(2.0f, 1.0f, 3.0f);

    std::cout << "v * v2" << std::endl << v * v2 << std::endl;
    std::cout << "v + v2" << std::endl << v + v2 << std::endl;
    std::cout << "v * 2" << std::endl << v * 2.0f << std::endl;

    v *= 3;
    std::cout << "v *= 3" << std::endl << v << std::endl;
    v += v2;
    std::cout << "v += v2" << std::endl << v << std::endl;

    Vec3<float> v3(1.0f, 0.0f, 0.0f);
    Vec3<float> v4(0.0f, 1.0f, 0.0f);
    std::cout << "v3 . v4" << std::endl << Vec3<float>::Dot(v3, v4) << std::endl;
    std::cout << "v3 x v4" << std::endl << v4 * v3 << std::endl;

    return 0;
}