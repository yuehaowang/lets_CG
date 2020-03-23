#ifndef _MAT4X4_H_
#define _MAT4X4_H_

#define MAT4X4_M 4
#define MAT4X4_N 4
#define MAT4X4_LENGTH 16
#define PI 3.14159265359

#include <iostream>
#include <cmath>
#include <cstring>
#include "glyk/vec3.h"


template <typename T>
class Mat4x4
{

private:

    T* array;

public:

    Mat4x4();
    virtual ~Mat4x4();
    Mat4x4(const Mat4x4<T>& m);
    Mat4x4(T* arr);
    const T* Ptr() const;
    void operator = (const Mat4x4<T>& m);
    T& operator () (unsigned int row, unsigned int col);
    void operator *= (const T c);
    void operator *= (const Mat4x4<T>& m);
    void operator += (const Mat4x4<T>& m);
    void operator -= (const Mat4x4<T>& m);
    template <typename U> friend std::ostream& operator << (std::ostream& os, const Mat4x4<U>& m);
    void Transpose();
    void Identity();
    void Translate(T x = 0, T y = 0, T z = 0);
    void Rotate(T euler_x = 0, T euler_y = 0, T euler_z = 0);
    void Scale(T sx = 1, T sy = 1, T sz = 1);
    void Perspective(float fovv, float aspect_ratio, T n, T f);
    void Orthographic(float fovv, float aspect_ratio, T n, T f);
    void LookAt(Vec3<T> cam_pos, Vec3<T> target, Vec3<T> cam_u);
    void Eye(Vec3<T> cam_pos, Vec3<T> cam_d, Vec3<T> cam_u);

};

template <typename T>
Mat4x4<T>::Mat4x4()
{
    array = new T[MAT4X4_LENGTH];

    for (int i = 0; i < MAT4X4_LENGTH; i++) {
        int col = i % MAT4X4_N, row = (i - col) / MAT4X4_N;
        if (col == row) {
            array[i] = 1;
        } else {
            array[i] = 0;
        }
    }
}

template <typename T>
Mat4x4<T>::~Mat4x4()
{
    delete[] array;
}

template <typename T>
Mat4x4<T>::Mat4x4(T* arr)
{
    array = new T[MAT4X4_LENGTH];

    for (int j = 0; j < MAT4X4_LENGTH; j++) {
        array[j] = arr[j];
    }
}

template <typename T>
Mat4x4<T>::Mat4x4(const Mat4x4<T>& m) : array(NULL) { *this = m; }

template <typename T>
const T* Mat4x4<T>::Ptr() const
{
    return array;
}

template <typename T>
T& Mat4x4<T>::operator () (unsigned int row, unsigned int col)
{
    if (row >= MAT4X4_M) {
        row = MAT4X4_M - 1;
    }

    if (col >= MAT4X4_N) {
        col = MAT4X4_N - 1;
    }

    return array[row * MAT4X4_N + col];
}

template <typename T>
Mat4x4<T> operator + (const Mat4x4<T>& m1, const Mat4x4<T>& m2)
{
    const T* a = m1.Ptr();
    const T* b = m2.Ptr();

    T* res_arr = new T[MAT4X4_LENGTH];

    for (int j = 0; j < MAT4X4_LENGTH; j++) {
        res_arr[j] = a[j] + b[j];
    }

    Mat4x4<T> res(res_arr);

    delete[] res_arr;

    return res;
}

template <typename T>
void Mat4x4<T>::operator += (const Mat4x4<T>& m) { *this = *this + m; }

template <typename T>
Mat4x4<T> operator - (const Mat4x4<T>& m1, const Mat4x4<T>& m2)
{
    const T* a = m1.Ptr();
    const T* b = m2.Ptr();

    T* res_arr = new T[MAT4X4_LENGTH];

    for (int j = 0; j < MAT4X4_LENGTH; j++) {
        res_arr[j] = a[j] - b[j];
    }

    Mat4x4<T> res(res_arr);

    delete[] res_arr;

    return res;
}

template <typename T>
void Mat4x4<T>::operator -= (const Mat4x4<T>& m) { *this = *this - m; }

