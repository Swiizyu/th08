#include "th_pch.h"

#ifdef DEBUG
#include <cstdarg>
#include <stdio.h>
#endif

#include "utils.hpp"
#include <math.h>
#include <stdio.h>

BOOL WithinEpsilon(FLOAT a, FLOAT b)
{
    return fabsf(a - b) <= 1.0e-6f;
}

namespace th08
{

D3DXVECTOR2 *WINAPI D3DXVec2Hermite(D3DXVECTOR2 *out, const D3DXVECTOR2 *v1, const D3DXVECTOR2 *t1,
                                     const D3DXVECTOR2 *v2, const D3DXVECTOR2 *t2, FLOAT s)
{
    FLOAT s2 = s * s, s3 = s2 * s;
    FLOAT h1 = 2.0f * s3 - 3.0f * s2 + 1.0f;
    FLOAT h2 = s3 - 2.0f * s2 + s;
    FLOAT h3 = -2.0f * s3 + 3.0f * s2;
    FLOAT h4 = s3 - s2;
    out->x = h1 * v1->x + h2 * t1->x + h3 * v2->x + h4 * t2->x;
    out->y = h1 * v1->y + h2 * t1->y + h3 * v2->y + h4 * t2->y;
    return out;
}

D3DXVECTOR2 *WINAPI D3DXVec2CatmullRom(D3DXVECTOR2 *out, const D3DXVECTOR2 *v0, const D3DXVECTOR2 *v1,
                                        const D3DXVECTOR2 *v2, const D3DXVECTOR2 *v3, FLOAT s)
{
    FLOAT s2 = s * s, s3 = s2 * s;
    out->x = 0.5f * (2.0f * v1->x + (-v0->x + v2->x) * s +
                      (2.0f * v0->x - 5.0f * v1->x + 4.0f * v2->x - v3->x) * s2 +
                      (-v0->x + 3.0f * v1->x - 3.0f * v2->x + v3->x) * s3);
    out->y = 0.5f * (2.0f * v1->y + (-v0->y + v2->y) * s +
                      (2.0f * v0->y - 5.0f * v1->y + 4.0f * v2->y - v3->y) * s2 +
                      (-v0->y + 3.0f * v1->y - 3.0f * v2->y + v3->y) * s3);
    return out;
}

D3DXVECTOR2 *WINAPI D3DXVec2BaryCentric(D3DXVECTOR2 *out, const D3DXVECTOR2 *v1, const D3DXVECTOR2 *v2,
                                         const D3DXVECTOR2 *v3, FLOAT f, FLOAT g)
{
    out->x = v1->x + f * (v2->x - v1->x) + g * (v3->x - v1->x);
    out->y = v1->y + f * (v2->y - v1->y) + g * (v3->y - v1->y);
    return out;
}

D3DXVECTOR3 *WINAPI D3DXVec3Hermite(D3DXVECTOR3 *out, const D3DXVECTOR3 *v1, const D3DXVECTOR3 *t1,
                                     const D3DXVECTOR3 *v2, const D3DXVECTOR3 *t2, FLOAT s)
{
    FLOAT s2 = s * s, s3 = s2 * s;
    FLOAT h1 = 2.0f * s3 - 3.0f * s2 + 1.0f;
    FLOAT h2 = s3 - 2.0f * s2 + s;
    FLOAT h3 = -2.0f * s3 + 3.0f * s2;
    FLOAT h4 = s3 - s2;
    out->x = h1 * v1->x + h2 * t1->x + h3 * v2->x + h4 * t2->x;
    out->y = h1 * v1->y + h2 * t1->y + h3 * v2->y + h4 * t2->y;
    out->z = h1 * v1->z + h2 * t1->z + h3 * v2->z + h4 * t2->z;
    return out;
}

D3DXVECTOR3 *WINAPI D3DXVec3CatmullRom(D3DXVECTOR3 *out, const D3DXVECTOR3 *v0, const D3DXVECTOR3 *v1,
                                        const D3DXVECTOR3 *v2, const D3DXVECTOR3 *v3, FLOAT s)
{
    FLOAT s2 = s * s, s3 = s2 * s;
    out->x = 0.5f * (2.0f * v1->x + (-v0->x + v2->x) * s +
                      (2.0f * v0->x - 5.0f * v1->x + 4.0f * v2->x - v3->x) * s2 +
                      (-v0->x + 3.0f * v1->x - 3.0f * v2->x + v3->x) * s3);
    out->y = 0.5f * (2.0f * v1->y + (-v0->y + v2->y) * s +
                      (2.0f * v0->y - 5.0f * v1->y + 4.0f * v2->y - v3->y) * s2 +
                      (-v0->y + 3.0f * v1->y - 3.0f * v2->y + v3->y) * s3);
    out->z = 0.5f * (2.0f * v1->z + (-v0->z + v2->z) * s +
                      (2.0f * v0->z - 5.0f * v1->z + 4.0f * v2->z - v3->z) * s2 +
                      (-v0->z + 3.0f * v1->z - 3.0f * v2->z + v3->z) * s3);
    return out;
}

char *__fastcall ConvertToFullWidthDigits(i32 value, i32 digits)
{
    i32 place;
    i32 i;

    place = 1;
    memset((void *)0x18bddc8, 0, 0x40);
    for (i = 1; i < digits; i++)
    {
        place *= 10;
    }
    i = 0;
    while (place > 0)
    {
        strcpy((char *)0x18bddc8 + i, *(const char **)(0x4c829c + (value / place) * 4));
        value %= place;
        place /= 10;
        i += 2;
    }
    return (char *)0x18bddc8;
}

// FUNCTION: th08 0x477024
D3DXVECTOR3 *WINAPI FUN_00477024(D3DXVECTOR3 *out, const D3DXVECTOR3 *v1, const D3DXVECTOR3 *v2,
                                 const D3DXVECTOR3 *v3, FLOAT f, FLOAT g)
{
    return ::D3DXVec3BaryCentric(out, v1, v2, v3, f, g);
}

// FUNCTION: th08 0x477081
D3DXVECTOR4 *WINAPI FUN_00477081(D3DXVECTOR4 *out, const D3DXVECTOR3 *v, const D3DXMATRIX *matrix)
{
    return ::D3DXVec3Transform(out, v, matrix);
}

// FUNCTION: th08 0x47717e
D3DXVECTOR4 *WINAPI FUN_0047717e(D3DXVECTOR4 *out, const D3DXVECTOR4 *v1, const D3DXVECTOR4 *v2,
                                 const D3DXVECTOR4 *v3)
{
    return ::D3DXVec4Cross(out, v1, v2, v3);
}

// FUNCTION: th08 0x47726e
D3DXVECTOR4 *WINAPI FUN_0047726e(D3DXVECTOR4 *out, const D3DXVECTOR4 *v1, const D3DXVECTOR4 *t1,
                                 const D3DXVECTOR4 *v2, const D3DXVECTOR4 *t2, FLOAT s)
{
    return ::D3DXVec4Hermite(out, v1, t1, v2, t2, s);
}

// FUNCTION: th08 0x47733f
D3DXVECTOR4 *WINAPI FUN_0047733f(D3DXVECTOR4 *out, const D3DXVECTOR4 *v0, const D3DXVECTOR4 *v1,
                                 const D3DXVECTOR4 *v2, const D3DXVECTOR4 *v3, FLOAT s)
{
    return ::D3DXVec4CatmullRom(out, v0, v1, v2, v3, s);
}

// FUNCTION: th08 0x477432
D3DXVECTOR4 *WINAPI FUN_00477432(D3DXVECTOR4 *out, const D3DXVECTOR4 *v1, const D3DXVECTOR4 *v2,
                                 const D3DXVECTOR4 *v3, FLOAT f, FLOAT g)
{
    return ::D3DXVec4BaryCentric(out, v1, v2, v3, f, g);
}

// FUNCTION: th08 0x4774a9
D3DXVECTOR4 *WINAPI FUN_004774a9(D3DXVECTOR4 *out, const D3DXVECTOR4 *v, const D3DXMATRIX *matrix)
{
    return ::D3DXVec4Transform(out, v, matrix);
}

// FUNCTION: th08 0x477548
FLOAT WINAPI FUN_00477548(const D3DXMATRIX *matrix)
{
    D3DXMATRIX inverse;
    FLOAT determinant;
    ::D3DXMatrixInverse(&inverse, &determinant, matrix);
    return determinant;
}

// FUNCTION: th08 0x477618
D3DXMATRIX *WINAPI FUN_00477618(D3DXMATRIX *out, const D3DXMATRIX *m1, const D3DXMATRIX *m2)
{
    return ::D3DXMatrixMultiply(out, m1, m2);
}

// FUNCTION: th08 0x477854
D3DXMATRIX *WINAPI FUN_00477854(D3DXMATRIX *out, const D3DXMATRIX *matrix)
{
    return ::D3DXMatrixTranspose(out, matrix);
}

// FUNCTION: th08 0x4778c1
D3DXMATRIX *WINAPI FUN_004778c1(D3DXMATRIX *out, const D3DXMATRIX *m1, const D3DXMATRIX *m2)
{
    return ::D3DXMatrixMultiplyTranspose(out, m1, m2);
}

// FUNCTION: th08 0x477afd
D3DXMATRIX *WINAPI FUN_00477afd(D3DXMATRIX *out, FLOAT *determinant, const D3DXMATRIX *matrix)
{
    return ::D3DXMatrixInverse(out, determinant, matrix);
}

// FUNCTION: th08 0x477ee6
D3DXMATRIX *WINAPI FUN_00477ee6(D3DXMATRIX *out, FLOAT x, FLOAT y, FLOAT z)
{
    return ::D3DXMatrixScaling(out, x, y, z);
}

// FUNCTION: th08 0x477f48
D3DXMATRIX *WINAPI FUN_00477f48(D3DXMATRIX *out, FLOAT angle)
{
    return ::D3DXMatrixRotationX(out, angle);
}

// FUNCTION: th08 0x477fc8
D3DXMATRIX *WINAPI FUN_00477fc8(D3DXMATRIX *out, FLOAT angle)
{
    return ::D3DXMatrixRotationY(out, angle);
}

// FUNCTION: th08 0x478049
D3DXMATRIX *WINAPI FUN_00478049(D3DXMATRIX *out, FLOAT angle)
{
    return ::D3DXMatrixRotationZ(out, angle);
}

// FUNCTION: th08 0x4780c5
D3DXMATRIX *WINAPI FUN_004780c5(D3DXMATRIX *out, const D3DXVECTOR3 *axis, FLOAT angle)
{
    return ::D3DXMatrixRotationAxis(out, axis, angle);
}

// FUNCTION: th08 0x4781cd
D3DXMATRIX *WINAPI FUN_004781cd(D3DXMATRIX *out, const D3DXQUATERNION *rotation)
{
    return ::D3DXMatrixRotationQuaternion(out, rotation);
}

// FUNCTION: th08 0x4782af
D3DXMATRIX *WINAPI FUN_004782af(D3DXMATRIX *out, FLOAT scaling, const D3DXVECTOR3 *rotationCenter,
                                 const D3DXQUATERNION *rotation, const D3DXVECTOR3 *translation)
{
    return ::D3DXMatrixAffineTransformation(out, scaling, rotationCenter, rotation, translation);
}

// FUNCTION: th08 0x478395
D3DXMATRIX *WINAPI D3DXMatrixLookAtLH(D3DXMATRIX *out, const D3DXVECTOR3 *eye, const D3DXVECTOR3 *at,
                                      const D3DXVECTOR3 *up)
{
    return ::D3DXMatrixLookAtLH(out, eye, at, up);
}

// FUNCTION: th08 0x4784dd
D3DXMATRIX *WINAPI D3DXMatrixPerspectiveFovLH(D3DXMATRIX *out, FLOAT fovY, FLOAT aspect, FLOAT zn, FLOAT zf)
{
    return ::D3DXMatrixPerspectiveFovLH(out, fovY, aspect, zn, zf);
}

// FUNCTION: th08 0x478571
void WINAPI FUN_00478571(const D3DXQUATERNION *q, D3DXVECTOR3 *axis, FLOAT *angle)
{
    ::D3DXQuaternionToAxisAngle(q, axis, angle);
}

// FUNCTION: th08 0x47860e
D3DXQUATERNION *WINAPI FUN_0047860e(D3DXQUATERNION *out, const D3DXMATRIX *matrix)
{
    return ::D3DXQuaternionRotationMatrix(out, matrix);
}

// FUNCTION: th08 0x478742
D3DXQUATERNION *WINAPI FUN_00478742(D3DXQUATERNION *out, const D3DXVECTOR3 *axis, FLOAT angle)
{
    return ::D3DXQuaternionRotationAxis(out, axis, angle);
}

// FUNCTION: th08 0x4787a4
D3DXQUATERNION *WINAPI FUN_004787a4(D3DXQUATERNION *out, FLOAT yaw, FLOAT pitch, FLOAT roll)
{
    return ::D3DXQuaternionRotationYawPitchRoll(out, yaw, pitch, roll);
}

// FUNCTION: th08 0x478883
D3DXQUATERNION *WINAPI FUN_00478883(D3DXQUATERNION *out, const D3DXQUATERNION *q1,
                                    const D3DXQUATERNION *q2)
{
    return ::D3DXQuaternionMultiply(out, q1, q2);
}

// FUNCTION: th08 0x47891f
D3DXQUATERNION *WINAPI FUN_0047891f(D3DXQUATERNION *out, const D3DXQUATERNION *q)
{
    return ::D3DXQuaternionNormalize(out, q);
}

// FUNCTION: th08 0x478afe
D3DXQUATERNION *WINAPI FUN_00478afe(D3DXQUATERNION *out, const D3DXQUATERNION *q1,
                                    const D3DXQUATERNION *q2, FLOAT t)
{
    return ::D3DXQuaternionSlerp(out, q1, q2, t);
}

// FUNCTION: th08 0x478be1
D3DXQUATERNION *WINAPI FUN_00478be1(D3DXQUATERNION *out, const D3DXQUATERNION *q1,
                                    const D3DXQUATERNION *q2, const D3DXQUATERNION *q3, FLOAT f, FLOAT g)
{
    return ::D3DXQuaternionBaryCentric(out, q1, q2, q3, f, g);
}

// FUNCTION: th08 0x478c38
D3DXQUATERNION *WINAPI FUN_00478c38(D3DXQUATERNION *out, const D3DXQUATERNION *q1,
                                    const D3DXQUATERNION *q2, const D3DXQUATERNION *q3,
                                    const D3DXQUATERNION *q4, FLOAT t)
{
    return ::D3DXQuaternionSquad(out, q1, q2, q3, q4, t);
}

// FUNCTION: th08 0x478cc9
D3DXQUATERNION *WINAPI FUN_00478cc9(D3DXQUATERNION *out, const D3DXQUATERNION *q)
{
    return ::D3DXQuaternionExp(out, q);
}

// FUNCTION: th08 0x478d6d
D3DXVECTOR3 *WINAPI FUN_00478d6d(D3DXVECTOR3 *out, const D3DXPLANE *plane,
                                 const D3DXVECTOR3 *p1, const D3DXVECTOR3 *p2)
{
    return ::D3DXPlaneIntersectLine(out, plane, p1, p2);
}

// FUNCTION: th08 0x478e47
D3DXPLANE *WINAPI FUN_00478e47(D3DXPLANE *out, const D3DXVECTOR3 *p1,
                               const D3DXVECTOR3 *p2, const D3DXVECTOR3 *p3)
{
    return ::D3DXPlaneFromPoints(out, p1, p2, p3);
}

// FUNCTION: th08 0x478ee9
D3DXPLANE *WINAPI FUN_00478ee9(D3DXPLANE *out, const D3DXPLANE *plane, const D3DXMATRIX *matrix)
{
    return ::D3DXPlaneTransform(out, plane, matrix);
}

// FUNCTION: th08 0x478f88
D3DXPLANE *WINAPI FUN_00478f88(D3DXPLANE *out, const D3DXPLANE *plane)
{
    return ::D3DXPlaneNormalize(out, plane);
}

// FUNCTION: th08 0x479015
D3DXVECTOR2 *WINAPI FUN_00479015(D3DXVECTOR2 *out, const D3DXVECTOR2 *v1, const D3DXVECTOR2 *t1,
                                 const D3DXVECTOR2 *v2, const D3DXVECTOR2 *t2, FLOAT s)
{
    return th08::D3DXVec2Hermite(out, v1, t1, v2, t2, s);
}

// FUNCTION: th08 0x47903e
D3DXVECTOR2 *WINAPI FUN_0047903e(D3DXVECTOR2 *out, const D3DXVECTOR2 *v0, const D3DXVECTOR2 *v1,
                                 const D3DXVECTOR2 *v2, const D3DXVECTOR2 *v3, FLOAT s)
{
    return th08::D3DXVec2CatmullRom(out, v0, v1, v2, v3, s);
}

// FUNCTION: th08 0x479067
D3DXVECTOR2 *WINAPI FUN_00479067(D3DXVECTOR2 *out, const D3DXVECTOR2 *v1, const D3DXVECTOR2 *v2,
                                 const D3DXVECTOR2 *v3, FLOAT f, FLOAT g)
{
    return th08::D3DXVec2BaryCentric(out, v1, v2, v3, f, g);
}

// FUNCTION: th08 0x479095
D3DXVECTOR4 *WINAPI FUN_00479095(D3DXVECTOR4 *out, const D3DXVECTOR2 *v, const D3DXMATRIX *matrix)
{
    return ::D3DXVec2Transform(out, v, matrix);
}

// FUNCTION: th08 0x479100
D3DXVECTOR2 *WINAPI FUN_00479100(D3DXVECTOR2 *out, const D3DXVECTOR2 *v, const D3DXMATRIX *matrix)
{
    return ::D3DXVec2TransformCoord(out, v, matrix);
}

// FUNCTION: th08 0x479180
D3DXVECTOR2 *WINAPI FUN_00479180(D3DXVECTOR2 *out, const D3DXVECTOR2 *v, const D3DXMATRIX *matrix)
{
    return ::D3DXVec2TransformNormal(out, v, matrix);
}

// FUNCTION: th08 0x4791bc
D3DXVECTOR2 *WINAPI FUN_004791bc(D3DXVECTOR2 *out, const D3DXVECTOR2 *v)
{
    return ::D3DXVec2Normalize(out, v);
}

// FUNCTION: th08 0x47925c
D3DXVECTOR3 *WINAPI FUN_0047925c(D3DXVECTOR3 *out, const D3DXVECTOR3 *v1, const D3DXVECTOR3 *t1,
                                 const D3DXVECTOR3 *v2, const D3DXVECTOR3 *t2, FLOAT s)
{
    return th08::D3DXVec3Hermite(out, v1, t1, v2, t2, s);
}

// FUNCTION: th08 0x479285
D3DXVECTOR3 *WINAPI FUN_00479285(D3DXVECTOR3 *out, const D3DXVECTOR3 *v0, const D3DXVECTOR3 *v1,
                                 const D3DXVECTOR3 *v2, const D3DXVECTOR3 *v3, FLOAT s)
{
    return th08::D3DXVec3CatmullRom(out, v0, v1, v2, v3, s);
}

// FUNCTION: th08 0x4792ae
D3DXVECTOR3 *WINAPI FUN_004792ae(D3DXVECTOR3 *out, const D3DXVECTOR3 *v1, const D3DXVECTOR3 *v2,
                                 const D3DXVECTOR3 *v3, FLOAT f, FLOAT g)
{
    return FUN_00477024(out, v1, v2, v3, f, g);
}

// FUNCTION: th08 0x4792e8
D3DXVECTOR4 *WINAPI FUN_004792e8(D3DXVECTOR4 *out, const D3DXVECTOR3 *v, const D3DXMATRIX *matrix)
{
    return ::D3DXVec3Transform(out, v, matrix);
}

// FUNCTION: th08 0x4793b3
D3DXVECTOR3 *WINAPI FUN_004793b3(D3DXVECTOR3 *out, const D3DXVECTOR3 *v, const D3DVIEWPORT8 *viewport,
                                 const D3DXMATRIX *projection, const D3DXMATRIX *view, const D3DXMATRIX *world)
{
    return ::D3DXVec3Project(out, v, viewport, projection, view, world);
}

// FUNCTION: th08 0x479540
D3DXVECTOR3 *WINAPI FUN_00479540(D3DXVECTOR3 *out, const D3DXVECTOR3 *v, const D3DVIEWPORT8 *viewport,
                                 const D3DXMATRIX *projection, const D3DXMATRIX *view, const D3DXMATRIX *world)
{
    return ::D3DXVec3Unproject(out, v, viewport, projection, view, world);
}

// FUNCTION: th08 0x4796f2
D3DXVECTOR3 *WINAPI FUN_004796f2(D3DXVECTOR3 *out, const D3DXVECTOR3 *v)
{
    return ::D3DXVec3Normalize(out, v);
}

// FUNCTION: th08 0x4797ab
D3DXVECTOR4 *WINAPI FUN_004797ab(D3DXVECTOR4 *out, const D3DXVECTOR4 *v1, const D3DXVECTOR4 *t1,
                                 const D3DXVECTOR4 *v2, const D3DXVECTOR4 *t2, FLOAT s)
{
    return ::D3DXVec4Hermite(out, v1, t1, v2, t2, s);
}

// FUNCTION: th08 0x4797d4
D3DXVECTOR4 *WINAPI FUN_004797d4(D3DXVECTOR4 *out, const D3DXVECTOR4 *v0, const D3DXVECTOR4 *v1,
                                 const D3DXVECTOR4 *v2, const D3DXVECTOR4 *v3, FLOAT s)
{
    return ::D3DXVec4CatmullRom(out, v0, v1, v2, v3, s);
}

// FUNCTION: th08 0x4797fd
D3DXVECTOR4 *WINAPI FUN_004797fd(D3DXVECTOR4 *out, const D3DXVECTOR4 *v1, const D3DXVECTOR4 *v2,
                                 const D3DXVECTOR4 *v3, FLOAT f, FLOAT g)
{
    return ::D3DXVec4BaryCentric(out, v1, v2, v3, f, g);
}

// FUNCTION: th08 0x479843
D3DXMATRIX *WINAPI FUN_00479843(D3DXMATRIX *out, const D3DXMATRIX *m1, const D3DXMATRIX *m2)
{
    return ::D3DXMatrixMultiply(out, m1, m2);
}

// FUNCTION: th08 0x47984f
D3DXMATRIX *WINAPI FUN_0047984f(D3DXMATRIX *out, const D3DXMATRIX *matrix)
{
    return ::D3DXMatrixTranspose(out, matrix);
}

// FUNCTION: th08 0x479867
D3DXMATRIX *WINAPI FUN_00479867(D3DXMATRIX *out, FLOAT *determinant, const D3DXMATRIX *matrix)
{
    return ::D3DXMatrixInverse(out, determinant, matrix);
}

// FUNCTION: th08 0x4798cd
D3DXMATRIX *WINAPI FUN_004798cd(D3DXMATRIX *out, FLOAT angle)
{
    return ::D3DXMatrixRotationX(out, angle);
}

// FUNCTION: th08 0x4798e8
D3DXMATRIX *WINAPI FUN_004798e8(D3DXMATRIX *out, FLOAT angle)
{
    return ::D3DXMatrixRotationY(out, angle);
}

// FUNCTION: th08 0x479903
D3DXMATRIX *WINAPI FUN_00479903(D3DXMATRIX *out, FLOAT angle)
{
    return ::D3DXMatrixRotationZ(out, angle);
}

// FUNCTION: th08 0x47993d
D3DXMATRIX *WINAPI FUN_0047993d(D3DXMATRIX *out, const D3DXQUATERNION *rotation)
{
    return ::D3DXMatrixRotationQuaternion(out, rotation);
}

// FUNCTION: th08 0x479949
D3DXMATRIX *WINAPI FUN_00479949(D3DXMATRIX *out, FLOAT yaw, FLOAT pitch, FLOAT roll)
{
    return ::D3DXMatrixRotationYawPitchRoll(out, yaw, pitch, roll);
}

namespace utils
{
void DebugPrint(char *fmt, ...)
{
#ifdef DEBUG
    char tmpBuffer[512];
    std::va_list args;

    va_start(args, fmt);
    vsprintf(tmpBuffer, fmt, args);
    va_end(args);

    printf("DEBUG: %s\n", tmpBuffer);
#endif
}

void GuiDebugPrint(char *fmt, ...)
{
#ifdef DEBUG
    char tmpBuffer[512];
    std::va_list args;

    va_start(args, fmt);
    vsprintf(tmpBuffer, fmt, args);
    va_end(args);

    printf("DEBUG: %s\n", tmpBuffer);
#endif
}
}; // namespace utils
}; // namespace th08
