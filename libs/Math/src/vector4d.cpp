//
// Created by xiaoxuetu on 2026/1/26.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#include "Math/vector4d.h"

#include <cmath>
#include <stdexcept>

namespace CS
{
namespace Math
{
Vector4d::Vector4d(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {}

Vector4d::Vector4d(std::initializer_list<float> list)
{
    if (list.size() != 4)
        throw std::invalid_argument("Logic error:Vector4d requires exactly 4 elements");

    x = list.begin()[0];
    y = list.begin()[1];
    z = list.begin()[2];
    w = list.begin()[3];
}

Vector4d::Vector4d(const Vector3d& InVector, float InW)
{
    this->x = InVector.x;
    this->y = InVector.y;
    this->z = InVector.z;
    this->w = InW;
}

float Vector4d::Dot(const Vector4d& V) const
{
    return this->x * V.x + this->y * V.y + this->z * V.z + this->w * V.w;
}

float Vector4d::DotProduct(const Vector4d& A, const Vector4d& B)
{
    return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
}

void Vector4d::Normalize()
{
    float len = this->Length();
    if (len == 0)
        return;
    this->x /= len;
    this->y /= len;
    this->z /= len;
    this->w /= len;
}

void Vector4d::Set(float InX, float InY, float InZ, float InW)
{
    this->x = InX;
    this->y = InY;
    this->z = InZ;
    this->w = InW;
}

float Vector4d::Length() const
{
    return std::sqrt(x * x + y * y + z * z + w * w);
}

Vector4d Vector4d::operator+(const Vector4d& V) const
{
    return Vector4d{this->x + V.x, this->y + V.y, this->z + V.z, this->w + V.w};
}

Vector4d& Vector4d::operator+=(const Vector4d& V)
{
    this->x += V.x;
    this->y += V.y;
    this->z += V.z;
    this->w += V.w;
    return *this;
}

Vector4d Vector4d::operator-(const Vector4d& V) const
{
    return Vector4d{this->x - V.x, this->y - V.y, this->z - V.z, this->w - V.w};
}

Vector4d& Vector4d::operator-=(const Vector4d& V)
{
    this->x -= V.x;
    this->y -= V.y;
    this->z -= V.z;
    this->w -= V.w;
    return *this;
}

Vector4d Vector4d::operator*(float Scale) const
{
    return Vector4d{this->x * Scale, this->y * Scale, this->z * Scale, this->w * Scale};
}

Vector4d& Vector4d::operator*=(float Scale)
{
    this->x *= Scale;
    this->y *= Scale;
    this->z *= Scale;
    this->w *= Scale;
    return *this;
}

Vector4d Vector4d::operator/(float Scale) const
{
    if (Scale <= 1e-4f)
        throw std::invalid_argument("Logic error:The divisor in vector division cannot be zero");
    return Vector4d{this->x / Scale, this->y / Scale, this->z / Scale, this->w / Scale};
}

Vector4d& Vector4d::operator/=(float Scale)
{
    if (Scale <= 1e-4f)
        throw std::invalid_argument("Logic error:The divisor in vector division cannot be zero");
    this->x /= Scale;
    this->y /= Scale;
    this->z /= Scale;
    this->w /= Scale;
    return *this;
}

bool Vector4d::Equal(const Vector4d& V, float Tolerance) const
{
    if (std::fabs(x - V.x) <= Tolerance && std::fabs(y - V.y) <= Tolerance && std::fabs(z - V.z) <= Tolerance &&
        fabs(w - V.w) <= Tolerance)
        return true;

    return false;
}

bool Vector4d::operator==(const Vector4d& V) const
{
    return (x == V.x && y == V.y && z == V.z && w == V.w);
}

Vector4d operator*(float Scale, const Vector4d& V)
{
    return Vector4d{V.x * Scale, V.y * Scale, V.z * Scale, V.w * Scale};
}
}
}