template <typename T>
Mat4x4<T> operator * (const Mat4x4<T>& m1, const Mat4x4<T>& m2)
{
    const T* a = m1.Ptr();
    const T* b = m2.Ptr();

    T* res_arr = new T[MAT4X4_LENGTH];

    for (int j = 0; j < MAT4X4_LENGTH; j++) {
        res_arr[j] = 0;

        int col = j % MAT4X4_N;
        int row = (j - col) / MAT4X4_N;

        for (int i = 0; i < MAT4X4_N; i++) {
            res_arr[j] += a[row * MAT4X4_N + i] * b[col + i * MAT4X4_N];
        }
    }

    Mat4x4<T> res(res_arr);

    delete[] res_arr;

    return res;
}

template <typename T>
Mat4x4<T> operator * (const Mat4x4<T>& m1, const T c)
{
    const T* a = m1.Ptr();

    T* res_arr = new T[MAT4X4_LENGTH];

    for (int j = 0; j < MAT4X4_LENGTH; j++) {
        res_arr[j] = a[j] * c;
    }

    Mat4x4<T> res(res_arr);

    delete[] res_arr;

    return res;
}

template <typename T>
Vec3<T> operator * (const Mat4x4<T>& m1, const Vec3<T>& v)
{
    const T* a = m1.Ptr();
    T v_arr[] = {v.x, v.y, v.z, 1};
    T res_arr[MAT4X4_N];

    for (int j = 0; j < MAT4X4_N; j++) {
        res_arr[j] = 0;

        for (int i = 0; i < MAT4X4_N; i++) {
            res_arr[j] += a[j * MAT4X4_N + i] * v_arr[i];
        }
    }

    Vec3<T> res(res_arr[0] / res_arr[3], res_arr[1] / res_arr[3], res_arr[2] / res_arr[3]);
    return res;
}

template <typename T>
Mat4x4<T> operator * (const T c, const Mat4x4<T>& m1) { return m1 * c; }

template <typename T>
void Mat4x4<T>::operator *= (const T c) { *this = *this * c; }

template <typename T>
void Mat4x4<T>::operator *= (const Mat4x4<T>& m) { *this = *this * m; }

template <typename T>
void Mat4x4<T>::operator = (const Mat4x4<T>& m)
{
    if (array == NULL) {
        array = new T[MAT4X4_LENGTH];
    }

    std::memcpy(array, m.Ptr(), MAT4X4_LENGTH * sizeof(T));
}

template <typename U>
std::ostream& operator << (std::ostream& os, const Mat4x4<U>& m)
{
    for (int i = 0; i < MAT4X4_LENGTH; i++) {
        if (i > 0 && i % MAT4X4_N == 0) {
            os << std::endl;
        }

        os << m.array[i] << " ";
    }
     
    return os;  
}

template <typename T>
void Mat4x4<T>::Transpose()
{
    Mat4x4<T> new_m;

    for (int j = 0; j < MAT4X4_LENGTH; j++) {
        int col = j % MAT4X4_N;
        int row = (j - col) / MAT4X4_N;

        new_m(col, row) = array[j];
    }

    *this = new_m;
}

template <typename T>
void Mat4x4<T>::Identity()
{
    for (int j = 0; j < MAT4X4_LENGTH; j++) {
        int col = j % MAT4X4_N;
        int row = (j - col) / MAT4X4_N;

        if (col == row) {
            array[j] = 1;
        } else {
            array[j] = 0;
        }
    }
}

template <typename T>
void Mat4x4<T>::Translate(T x, T y, T z)
{
    T temp_m_arr[MAT4X4_LENGTH] = {
        1,  0,  0, x,
        0,  1,  0, y,
        0,  0,  1, z,
        0,  0,  0, 1
    };

    Mat4x4<T> m_t((T*)temp_m_arr);

    *this = m_t * (*this);
}

