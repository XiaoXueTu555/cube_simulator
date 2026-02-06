//
// Created by xiaoxuetu on 2026/1/27.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#include "Math/rotation_matrix.h"
#include "Math/rotator.h"
#include "Math/Quaternion.h"
#include <cmath>

namespace CS::Math
{
RotationMatrix::RotationMatrix(const Quaternion& q)
{
    Make(q);
}

void RotationMatrix::Make(const Quaternion& q)
{
    Quaternion Q = q;
    Q.Normalize();

    float bb = Q.b * Q.b;
    float cc = Q.c * Q.c;
    float dd = Q.d * Q.d;
    float bc = Q.b * Q.c;
    float bd = Q.b * Q.d;
    float cd = Q.c * Q.d;
    float ab = Q.a * Q.b;
    float ac = Q.a * Q.c;
    float ad = Q.a * Q.d;

    M[0][0] = 1.0f - 2.0f * (cc + dd);
    M[0][1] = 2.0f * (bc - ad);
    M[0][2] = 2.0f * (bd + ac);
    M[0][3] = 0.0f;

    M[1][0] = 2.0f * (bc + ad);
    M[1][1] = 1.0f - 2.0f * (bb + dd);
    M[1][2] = 2.0f * (cd - ab);
    M[1][3] = 0.0f;

    M[2][0] = 2.0f * (bd - ac);
    M[2][1] = 2.0f * (cd + ab);
    M[2][2] = 1.0f - 2.0f * (bb + cc);
    M[2][3] = 0.0f;

    M[3][0] = 0.0f;
    M[3][1] = 0.0f;
    M[3][2] = 0.0f;
    M[3][3] = 1.0f;
}

}
