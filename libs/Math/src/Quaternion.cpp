//
// Created by QuAn on 26-2-5.
//

#include "Math/quaternion.h"
#include "Math/matrix.h"
#include <cmath>
#include <stdexcept>

namespace CS::Math
{
Quaternion::Quaternion()
{
    this -> MakeIdentity();
}

Quaternion::Quaternion(float a, float b, float c, float d): a(a), b(b), c(c), d(d){}

Quaternion::Quaternion(std::initializer_list<float> list)
{
    if (list.size() != 4)
        throw std::invalid_argument("Logic error:Quaternion requires exactly 4 elements");

    a = list.begin()[0];
    b = list.begin()[1];
    c = list.begin()[2];
    d = list.begin()[3];
}

Quaternion Quaternion::operator+(const Quaternion &other) const
{
    return Quaternion{this->a + other.a, this->b + other.b, this->c + other.c, this->d + other.d};
}

Quaternion& Quaternion::operator+=(const Quaternion &other)
{
    this->a += other.a;
    this->b += other.b;
    this->c += other.c;
    this->d += other.d;
    return *this;
}

Quaternion Quaternion::operator-(const Quaternion &other) const
{
    return Quaternion{this->a - other.a, this->b - other.b, this->c - other.c, this->d - other.d};
}

Quaternion &Quaternion::operator-=(const Quaternion &other)
{
    this->a -= other.a;
    this->b -= other.b;
    this->c -= other.c;
    this->d -= other.d;
    return *this;
}

Quaternion Quaternion::operator*(const Quaternion &other) const
{
    return Quaternion{a * other.a - b * other.b - c * other.c - d * other.d,
                      b * other.a + a * other.b - d * other.c + c * other.d,
                      c * other.a + d * other.b + a * other.c - b * other.d,
                      d * other.a - c * other.b + b * other.c + a * other.d};
}

Quaternion& Quaternion::operator*=(const Quaternion &other)
{
    *this = *this * other;
    return *this;
}

Quaternion Quaternion::operator*(float scalar)const
{
    return Quaternion{this->a * scalar, this->b * scalar, this->c * scalar, this->d * scalar};
}

Quaternion& Quaternion::operator*=(float scalar)
{
    *this = *this * scalar;
    return *this;
}

Quaternion Quaternion::operator/(float scalar) const
{
    if (std::fabs(scalar) <= CS_KINDA_SMALL_NUMBER)
        throw std::invalid_argument("Logic error:The divisor in Quaternion division cannot be zero");
    return Quaternion{this->a / scalar, this->b / scalar, this->c / scalar, this->d / scalar};
}

Quaternion& Quaternion::operator/=(float scalar)
{
    if (std::fabs(scalar) <= CS_KINDA_SMALL_NUMBER)
        throw std::invalid_argument("Logic error:The divisor in Quaternion division cannot be zero");
    *this = *this / scalar;
    return *this;
}

void Quaternion::MakeIdentity()
{
    this->a = 1.0f;
    this->b = 0.0f;
    this->c = 0.0f;
    this->d = 0.0f;
}

void Quaternion::Set(float a, float b, float c, float d)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
}

void Quaternion::Normalize()
{
    float len = this->Length();
    if (std::fabs(len) <= CS_KINDA_SMALL_NUMBER)
    {
        this-> MakeIdentity();
        return;
    }
    this->a /= len;
    this->b /= len;
    this->c /= len;
    this->d /= len;
}

Quaternion Quaternion::FromAxisAngle(Vector3d& axis, float angle)
{
    float halfAngle = angle * 0.5f;
    float sinHalf = sin(halfAngle);
    float cosHalf = cos(halfAngle);

    axis.Normalize();

    return Quaternion{cosHalf,axis.x * sinHalf, axis.y * sinHalf, axis.z * sinHalf};
}

Quaternion Quaternion::FromEulerAngle(Rotator& Rotation)
{
    float cosZ = cos(0.5f * Rotation.yaw);
    float sinZ = sin(0.5f * Rotation.yaw);
    float cosY = cos(0.5f * Rotation.pitch);
    float sinY = sin(0.5f * Rotation.pitch);
    float cosX = cos(0.5f * Rotation.roll);
    float sinX = sin(0.5f * Rotation.roll);

    a = cosX * cosY * cosZ - sinX * sinY * sinZ,
    b = sinX * cosY * cosZ + cosX * sinY * sinZ,
    c = cosX * sinY * cosZ - sinX * cosY * sinZ,
    d = cosX * cosY * sinZ + sinX * sinY * cosZ;
}

float Quaternion::Length() const
{
    return std::sqrt(a * a + b * b + c * c + d * d);
}

float Quaternion::Dot(const Quaternion &other) const
{
    return a * other.a + b * other.b + c * other.c + d * other.d;
}


Quaternion Quaternion::conjugate() const
{
    return Quaternion{this->a, -this->b, -this->c, -this->d};
}

Quaternion Quaternion::Inverse() const
{
    if (std::fabs(this->Length()) <= CS_KINDA_SMALL_NUMBER)
        throw std::invalid_argument("Logic error:The divisor in Quaternion division cannot be zero");
    return this->conjugate() / (this->Length() * this->Length());
}

Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
    Quaternion result{
        q1.a + t * (q2.a - q1.a),
        q1.b + t * (q2.b - q1.b),
        q1.c + t * (q2.c - q1.c),
        q1.d + t * (q2.d - q1.d)
    };
    return result;
}

Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
    t = (t < 0.0f) ? 0.0f : ((t > 1.0f) ? 1.0f : t);
    const float DOT_THRESHOLD = 0.9995f;

    float dot = q1.Dot(q2);

    Quaternion q2Adjusted = q2;
    if (dot < 0.0f)
    {
        q2Adjusted = Quaternion{-q2.a, -q2.b, -q2.c, -q2.d};
        dot = -dot;
    }

    if (dot > DOT_THRESHOLD)
    {
        // 四元数非常接近，使用线性插值避免除零
        Quaternion result = Quaternion{
            q1.a + t * (q2Adjusted.a - q1.a),
            q1.b + t * (q2Adjusted.b - q1.b),
            q1.c + t * (q2Adjusted.c - q1.c),
            q1.d + t * (q2Adjusted.d - q1.d)
        };
        result.Normalize();
        return result;
    }

    // 计算插值角度
    float theta0 = acosf(dot);
    float theta = theta0 * t;

    float sinTheta = sinf(theta);
    float sinTheta0 = sinf(theta0);

    float s0 = cosf(theta) - dot * sinTheta / sinTheta0;
    float s1 = sinTheta / sinTheta0;

    return Quaternion{
        s0 * q1.a + s1 * q2Adjusted.a,
        s0 * q1.b + s1 * q2Adjusted.b,
        s0 * q1.c + s1 * q2Adjusted.c,
        s0 * q1.d + s1 * q2Adjusted.d
    };
}
}