template <typename T>
void Mat4x4<T>::Rotate(T euler_x, T euler_y, T euler_z)
{
    T a = euler_x * PI / 180;
    T temp_m_arr[MAT4X4_LENGTH] = {
        1,       0,        0,     0,
        0,  cos(a),  -sin(a),     0,
        0,  sin(a),   cos(a),     0,
        0,       0,        0,     1
    };

    Mat4x4<T> m_x((T*)temp_m_arr);

    a = euler_y * PI / 180;
    T temp_m_arr2[MAT4X4_LENGTH] = {
        cos(a),   0, sin(a),   0,
             0,   1,      0,   0,
        -sin(a),  0, cos(a),   0,
             0,   0,      0,   1
    };
    Mat4x4<T> m_y((T*)temp_m_arr2);

    a = euler_z * PI / 180;
    T temp_m_arr3[MAT4X4_LENGTH] = {
        cos(a),  -sin(a),    0,    0,
        sin(a),   cos(a),    0,    0,
             0,        0,    1,    0,
             0,        0,    0,    1
    };
    Mat4x4<T> m_z((T*)temp_m_arr3);

    *this = m_z * m_y * m_x * (*this);
}

template <typename T>
void Mat4x4<T>::Scale(T sx, T sy, T sz)
{
    T temp_m_arr[MAT4X4_LENGTH] = {
        sx,  0,  0, 0,
         0, sy,  0, 0,
         0,  0, sz, 0,
         0,  0,  0, 1
    };

    Mat4x4<T> m_s((T*)temp_m_arr);
    
    *this = m_s * (*this);
}

template <typename T>
void Mat4x4<T>::LookAt(Vec3<T> cam_pos, Vec3<T> target, Vec3<T> cam_u)
{
    Eye(cam_pos, cam_pos - target, cam_u);
}

template <typename T>
void Mat4x4<T>::Eye(Vec3<T> cam_pos, Vec3<T> cam_d, Vec3<T> cam_u)
{
    cam_u.Normalize();
    cam_d.Normalize();
    Vec3<T> cam_r = cam_u * cam_d;

    T temp_m_arr[MAT4X4_LENGTH] = {
        cam_r.x, cam_r.y, cam_r.z, 0,
        cam_u.x, cam_u.y, cam_u.z, 0,
        cam_d.x, cam_d.y, cam_d.z, 0,
              0,       0,       0, 1
    };
    Mat4x4<T> orientation_mat((T*)temp_m_arr);

    T temp_m_arr2[MAT4X4_LENGTH] = {
        1, 0, 0, -cam_pos.x,
        0, 1, 0, -cam_pos.y,
        0, 0, 1, -cam_pos.z,
        0, 0, 0,          1
    };
    Mat4x4<T> translation_mat((T*)temp_m_arr2);

    *this = orientation_mat * translation_mat * (*this);
}

template <typename T>
void Mat4x4<T>::Perspective(float fovv, float aspect_ratio, T n, T f)
{
    T t = n * tan((fovv / 2) * PI / 180);
    T r = t * aspect_ratio;
    
    T temp_m_arr[MAT4X4_LENGTH] = {
        n / r,     0,                  0,                      0,
            0, n / t,                  0,                      0,
            0,     0, -(n + f) / (f - n), -(2 * f * n) / (f - n),
            0,     0,                 -1,                      0
    };
    Mat4x4<T> m_p((T*)temp_m_arr);

    *this = m_p * (*this);
}


template <typename T>
void Mat4x4<T>::Orthographic(float fovv, float aspect_ratio, T n, T f)
{
    T t = n * tan((fovv / 2) * PI / 180);
    T r = t * aspect_ratio;
    
    T temp_m_arr[MAT4X4_LENGTH] = {
        1 / r,     0,            0,                  0,
            0, 1 / t,            0,                  0,
            0,     0, -2 / (f - n), -(f + n) / (f - n),
            0,     0,            0,                  1
    };
    Mat4x4<T> m_p((T*)temp_m_arr);

    *this = m_p * (*this);
}


typedef Mat4x4<float> Mat4x4f;
typedef Mat4x4<double> Mat4x4d;
typedef Mat4x4<int> Mat4x4i;
typedef Mat4x4<unsigned int> Mat4x4ui;


#endif