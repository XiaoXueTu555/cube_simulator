//
// Created by QuAn on 26-2-5.
//
#include "Math/rotator.h"

#include <stdexcept>

#include "Math/Quaternion.h"

namespace CS::Math
{
Rotator::Rotator()
{
    this->MakeIdentity();
}

Rotator::Rotator(float InZAngle, float InYAngle, float InXAngle): yaw(InZAngle), pitch(InYAngle), roll(InXAngle){}

Rotator::Rotator(const Vector3d &V): yaw(V.z), pitch(V.y), roll(V.x){}

Rotator::Rotator(std::initializer_list<float> list)
{
    if (list.size() != 3)
        throw std::invalid_argument("Logic error:Rotator requires exactly 3 elements");

    yaw = list.begin()[2];
    pitch = list.begin()[1];
    roll = list.begin()[0];
}

void Rotator::set(float InZAngle, float InYAngle, float InXAngle)
{
    this->yaw = InZAngle;
    this->pitch = InYAngle;
    this->roll = InXAngle;
}

void Rotator::MakeIdentity()
{
    this->yaw = 0.0f;
    this->pitch = 0.0f;
    this->roll = 0.0f;
}

}
