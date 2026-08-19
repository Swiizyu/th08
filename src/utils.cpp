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
    static char buffer[64];
    if (digits < 1) digits = 1;
    if (digits > 30) digits = 30;
    sprintf(buffer, "%0*d", digits, value);
    return buffer;
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
