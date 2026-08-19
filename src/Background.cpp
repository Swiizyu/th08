#include "th_pch.h"

#include "Background.hpp"

namespace th08
{
DIFFABLE_STATIC(Background, g_Background);

// FUNCTION: th08 0x4031e0
f32 FUN_004031e0(f32 value)
{
    return fabs(value);
}

// FUNCTION: th08 0x40c7b0
f32 FUN_0040c7b0(f32 value, f32 exponent)
{
    return pow(value, exponent);
}

// FUNCTION: th08 0x4073b0
BackgroundUnkVectors::BackgroundUnkVectors()
{
}

// FUNCTION: th08 0x409160
void Background::FUN_00409160(u32 color)
{
    u32 colorCopy;

    if (*(u8 *)((u8 *)this + 0x646b) == 0)
    {
        *(u32 *)((u8 *)this + 0x6468) = color;
    }
    else
    {
        colorCopy = color;
        *(u8 *)((u8 *)this + 0x646a) =
            (u32)(((u8 *)&colorCopy)[2] + *(u8 *)((u8 *)this + 0x646a)) >> 1;
        *(u8 *)((u8 *)this + 0x6469) =
            (u32)(((u8 *)&colorCopy)[1] + *(u8 *)((u8 *)this + 0x6469)) >> 1;
        *(u8 *)((u8 *)this + 0x6468) =
            (u32)(((u8 *)&colorCopy)[0] + *(u8 *)((u8 *)this + 0x6468)) >> 1;
        *(u8 *)((u8 *)this + 0x646b) =
            (u32)(((u8 *)&colorCopy)[3] + *(u8 *)((u8 *)this + 0x646b)) >> 1;
    }
}

// FUNCTION: th08 0x409080
Float3 Float3::operator+(const Float3 &other)
{
    return Float3(this->x + other.x, this->y + other.y, this->z + other.z);
}

// FUNCTION: th08 0x4090d0
Float3 Float3::operator-(const Float3 &other)
{
    return Float3(this->x - other.x, this->y - other.y, this->z - other.z);
}

// FUNCTION: th08 0x409120
Float3 Float3::operator*(f32 scalar)
{
    return Float3(this->x * scalar, this->y * scalar, this->z * scalar);
}

// FUNCTION: th08 0x40b470
Float3 *Float3::operator/=(f32 scalar)
{
    f32 inverse = 1.0f / scalar;
    this->x *= inverse;
    this->y *= inverse;
    this->z *= inverse;
    return this;
}

// FUNCTION: th08 0x40c7d0
Float3 Float3::operator/(f32 scalar)
{
    f32 inverse = 1.0f / scalar;
    return Float3(this->x * inverse, this->y * inverse, this->z * inverse);
}

// FUNCTION: th08 0x415ce0
void Background::background_fun_00415ce0()
{
    *(i32 *)((u8 *)this + 0xb24) = 1;
    *(i32 *)((u8 *)this + 0xb28) = 0;
}

// FUNCTION: th08 0x416ad0
void Background::background_fun_00416ad0()
{
    *(i32 *)((u8 *)this + 0xb24) = 0;
}

// STUB: th08 0x4071a0
Background::Background()
{
}

// STUB: th08 0x407400
ChainCallbackResult Background::OnUpdate(Background *background)
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// STUB: th08 0x409200
ChainCallbackResult Background::OnDrawHighPrio(Background *background)
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// STUB: th08 0x409640
ChainCallbackResult Background::OnDrawLowPrio(Background *background)
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// STUB: th08 0x409850
ZunResult Background::AddedCallback(Background *background)
{
    return ZUN_ERROR;
}

// STUB: th08 0x409b20
ZunResult Background::RegisterChain()
{
    return ZUN_ERROR;
}

// STUB: th08 0x409c20
ZunResult Background::DeletedCallback()
{
    return ZUN_ERROR;
}

// STUB: th08 0x409ca0
void Background::CutChain()
{
}

// STUB: th08 0x409ce0
ZunResult Background::LoadStageData()
{
    return ZUN_ERROR;
}

}; // Namespace th08
