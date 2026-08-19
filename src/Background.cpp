#include "th_pch.h"

#include "Background.hpp"
#include "EffectManager.hpp"
#include "GameManager.hpp"
#include "Gui.hpp"
#include "Player.hpp"
#include "ScreenEffect.hpp"
#include "ZunMath.hpp"

namespace th08
{
u32 IsDisableResourceReload();
u8 __fastcall MixColors(u8 color1, u8 color2);

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

// FUNCTION: th08 0x409200
#pragma var_order(i, unused1, unused2, clearRect, effect, drawRect, fogColor)
ChainCallbackResult Background::OnDrawHighPrio(Background *background)
{
    i32 i;
    i32 unused1;
    i32 unused2;
    D3DRECT clearRect;
    ZunRect drawRect;
    Effect *effect;
    ZunColor fogColor;

    *(i32 *)((u8 *)background + 0x6478) = 0;
    for (i = 0; i < 16; i++)
    {
        background->positions0x6480[i] = Float3(0.0f, 0.0f, 0.0f);
    }
    g_Supervisor.viewport.X = 32;
    g_Supervisor.viewport.Y = 16;
    g_Supervisor.viewport.Width = 384;
    g_Supervisor.viewport.Height = 448;
    g_AnmManager->ClearVertexBuffer();
    g_AnmManager->ClearVertexShader();
    g_AnmManager->ClearSprite();
    g_AnmManager->ClearTexture();
    g_AnmManager->ClearColorOp();
    g_AnmManager->ClearBlendMode();
    g_AnmManager->ClearZWrite();
    g_AnmManager->ResetFrameDebugInfo();
    g_AnmManager->ClearCameraSettings();
    if (!g_Supervisor.IsFogDisabled())
    {
        g_Supervisor.DisableFog();
    }
    g_AnmManager->FlushVertexBuffer();

    if (*(i32 *)((u8 *)background + 0xb2c) != 0)
    {
        clearRect.x1 = 32;
        clearRect.y1 = 16;
        clearRect.x2 = 384;
        clearRect.y2 = 448;
        g_Supervisor.d3dDevice->SetViewport((D3DVIEWPORT8 *)&clearRect);
        g_Supervisor.d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
        *(i32 *)((u8 *)background + 0xb2c) = 0;
    }
    g_Supervisor.d3dDevice->SetViewport(&g_Supervisor.viewport);
    if (*(u8 *)((u8 *)background + 0x646b) > 0)
    {
        g_AnmManager->SetMixColor(*(u32 *)((u8 *)background + 0x6468));
    }
    *(u8 *)((u8 *)background + 0x646b) = 0;
    *(u8 *)((u8 *)background + 0x646a) = 0x80;
    *(u8 *)((u8 *)background + 0x6469) = 0x80;
    *(u8 *)((u8 *)background + 0x6468) = 0x80;

    if (*(i32 *)((u8 *)background + 0xb24) <= 1 && !g_Gui.FUN_00437d87())
    {
        if (background->vm0x4.activeSpriteIndex > 0)
        {
            g_AnmManager->Draw2DAndFlush(&background->vm0x4);
        }
        if (background->vm0x2a8.activeSpriteIndex > 0)
        {
            g_AnmManager->Draw2DAndFlush(&background->vm0x2a8);
        }
        if (*(Effect **)((u8 *)background + 0xae8) != NULL)
        {
            effect = *(Effect **)((u8 *)background + 0xae8);
            effect->drawCallback(effect);
        }
    }

    if ((*(u32 *)((u8 *)background + 0x830) & 0xff000000) == 0xff000000)
    {
        g_Supervisor.d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                      *(u32 *)((u8 *)background + 0x830), 1.0f, 0);
    }
    else
    {
        if (*(u32 *)((u8 *)background + 0x830) != 0)
        {
            drawRect.left = 32.0f;
            drawRect.top = 16.0f;
            drawRect.right = 416.0f;
            drawRect.bottom = 464.0f;
            ScreenEffect::DrawSquare(&drawRect, *(u32 *)((u8 *)background + 0x830));
        }
        g_Supervisor.d3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, *(u32 *)((u8 *)background + 0x830), 1.0f, 0);
    }
    g_Supervisor.SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    if (!g_AnmManager->useMixColor)
    {
        g_Supervisor.SetRenderState(D3DRS_FOGCOLOR, *(i32 *)((u8 *)background + 0xaf4));
    }
    else
    {
        fogColor.d3dColor = *(i32 *)((u8 *)background + 0xaf4);
        fogColor.r = MixColors(fogColor.r, g_AnmManager->color.r);
        fogColor.g = MixColors(fogColor.g, g_AnmManager->color.g);
        fogColor.b = MixColors(fogColor.b, g_AnmManager->color.b);
        g_Supervisor.SetRenderState(D3DRS_FOGCOLOR, fogColor.d3dColor);
    }
    g_Supervisor.SetRenderState(D3DRS_FOGSTART, *(u32 *)((u8 *)background + 0xaec));
    g_Supervisor.SetRenderState(D3DRS_FOGEND, *(u32 *)((u8 *)background + 0xaf0));
    if (!g_Supervisor.IsFogDisabled())
    {
        g_Supervisor.EnableFog();
    }
    if (*(i32 *)((u8 *)background + 0xb24) <= 1 && !g_Gui.FUN_00437d87())
    {
        background->RenderObjects(0);
        background->RenderObjects(1);
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x409640
#pragma var_order(fogDistance, alpha, rect, i)
ChainCallbackResult Background::OnDrawLowPrio(Background *background)
{
    ZunRect rect;
    i32 alpha;
    i32 i;
    f32 fogDistance;

    if (*(i32 *)((u8 *)background + 0xb24) <= 1 && !g_Gui.FUN_00437d87())
    {
        background->RenderObjects(2);
        background->RenderObjects(3);
        if (!g_Supervisor.IsFogDisabled())
        {
            g_Supervisor.DisableFog();
        }
        g_EffectManager.FUN_004281e0();
        if (*(i32 *)((u8 *)background + 0xb24) == 1)
        {
            rect.left = 32.0f;
            rect.top = 16.0f;
            rect.right = 416.0f;
            rect.bottom = 464.0f;
            alpha = *(i32 *)((u8 *)background + 0xb28) * 255 / 60;
            g_AnmManager->FlushVertexBuffer();
            g_Supervisor.SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
            if (!g_Supervisor.IsFogDisabled())
            {
                g_Supervisor.SetRenderState(D3DRS_FOGENABLE, FALSE);
            }
            ScreenEffect::DrawSquare(&rect, alpha << 24);
        }
    }

    g_AnmManager->FlushVertexBuffer();
    g_Supervisor.SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    if (!g_Supervisor.IsFogDisabled())
    {
        g_Supervisor.DisableFog();
    }
    if (*(i32 *)((u8 *)background + 0xb24) >= 1)
    {
        for (i = 0; i < *(i32 *)((u8 *)background + 0xb30); i++)
        {
            g_AnmManager->Draw2DAndFlush(&background->vms0xb38[i]);
        }
        if (background->callback0x625c != NULL)
        {
            background->callback0x625c();
        }
    }
    g_AnmManager->SetCameraMode(0);
    background->SetCamera1();
    g_Supervisor.d3dDevice->SetViewport(&g_Supervisor.viewport);
    fogDistance = 1000.0f;
    g_Supervisor.SetRenderState(D3DRS_FOGSTART, *(u32 *)&fogDistance);
    fogDistance = 2000.0f;
    g_Supervisor.SetRenderState(D3DRS_FOGEND, *(u32 *)&fogDistance);
    if (*(i32 *)((u8 *)background + 0x646c) == 0)
    {
        g_AnmManager->SetMixColorDefault();
    }
    *(i32 *)((u8 *)background + 0x646c) = 0;
    *(i32 *)((u8 *)background + 0x647c) = 0;
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

// FUNCTION: th08 0x409f40
#pragma var_order(unused, vmCount, i, vm, object, quad)
u32 Background::FUN_00409f40()
{
    StdRawQuadBasic *quad;
    StdRawObject *object;
    AnmVm *vm;
    i32 i;
    i32 vmCount;
    StdRawQuadBasic *unused;

    if (*(u8 *)((u8 *)this + 0x834))
    {
        if (g_Player.IsHuman())
        {
            *(u8 *)((u8 *)this + 0x834) = 0;
            this->timer0x838 = 0;
            this->vm0x844.SetInterrupt(2);
        }
    }
    else if (g_Player.IsYoukai())
    {
        *(u8 *)((u8 *)this + 0x834) = 1;
        this->timer0x838 = 0;
        this->vm0x844.SetInterrupt(1);
    }
    this->timer0x838++;
    g_AnmManager->ExecuteScript(&this->vm0x844);

    for (i = 0; i < this->objectsCount; i++)
    {
        object = this->objects[i];
        if (object->flags & 1)
        {
            vmCount = 0;
            quad = &object->firstQuad;
            while (quad->type >= 0)
            {
                vm = &this->quadVms[quad->vmIndex];
                switch (quad->type)
                {
                case 0:
                    g_AnmManager->ExecuteScript(vm);
                    break;
                case 1:
                    unused = quad;
                    g_AnmManager->ExecuteScript(vm);
                    break;
                }
                if (vm->currentInstruction != NULL)
                {
                    vmCount++;
                }
                quad = (StdRawQuadBasic *)((u8 *)quad + quad->byteSize);
            }
            if (vm->type == 1)
            {
                *(u32 *)((u8 *)vm + 0x1f8) |= 0x20000;
                vm->color2.r = (u32)(vm->color1.r * this->vm0x844.color1.r) >> 8;
                vm->color2.g = (u32)(vm->color1.g * this->vm0x844.color1.g) >> 8;
                vm->color2.b = (u32)(vm->color1.b * this->vm0x844.color1.b) >> 8;
                vm->color2.a = (u32)(vm->color1.a * this->vm0x844.color1.a) >> 8;
            }
            if (vmCount == 0)
            {
                object->flags &= ~1;
            }
        }
    }
    return 0;
}

// FUNCTION: th08 0x40a1b0
u8 Background::RenderObjects(u32 zLevel)
{
    StdRawInstance *instance;
    StdRawObject *object;
    StdRawQuadBasic *quad;
    AnmVm *vm;
    Float3 center;
    Float3 cameraPosition;
    f32 distance;
    f32 radius;

    instance = this->objectInstances;
    this->SetCamera2();
    g_AnmManager->SetCameraMode(1);
    while (instance->id >= 0)
    {
        object = this->objects[instance->id];
        if (object->zLevel == zLevel)
        {
            center.x = object->position.x + instance->position.x - this->position0x824.x + object->size.x / 2.0f;
            center.y = object->position.y + instance->position.y - this->position0x824.y + object->size.y / 2.0f;
            center.z = object->position.z + instance->position.z - this->position0x824.z + object->size.z / 2.0f;
            cameraPosition = this->vectors0x6394.vector0 + this->vectors0x6394.vector5;
            center -= cameraPosition;
            if (center.FUN_0040b500() <= *(f32 *)((u8 *)this + 0x6470))
            {
                distance = center.FUN_0040b540(&this->vectors0x6394.vector3);
                radius = object->size.FUN_0040b4c0() / 2.0f + 960.0f;
                if (distance <= radius && distance >= 80.0f)
                {
                    object->flags |= 2;
                    quad = &object->firstQuad;
                    while (quad->type >= 0)
                    {
                        vm = &this->quadVms[quad->vmIndex];
                        vm->pos.x = vm->pos2.x + quad->position.x + instance->position.x - this->position0x824.x;
                        vm->pos.y = vm->pos2.y + quad->position.y + instance->position.y - this->position0x824.y;
                        vm->pos.z = vm->pos2.z + quad->position.z + instance->position.z - this->position0x824.z;
                        if (quad->size.x != 0.0f && vm->loadedSprite != NULL)
                        {
                            vm->scale.x = quad->size.x / vm->loadedSprite->widthPx;
                        }
                        if (quad->size.y != 0.0f && vm->loadedSprite != NULL)
                        {
                            vm->scale.y = quad->size.y / vm->loadedSprite->heightPx;
                        }
                        if (quad->type == 0)
                        {
                            g_AnmManager->DrawWorld(vm);
                        }
                        else if (quad->type == 1)
                        {
                            g_AnmManager->FUN_00464070(vm);
                        }
                        quad = (StdRawQuadBasic *)((u8 *)quad + quad->byteSize);
                    }
                }
            }
        }
        instance++;
    }
    return 0;
}

// FUNCTION: th08 0x40b5a0
#pragma var_order(eyeZ, centerY, centerX, aspectRatio, fov)
void Background::SetCamera1()
{
    f32 fov;
    f32 aspectRatio;
    f32 centerX;
    f32 centerY;
    f32 eyeZ;

    centerX = (f32)g_Supervisor.viewport.Width / 2.0f;
    centerY = (f32)g_Supervisor.viewport.Height / 2.0f;
    aspectRatio = (f32)g_Supervisor.viewport.Width / (f32)g_Supervisor.viewport.Height;
    fov = ZUN_PI / 10.0f;
    eyeZ = centerY / tan(fov / 2.0f);
    D3DXMatrixLookAtLH(&g_Supervisor.viewMatrix, (D3DXVECTOR3 *)&Float3(centerX, centerY, eyeZ),
                       (D3DXVECTOR3 *)&Float3(centerX, centerY, 0.0f),
                       (D3DXVECTOR3 *)&Float3(0.0f, -1.0f, 0.0f));
    D3DXMatrixPerspectiveFovLH(&g_Supervisor.projectionMatrix, fov, aspectRatio, 1.0f, 10000.0f);
    g_Supervisor.d3dDevice->SetTransform(D3DTS_VIEW, &g_Supervisor.viewMatrix);
    g_Supervisor.d3dDevice->SetTransform(D3DTS_PROJECTION, &g_Supervisor.projectionMatrix);
}

// FUNCTION: th08 0x40b6d0
#pragma var_order(eye, at)
void Background::SetCamera2()
{
    Float3 at = this->vectors0x6394.vector1 + this->vectors0x6394.vector0;
    Float3 eye = this->vectors0x6394.vector5 + this->vectors0x6394.vector0;

    D3DXMatrixLookAtLH(&g_Supervisor.viewMatrix, (D3DXVECTOR3 *)&eye, (D3DXVECTOR3 *)&at,
                       (D3DXVECTOR3 *)&this->vectors0x6394.vector2);
    D3DXMatrixPerspectiveFovLH(&g_Supervisor.projectionMatrix, this->vectors0x6394.angle,
                               (f32)g_Supervisor.viewport.Width / (f32)g_Supervisor.viewport.Height, 30.0f,
                               1800.0f);
    g_Supervisor.d3dDevice->SetTransform(D3DTS_VIEW, &g_Supervisor.viewMatrix);
    g_Supervisor.d3dDevice->SetTransform(D3DTS_PROJECTION, &g_Supervisor.projectionMatrix);
    D3DXVec3Cross((D3DXVECTOR3 *)&this->vectors0x6394.vector4, (D3DXVECTOR3 *)&this->vectors0x6394.vector1,
                  (D3DXVECTOR3 *)&this->vectors0x6394.vector2);
    D3DXVec3Normalize((D3DXVECTOR3 *)&this->vectors0x6394.vector4,
                      (D3DXVECTOR3 *)&this->vectors0x6394.vector4);
}

}; // Namespace th08
