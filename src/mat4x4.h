#ifndef _MAT4X4_H_
#define _MAT4X4_H_

#define MAT4X4_M 4
#define MAT4X4_N 4
#define MAT4X4_LENGTH 16
#define PI 3.14159265359

#include <iostream>
#include <cmath>
#include <cstring>

#include "vec3.h"


template <typename T>
class Mat4x4
{

private:

    T* array;

public:

    Mat4x4();
    ~Mat4x4();
    Mat4x4(const Mat4x4<T>& m);
    Mat4x4(T* arr);
    const T* Ptr() const;
    void operator = (const Mat4x4<T>& m);
    T operator () (unsigned int row, unsigned int col);
    void operator *= (const T c);
    void operator *= (const Mat4x4<T>& m);
    void operator += (const Mat4x4<T>& m);
    template <typename U> friend std::ostream& operator << (std::ostream& os, const Mat4x4<U>& m);
    void Translate(T x = 0, T y = 0, T z = 0);
    void Rotate(T euler_x = 0, T euler_y = 0, T euler_z = 0);
    void Scale(T sx = 1, T sy = 1, T sz = 1);
    void Perspective(Vec3<T> image_plane);
    void Projection(Vec3<T> space);

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
Mat4x4<T>::Mat4x4(const Mat4x4<T>& m) { *this = m; }

template <typename T>
const T* Mat4x4<T>::Ptr() const
{
    return array;
}

template <typename T>
T Mat4x4<T>::operator () (unsigned int row, unsigned int col)
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
Mat4x4<T> operator * (const T c, const Mat4x4<T>& m1) { return m1 * c; }

template <typename T>
void Mat4x4<T>::operator *= (const T c) { *this = *this * c; }

template <typename T>
void Mat4x4<T>::operator *= (const Mat4x4<T>& m) { *this = *this * m; }

template <typename T>
void Mat4x4<T>::operator = (const Mat4x4<T>& m)
{
    const T* m_ptr = m.Ptr();

    array = new T[MAT4X4_LENGTH];

    for (int j = 0; j < MAT4X4_LENGTH; j++) {
        array[j] = m_ptr[j];
    }
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
void Mat4x4<T>::Translate(T x, T y, T z)
{
    Mat4x4<T> m_t((T [MAT4X4_LENGTH]) {
        1,  0,  0, 0,
        0,  1,  0, 0,
        0,  0,  1, 0,
        x,  y,  z, 1
    });

    std::memcpy(array, (m_t * (*this)).Ptr(), MAT4X4_LENGTH * sizeof(T));
}

template <typename T>
void Mat4x4<T>::Rotate(T euler_x, T euler_y, T euler_z)
{
    T a = euler_x * PI / 180;
    Mat4x4<T> m_x((T [MAT4X4_LENGTH]) {
        1,       0,        0,     0,
        0,  cos(a),  -sin(a),     0,
        0,  sin(a),   cos(a),     0,
        0,       0,        0,     1
    });

    a = euler_y * PI / 180;
    Mat4x4<T> m_y((T [MAT4X4_LENGTH]) {
        cos(a),   0, sin(a),   0,
             0,   1,      0,   0,
        -sin(a),  0, cos(a),   0,
             0,   0,      0,   1
    });

    a = euler_z * PI / 180;
    Mat4x4<T> m_z((T [MAT4X4_LENGTH]) {
        cos(a),  -sin(a),    0,    0,
        sin(a),   cos(a),    0,    0,
             0,        0,    1,    0,
             0,        0,    0,    1
    });

    std::memcpy(array, (m_x * m_y * m_z * (*this)).Ptr(), MAT4X4_LENGTH * sizeof(T));
}

template <typename T>
void Mat4x4<T>::Scale(T sx, T sy, T sz)
{
    Mat4x4<T> m_s((T [MAT4X4_LENGTH]) {
        sx,  0,  0, 0,
         0, sy,  0, 0,
         0,  0, sz, 0,
         0,  0,  0, 1
    });

    std::memcpy(array, (m_s * (*this)).Ptr(), MAT4X4_LENGTH * sizeof(T));
}

template <typename T>
void Mat4x4<T>::Perspective(Vec3<T> image_plane)
{
    T ex = image_plane.x, ey = image_plane.y, ez = image_plane.z;

    Mat4x4<T> m_p((T [MAT4X4_LENGTH]) {
        1, 0, -ex / ez,       0,
        0, 1, -ey / ez,       0,
        0, 0,        1, -1 / ez,
        0, 0,        0,       1
    });

    std::memcpy(array, (m_p * (*this)).Ptr(), MAT4X4_LENGTH * sizeof(T));
}

template <typename T>
void Mat4x4<T>::Projection(Vec3<T> space)
{
    T width = space.x, height = space.y, depth = space.z;

    Mat4x4<T> m_p((T [MAT4X4_LENGTH]) {
        1 / width,           0,         0,  0,
                0, 1 / height,          0,  0,
                0,           0, 1 / depth,  0,
                0,           0,         0,  1,
    });

    std::memcpy(array, (m_p * (*this)).Ptr(), MAT4X4_LENGTH * sizeof(T));
}


#endif