//
// Created by xiaoxuetu on 2026/1/26.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#include "Math/vector3d.h"

#include <cmath>
#include <stdexcept>
// #include <bits/functexcept.h>
// #include <bits/valarray_after.h>

namespace CS
{
namespace Math
{
const Vector3d Vector3d::ZeroVector{0, 0, 0};
const Vector3d Vector3d::UpVector{0, 1, 0};
const Vector3d Vector3d::RightVector{1, 0, 0};
const Vector3d Vector3d::ForwardVector{0, 0, -1};
const Vector3d Vector3d::XAxisVector{1, 0, 0};
const Vector3d Vector3d::YAxisVector{0, 1, 0};
const Vector3d Vector3d::ZAxisVector{0, 0, 1};

Vector3d::Vector3d(float x, float y, float z): x(x), y(y), z(z) {}

Vector3d::Vector3d(std::initializer_list<float> list)
{
    if (list.size() != 3)
        throw std::invalid_argument("Logic error:Vector3d requires exactly 3 elements");

    x = list.begin()[0];
    y = list.begin()[1];
    z = list.begin()[2];
}

float Vector3d::Dot(const Vector3d& V) const
{
    return this->x * V.x + this->y * V.y + this->z * V.z;
}

float Vector3d::DotProduct(const Vector3d& A, const Vector3d& B)
{
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

Vector3d Vector3d::Cross(const Vector3d& V) const
{
    return Vector3d{
        this->y * V.z - this->z * V.y,
        this->z * V.x - this->x * V.z,
        this->x * V.y - this->y * V.x
    };
}

Vector3d Vector3d::CrossProduct(const Vector3d& A, const Vector3d& B)
{
    return Vector3d{
        A.y * B.z - A.z * B.y,
        A.z * B.x - A.x * B.z,
        A.x * B.y - A.y * B.x
    };
}

void Vector3d::Normalize()
{
    float len = this->Length();
    if (len == 0)
        return;
    this->x /= len;
    this->y /= len;
    this->z /= len;
}

void Vector3d::Set(float InX, float InY, float InZ)
{
    this->x = InX;
    this->y = InY;
    this->z = InZ;
}

float Vector3d::Length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

Vector3d Vector3d::operator+(const Vector3d& V) const
{
    return Vector3d{this->x + V.x, this->y + V.y, this->z + V.z};
}

Vector3d& Vector3d::operator+=(const Vector3d& V)
{
    this->x += V.x;
    this->y += V.y;
    this->z += V.z;
    return *this;
}

Vector3d Vector3d::operator-(const Vector3d& V) const
{
    return Vector3d{this->x - V.x, this->y - V.y, this->z - V.z};
}

Vector3d& Vector3d::operator-=(const Vector3d& V)
{
    this->x -= V.x;
    this->y -= V.y;
    this->z -= V.z;
    return *this;
}

Vector3d Vector3d::operator*(float Scale) const
{
    return Vector3d{this->x * Scale, this->y * Scale, this->z * Scale};
}

Vector3d& Vector3d::operator*=(float Scale)
{
    this->x *= Scale;
    this->y *= Scale;
    this->z *= Scale;
    return *this;
}

Vector3d Vector3d::operator/(float Scale) const
{
    if (Scale <= 1e-4f)
        throw std::invalid_argument("Logic error:The divisor in vector division cannot be zero");
    return Vector3d{this->x / Scale, this->y / Scale, this->z / Scale};
}

Vector3d& Vector3d::operator/=(float Scale)
{
    if (Scale <= 1e-4f)
        throw std::invalid_argument("Logic error:The divisor in vector division cannot be zero");
    this->x /= Scale;
    this->y /= Scale;
    this->z /= Scale;
    return *this;
}

bool Vector3d::Equal(const Vector3d& V, float Tolerance) const
{
    if (std::fabs(x - V.x) <= Tolerance && std::fabs(y - V.y) <= Tolerance && std::fabs(z - V.z) <= Tolerance)
        return true;
    else
        return false;
}

bool Vector3d::operator==(const Vector3d& V) const
{
    return (x == V.x && y == V.y && z == V.z);
}

Vector3d operator*(float Scale, const Vector3d& V)
{
    return Vector3d{V.x * Scale, V.y * Scale, V.z * Scale};
}
}
}
