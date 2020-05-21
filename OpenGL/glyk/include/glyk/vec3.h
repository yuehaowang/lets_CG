#ifndef _VEC3_H_
#define _VEC3_H_

#include <cmath>
#include <ostream>


template <typename T>
struct Vec3
{
    T x;
    T y;
    T z;

    Vec3() : x(0), y(0), z(0) {}

    Vec3(T v_x, T v_y, T v_z) : x(v_x), y(v_y), z(v_z) {}

    Vec3(const Vec3<T>& v) { *this = v; }

    float Length()
    {
        return sqrt(x * x + y * y + z * z);
    }

    void Normalize()
    {
        float l = Length();

        x /= l;
        y /= l;
        z /= l;
    }

    static T Dot(Vec3<T> v1, Vec3<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    static Vec3<T> Cross(Vec3<T> v1, Vec3<T> v2)
    {
        return Vec3<T>(
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        );
    }

    void operator *= (T c) { *this = *this * c; }
    void operator *= (Vec3<T> v) { *this = *this * v; }
    void operator /= (T c) { *this = *this / c; }
    void operator += (Vec3<T> v1) { *this = *this + v1; }
    void operator -= (Vec3<T> v1) { *this = *this - v1; }
    void operator = (const Vec3<T>& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }
};

template <typename T>
Vec3<T> operator * (Vec3<T> v, T c)
{
    return Vec3<T>(v.x * c, v.y * c, v.z * c);
}

template <typename T>
Vec3<T> operator * (T c, Vec3<T> v) { return v * c; }

template <typename T>
Vec3<T> operator * (Vec3<T> v1, Vec3<T> v2)
{
    return Vec3<T>::Cross(v1, v2);
}

template <typename T>
Vec3<T> operator / (Vec3<T> v, T c)
{
    return Vec3<T>(v.x / c, v.y / c, v.z / c);
}

template <typename T>
Vec3<T> operator + (Vec3<T> v1, Vec3<T> v2)
{
    return Vec3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template <typename T>
Vec3<T> operator - (Vec3<T> v1, Vec3<T> v2)
{
    return Vec3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template <typename T>
Vec3<T> operator - (Vec3<T> v)
{
    return Vec3<T>(-v.x, -v.y, -v.z);
}

template <typename T>
bool operator == (Vec3<T> v1, Vec3<T> v2)
{
    return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
}

template <typename T>
std::ostream& operator << (std::ostream& os, const Vec3<T>& v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
     
    return os;  
}


typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<int> Vec3i;
typedef Vec3<unsigned int> Vec3ui;


#endif