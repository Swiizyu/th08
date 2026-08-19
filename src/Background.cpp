#include "th_pch.h"

#include "Background.hpp"
#include "GameManager.hpp"

namespace th08
{
u32 IsDisableResourceReload();

DIFFABLE_STATIC(Background, g_Background);
DIFFABLE_STATIC(ChainElem, g_BackgroundCalcChain);
DIFFABLE_STATIC(ChainElem, g_BackgroundDrawChainHighPrio);
DIFFABLE_STATIC(ChainElem, g_BackgroundDrawChainLowPrio);
DIFFABLE_STATIC_ARRAY_ASSIGN(const char *, 9, g_StageAnmFiles) = {
    "stg1bg.anm", "stg2bg.anm", "stg3bg.anm", "stg4abg.anm", "stg4abg.anm",
    "stg5bg.anm", "stg6bg.anm", "stg7bg.anm", "stg8bg.anm",
};
DIFFABLE_STATIC_ARRAY_ASSIGN(const char *, 9, g_StageStdFiles) = {
    "stage1.std", "stage2.std", "stage3.std", "stage4a.std", "stage4b.std",
    "stage5.std", "stage6.std", "stage7.std", "stage8.std",
};
DIFFABLE_STATIC_ARRAY_ASSIGN(const char *, 9, g_StageSpellStdFiles) = {
    "stage1_s.std", "stage2_s.std", "stage3_s.std", "stage4a_s.std", "stage4b_s.std",
    "stage5_s.std", "stage6_s.std", "stage7_s.std", "stage8_s.std",
};

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

// FUNCTION: th08 0x409850
#pragma var_order(i)
ZunResult Background::AddedCallback(Background *background)
{
    i32 i;

    background->timer0x80c = 0;
    *(i32 *)((u8 *)background + 0x818) = 0;
    background->position0x824.x = 0.0f;
    background->position0x824.y = 0.0f;
    background->position0x824.z = 0.0f;
    *(i32 *)((u8 *)background + 0xb24) = 0;
    *(i32 *)((u8 *)background + 0xb10) = 0;

    if (!IsDisableResourceReload())
    {
        background->anm0x7f0 = g_AnmManager->PreloadAnm(4, g_StageAnmFiles[g_GameManager.currentStage]);
        if (background->anm0x7f0 == NULL)
        {
            return ZUN_ERROR;
        }
    }
    else
    {
        background->anm0x7f0 = g_AnmManager->GetAnm(4);
    }

    if (!g_GameManager.IsSpellPractice())
    {
        if (background->LoadStageData(g_StageStdFiles[g_GameManager.currentStage]) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
    }
    else if (background->LoadStageData(g_StageSpellStdFiles[g_GameManager.currentStage]) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    *(u32 *)((u8 *)background + 0xaf4) = 0xff000000;
    *(f32 *)((u8 *)background + 0xaec) = 200.0f;
    *(f32 *)((u8 *)background + 0xaf0) = 500.0f;
    background->vectors0x6394.vector0 = Float3(0.0f, 0.0f, 1000.0f);
    background->vectors0x6394.vector1 = Float3(0.0f, 0.0f, 0.0f);
    background->vectors0x6394.vector5 = Float3(0.0f, 0.0f, 0.0f);
    background->vectors0x6394.vector2 = Float3(0.0f, 1.0f, 0.0f);
    background->vectors0x6394.angle = 0.5235988f;
    background->vectors0x6264 = background->vectors0x6394;
    background->vectors0x62b0 = background->vectors0x6394;
    *(u8 *)((u8 *)background + 0x6474) = 0;
    for (i = 0; i < 4; i++)
    {
        *(i32 *)((u8 *)background + 0x63e0 + i * 4) = 0;
        background->timers0x63f4[i] = 0;
    }
    background->unknown0x6260 = 0;
    *(f32 *)((u8 *)background + 0x6470) = 1322500.0f;
    if (g_GameManager.currentStage == 5)
    {
        *(f32 *)((u8 *)background + 0x6470) = 1822500.0f;
    }
    else if (g_GameManager.currentStage == 6 || g_GameManager.currentStage == 7)
    {
        *(f32 *)((u8 *)background + 0x6470) = 3240000.0f;
    }
    return ZUN_SUCCESS;
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

// FUNCTION: th08 0x409ce0
#pragma var_order(vmIdx, i, object, quad)
ZunResult Background::LoadStageData(const char *path)
{
    StdRawQuadBasic *quad;
    StdRawObject *object;
    i32 i;
    i32 vmIdx;

    if (!IsDisableResourceReload())
    {
        this->stdData = (StdRawHeader *)FileSystem::OpenFile(path, NULL, FALSE);
        if (this->stdData == NULL)
        {
            g_GameErrorContext.Log("\x83\x58\x83\x65\x81\x5b\x83\x57\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
            return ZUN_ERROR;
        }
    }

    this->objectsCount = this->stdData->nbObjects;
    this->quadCount = this->stdData->nbQuads;
    this->objectInstances = (StdRawInstance *)(this->stdData->quadsOffset + (i32)this->stdData);
    this->beginningOfScript = (StdRawInstr *)(this->stdData->scriptOffset + (i32)this->stdData);
    this->objects = (StdRawObject **)(this->stdData + 1);
    if (!IsDisableResourceReload())
    {
        for (i = 0; i < this->objectsCount; i++)
        {
            this->objects[i] = (StdRawObject *)((u8 *)this->objects[i] + (u32)this->stdData);
        }
    }

    this->quadVms = (AnmVm *)g_ZunMemory.Alloc(this->quadCount * sizeof(AnmVm), "bgscroll");
    for (i = 0, vmIdx = 0; i < this->objectsCount; i++)
    {
        object = this->objects[i];
        object->flags = 1;
        quad = &object->firstQuad;
        while (quad->type >= 0)
        {
            this->anm0x7f0->ExecuteAnmIdx(&this->quadVms[vmIdx], quad->anmScript);
            quad->vmIndex = vmIdx++;
            quad = (StdRawQuadBasic *)((u8 *)quad + quad->byteSize);
        }
    }

    switch (g_GameManager.currentStage)
    {
    case 2:
        g_Supervisor.textAnm->SetAndExecuteScriptIdx(&this->vm0x844, 33);
        break;
    default:
        g_Supervisor.textAnm->SetAndExecuteScriptIdx(&this->vm0x844, 33);
        break;
    }
    this->vm0x844.SetInterrupt(2);
    *(u8 *)((u8 *)this + 0x834) = 0;
    this->timer0x838 = 0;
    return ZUN_SUCCESS;
}

}; // Namespace th08
