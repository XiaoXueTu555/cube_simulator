//
// Created by xiaoxuetu on 2026/1/27.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#include "Math/rotation_matrix.h"
#include <cmath>

namespace CS::Math
{
RotationMatrix::RotationMatrix(const Vector3d& Rotation)
{
    Make(Rotation);
}

void RotationMatrix::Make(const Vector3d& Rotation)
{
    float rx = Rotation.x;
    float ry = Rotation.y;
    float rz = Rotation.z;

    float cosX = cos(rx);
    float sinX = sin(rx);
    float cosY = cos(ry);
    float sinY = sin(ry);
    float cosZ = cos(rz);
    float sinZ = sin(rz);

    M[0][0] = cosZ * cosY;
    M[0][1] = cosZ * sinY * sinX - sinZ * cosX;
    M[0][2] = cosZ * sinY * cosX + sinZ * sinX;
    M[0][3] = 0.0f;

    M[1][0] = sinZ * cosY;
    M[1][1] = sinZ * sinY * sinX + cosZ * cosX;
    M[1][2] = sinZ * sinY * cosX - cosZ * sinX;
    M[1][3] = 0.0f;

    M[2][0] = -sinY;
    M[2][1] = cosY * sinX;
    M[2][2] = cosY * cosX;
    M[2][3] = 0.0f;

    M[3][0] = 0.0f;
    M[3][1] = 0.0f;
    M[3][2] = 0.0f;
    M[3][3] = 1.0f;
}
}
