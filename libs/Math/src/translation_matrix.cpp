//
// Created by xiaoxuetu on 2026/1/27.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#include "Math/translation_matrix.h"

namespace CS::Math
{
TranslationMatrix::TranslationMatrix(const Vector3d& Translation)
{
    M[0][3] = Translation.x;
    M[1][3] = Translation.y;
    M[2][3] = Translation.z;
}

void TranslationMatrix::Make(const Vector3d& Translation)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    M[0][3] = Translation.x;
    M[1][3] = Translation.y;
    M[2][3] = Translation.z;
}
}
