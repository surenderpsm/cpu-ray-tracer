#pragma once

#include <cmath>
#include <iostream>

class Vec3
{
public:
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float e0, float e1, float e2) : x(e0), y(e1), z(e2) {}

    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    Vec3 &operator+=(const Vec3 &v);
    Vec3 &operator*=(const float t);
    Vec3 &operator/=(const float t);

    float length() const;
    float length_squared() const;
};

inline Vec3 operator+(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vec3 operator*(float t, const Vec3 &v)
{
    return Vec3(t * v.x, t * v.y, t * v.z);
}

inline Vec3 operator*(const Vec3 &v, float t) { return t * v; }

inline Vec3 operator/(Vec3 v, float t) { return (1 / t) * v; }

inline float dot(const Vec3 &u, const Vec3 &v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

inline Vec3 normalize(Vec3 v) { return v / v.length(); }

// Output
inline std::ostream &operator<<(std::ostream &out, const Vec3 &v)
{
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

// Implementation
inline Vec3 &Vec3::operator+=(const Vec3 &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

inline Vec3 &Vec3::operator*=(const float t)
{
    x *= t;
    y *= t;
    z *= t;
    return *this;
}

inline Vec3 &Vec3::operator/=(const float t) { return *this *= 1 / t; }

inline float Vec3::length() const { return std::sqrt(length_squared()); }

inline float Vec3::length_squared() const { return x * x + y * y + z * z; }

inline Vec3 randomInUnitSphere()
{
    while (true)
    {
        Vec3 p(
            static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
            static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
            static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f);
        if (p.length_squared() >= 1.0f)
            continue;
        return p;
    }
}

inline Vec3 randomUnitVector()
{
    return normalize(randomInUnitSphere());
}

inline Vec3 reflect(const Vec3 &v, const Vec3 &n)
{
    return v - 2.0f * dot(v, n) * n;
}
