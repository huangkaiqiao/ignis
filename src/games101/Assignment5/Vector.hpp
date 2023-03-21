#pragma once

#include <cmath>
#include <iostream>

class Vector3f
{
public:
    Vector3f()
        : x(0)
        , y(0)
        , z(0)
    {}
    Vector3f(float xx)
        : x(xx)
        , y(xx)
        , z(xx)
    {}
    Vector3f(float xx, float yy, float zz)
        : x(xx)
        , y(yy)
        , z(zz)
    {}
    Vector3f operator*(const float& r) const
    {
        return Vector3f(x * r, y * r, z * r);
    }
    Vector3f operator/(const float& r) const
    {
        return Vector3f(x / r, y / r, z / r);
    }

    Vector3f operator*(const Vector3f& v) const
    {
        return Vector3f(x * v.x, y * v.y, z * v.z);
    }
    Vector3f operator-(const Vector3f& v) const
    {
        return Vector3f(x - v.x, y - v.y, z - v.z);
    }
    Vector3f operator+(const Vector3f& v) const
    {
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }
    Vector3f operator-() const
    {
        return Vector3f(-x, -y, -z);
    }
    Vector3f& operator+=(const Vector3f& v)
    {
        x += v.x, y += v.y, z += v.z;
        return *this;
    }
    friend Vector3f operator*(const float& r, const Vector3f& v)
    {
        return Vector3f(v.x * r, v.y * r, v.z * r);
    }
    friend std::ostream& operator<<(std::ostream& os, const Vector3f& v)
    {
        return os << v.x << ", " << v.y << ", " << v.z;
    }
    float x, y, z;
};

class Vector2f
{
public:
    Vector2f()
        : x(0)
        , y(0)
    {}
    Vector2f(float xx)
        : x(xx)
        , y(xx)
    {}
    Vector2f(float xx, float yy)
        : x(xx)
        , y(yy)
    {}
    Vector2f operator*(const float& r) const
    {
        return Vector2f(x * r, y * r);
    }
    Vector2f operator+(const Vector2f& v) const
    {
        return Vector2f(x + v.x, y + v.y);
    }
    float x, y;
};

inline Vector3f lerp(const Vector3f& a, const Vector3f& b, const float& t)
{
    return a * (1 - t) + b * t;
}

inline Vector3f normalize(const Vector3f& v)
{
    float mag2 = v.x * v.x + v.y * v.y + v.z * v.z;
    if (mag2 > 0)
    {
        float invMag = 1 / sqrtf(mag2);
        return Vector3f(v.x * invMag, v.y * invMag, v.z * invMag);
    }

    return v;
}

inline float dotProduct(const Vector3f& a, const Vector3f& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3f crossProduct(const Vector3f& a, const Vector3f& b)
{
    return Vector3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline void inv_mat(Vector3f* matrix, Vector3f* inverse){
    float tmp = 0.f;
    tmp = matrix[0].y; matrix[0].y = matrix[1].x; matrix[1].x = tmp;
    tmp = matrix[0].z; matrix[0].z = matrix[2].x; matrix[2].x = tmp;
    tmp = matrix[1].z; matrix[1].z = matrix[2].y; matrix[2].y = tmp;
    for(int i=0; i<3; i++){
        int j = (i+1)%3;
        int k = (i+2)%3;
        float x = matrix[j].y * matrix[k].z - matrix[j].z * matrix[k].y;
        float y = matrix[j].z * matrix[k].x - matrix[j].x * matrix[k].z;
        float z = matrix[j].x * matrix[k].y - matrix[j].y * matrix[k].x;
        // printf("x:%.2f, y:%.2f, z:%.2f\n", x, y, z);
        inverse[i] = Vector3f(x, y, z);
    }
    // Vector3f col = Vector3f(matrix[0].x, matrix[1].x, matrix[2].x);
    // printf("inv[0]:%.2f %.2f %.2f\n", inverse[0].x, inverse[0].y, inverse[0].z);
    float det = dotProduct(inverse[0], matrix[0]);
    for(int i=0; i<3; i++){
        inverse[i] = inverse[i]/det;
    }
}