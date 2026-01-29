//
// Created by xiaoxuetu on 2026/1/27.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#include "Math/scale_matrix.h"
#include  <cstring>

namespace CS
{
namespace Math
{
ScaleMatrix::ScaleMatrix(const Vector3d& Scale)
{
    M[0][0] *= Scale.x;
    M[1][1] *= Scale.y;
    M[2][2] *= Scale.z;
}

void ScaleMatrix::Make(const Vector3d& Scale)
{
    memset(M, 0, sizeof(M));
    M[0][0] = Scale.x;
    M[1][1] = Scale.y;
    M[2][2] = Scale.z;
    M[3][3] = 1.0f;
}
}
}
