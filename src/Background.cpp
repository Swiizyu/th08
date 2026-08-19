#include "th_pch.h"

#include "Background.hpp"

namespace th08
{
u32 IsDisableResourceReload();

DIFFABLE_STATIC(Background, g_Background);
DIFFABLE_STATIC(ChainElem, g_BackgroundCalcChain);
DIFFABLE_STATIC(ChainElem, g_BackgroundDrawChainHighPrio);
DIFFABLE_STATIC(ChainElem, g_BackgroundDrawChainLowPrio);

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

// FUNCTION: th08 0x4071a0
Background::Background()
{
    memset(this, 0, sizeof(Background));
    this->vectors0x6394.vector0 = Float3(0.0f, 0.0f, 1000.0f);
    this->vectors0x6394.vector1 = Float3(0.0f, 0.0f, 0.0f);
    this->vectors0x6394.vector2 = Float3(0.0f, 1.0f, 0.0f);
    this->vectors0x6394.angle = 0.5235988f;
    this->vectors0x6264 = this->vectors0x6394;
    this->vectors0x62b0 = this->vectors0x6394;
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

// FUNCTION: th08 0x409b20
#pragma var_order(savedStdData, background)
ZunResult Background::RegisterChain(i32 stage)
{
    Background *background;
    StdRawHeader *savedStdData;

    background = &g_Background;
    if (IsDisableResourceReload())
    {
        savedStdData = background->stdData;
    }
    memset(background, 0, sizeof(Background));
    if (IsDisableResourceReload())
    {
        background->stdData = savedStdData;
    }
    *(i32 *)((u8 *)background + 0x81c) = 0;
    *(i32 *)((u8 *)background + 0x820) = stage;

    g_BackgroundCalcChain.SetCallback((ChainCallback)Background::OnUpdate);
    g_BackgroundCalcChain.addedCallback = (ChainLifetimeCallback)Background::AddedCallback;
    g_BackgroundCalcChain.deletedCallback = (ChainLifetimeCallback)Background::DeletedCallback;
    g_BackgroundCalcChain.arg = background;
    if (g_Chain.AddToCalcChain(&g_BackgroundCalcChain, 8) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    g_BackgroundDrawChainHighPrio.SetCallback((ChainCallback)Background::OnDrawHighPrio);
    g_BackgroundDrawChainHighPrio.arg = background;
    g_Chain.AddToDrawChain(&g_BackgroundDrawChainHighPrio, 6);

    g_BackgroundDrawChainLowPrio.SetCallback((ChainCallback)Background::OnDrawLowPrio);
    g_BackgroundDrawChainLowPrio.arg = background;
    g_Chain.AddToDrawChain(&g_BackgroundDrawChainLowPrio, 7);
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x409c20
ZunResult Background::DeletedCallback(Background *background)
{
    if (!IsDisableResourceReload())
    {
        g_AnmManager->ReleaseAnm(4);
    }
    if (*(void **)background != NULL)
    {
        g_ZunMemory.Free(*(void **)background);
        *(void **)background = NULL;
    }
    if (!IsDisableResourceReload() && background->stdData != NULL)
    {
        g_ZunMemory.Free(background->stdData);
        background->stdData = NULL;
    }
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x409ca0
void Background::CutChain()
{
    g_Chain.Cut(&g_BackgroundCalcChain);
    g_Chain.Cut(&g_BackgroundDrawChainHighPrio);
    g_Chain.Cut(&g_BackgroundDrawChainLowPrio);
}

// STUB: th08 0x409ce0
ZunResult Background::LoadStageData()
{
    return ZUN_ERROR;
}

}; // Namespace th08
