//
// Created by xiaoxuetu on 2026/1/26.
//

#ifndef MATH_CSMATHUTILITY_H
#define MATH_CSMATHUTILITY_H

/* 有用的一些常数 */
#define CS_PI (3.1415926535897932f)


#define CS_KINDA_SMALL_NUMBER (1.e-4f)

#define CS_DELTA (0.00001f)

/* 角度和弧度互转 */
#define CS_AngleToRadian(angle) (angle * CS_PI / 180.0f)
#define CS_AngleToRadian(Radian) (Radian * 180.0f / CS_PI)

#endif //MATH_CSMATHUTILITY_H
