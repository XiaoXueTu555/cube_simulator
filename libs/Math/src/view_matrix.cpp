//
// Created by xiaoxuetu on 2026/1/27.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#include "Math/view_matrix.h"

namespace CS
{
namespace Math
{
ViewMatrix::ViewMatrix(const Vector3d& EyePosition, const Vector3d& LookAtPosition, const Vector3d& UpVector)
{
    Make(EyePosition, LookAtPosition, UpVector);
}

void ViewMatrix::Make(const Vector3d& EyePosition, const Vector3d& LookAtPosition, const Vector3d& UpVector)
{
    Vector3d Zcam = EyePosition - LookAtPosition;
    Zcam.Normalize();
    Vector3d Xcam = UpVector.Cross(Zcam);
    Xcam.Normalize();
    Vector3d Ycam = Zcam.Cross(Xcam);

    M[0][0] = Xcam.x;
    M[0][1] = Xcam.y;
    M[0][2] = Xcam.z;
    M[0][3] = -EyePosition.Dot(Xcam);

    M[1][0] = Ycam.x;
    M[1][1] = Ycam.y;
    M[1][2] = Ycam.z;
    M[1][3] = -EyePosition.Dot(Ycam);

    M[2][0] = Zcam.x;
    M[2][1] = Zcam.y;
    M[2][2] = Zcam.z;
    M[2][3] = -EyePosition.Dot(Zcam);

    M[3][0] = 0.0f;
    M[3][1] = 0.0f;
    M[3][2] = 0.0f;
    M[3][3] = 1.0f;
}
}
}
