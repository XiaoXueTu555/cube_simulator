//
// Created by xiaoxuetu on 2026/1/27.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#ifndef MATH_TRANSLATION_MATRIX_H
#define MATH_TRANSLATION_MATRIX_H

#include "matrix.h"

namespace CS::Math
{

/* 平移矩阵 */
class TranslationMatrix : public Matrix
{
public:
    using Matrix::M;

    /*
     *  构造平移矩阵
     *  @param Translation 偏移量（或者世界坐标）
     */
    TranslationMatrix(const Vector3d& Translation);

    /*
     *  构造平移矩阵
     *  @param Translation 偏移量（或者世界坐标）
     */
    void Make(const Vector3d& Translation);
};
}
#endif //MATH_TRANSLATION_MATRIX_H
