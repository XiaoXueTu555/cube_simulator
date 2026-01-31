//
// Created by xiaoxuetu on 2026/1/27.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#include "Math/perspective_projection_matrix.h"
#include <cstring>
#include <cmath>

namespace CS::Math
{
PerspectiveProjectionMatrix::PerspectiveProjectionMatrix(float FOV, float Near, float Far, float AspectRation)
{
    Make(FOV, Near, Far, AspectRation);
}

void PerspectiveProjectionMatrix::Make(float FOV, float Near, float Far, float AspectRation)
{
    memset(M, 0, sizeof(M));
    float t = Near * tan(FOV / 2);
    float r = AspectRation * t;

    M[0][0] = Near / r;
    M[1][1] = Near / t;
    M[2][2] = (Far + Near) / (Near - Far);
    M[2][3] = 2.0f * Far * Near / (Near - Far);
    M[3][2] = -1.0f;
}
}
