#include "th_pch.h"

#include "ScreenEffect.hpp"

#include "AnmManager.hpp"
#include "GameManager.hpp"

namespace th08
{

DIFFABLE_STATIC(i32, g_ScreenEffectCounter);
DIFFABLE_STATIC(ScreenEffect, g_ScreenEffect);

ScreenEffect::ScreenEffect()
{
}

// FUNCTION: th08 0x45b020
void ScreenEffect::Clear(D3DCOLOR color)
{
    // ZUN bloat: This is doing the exact same thing twice
    g_Supervisor.d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
    if (FAILED(g_Supervisor.d3dDevice->Present(NULL, NULL, NULL, NULL)))
    {
        g_Supervisor.d3dDevice->Reset(&g_Supervisor.presentParameters);
    }
    g_Supervisor.d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
    if (FAILED(g_Supervisor.d3dDevice->Present(NULL, NULL, NULL, NULL)))
    {
        g_Supervisor.d3dDevice->Reset(&g_Supervisor.presentParameters);
    }
}

// FUNCTION: th08 0x45b0e0
void ScreenEffect::SetViewport(D3DCOLOR clearColor)
{
    if (g_AnmManager)
    {
        g_AnmManager->FlushVertexBuffer();
    }
    g_Supervisor.viewport.X = 0;
    g_Supervisor.viewport.Y = 0;
    g_Supervisor.viewport.Width = WINDOW_WIDTH;
    g_Supervisor.viewport.Height = WINDOW_HEIGHT;
    g_Supervisor.viewport.MinZ = 0.0f;
    g_Supervisor.viewport.MaxZ = 1.0f;
    g_Supervisor.d3dDevice->SetViewport(&g_Supervisor.viewport);
    Clear(clearColor);
}

// FUNCTION: th08 0x45b160
ChainCallbackResult ScreenEffect::CalcFadeIn(ScreenEffect *screenEffect)
{
    if (screenEffect->effectLength != 0)
    {
        screenEffect->fadeAlpha = 255.0f - (((f32)screenEffect->timer * 255.0f) / screenEffect->effectLength);
        if (screenEffect->fadeAlpha < 0)
        {
            screenEffect->fadeAlpha = 0;
        }
    }

    if (screenEffect->timer >= screenEffect->effectLength)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
    }

    screenEffect->timer++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x45b1e0
void __fastcall ScreenEffect::DrawSquare(ZunRect *rect, D3DCOLOR color)
{
    g_AnmManager->FlushVertexBuffer();

    VertexDiffuseXyzrhw vertices[4];

    vertices[0].pos = Float3(rect->left, rect->top, 0.0f);
    vertices[1].pos = Float3(rect->right, rect->top, 0.0f);
    vertices[2].pos = Float3(rect->left, rect->bottom, 0.0f);
    vertices[3].pos = Float3(rect->right, rect->bottom, 0.0f);
    vertices[0].w = vertices[1].w = vertices[2].w = vertices[3].w = 1.0f;
    vertices[0].diffuse = vertices[1].diffuse = vertices[2].diffuse = vertices[3].diffuse = color;

    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    if (!g_Supervisor.IsDepthTestDisabled())
    {
        g_Supervisor.d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    }
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_Supervisor.d3dDevice->SetVertexShader(D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    g_Supervisor.d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VertexDiffuseXyzrhw));
    g_AnmManager->ClearVertexShader();
    g_AnmManager->ClearSprite();
    g_AnmManager->ClearTexture();
    g_AnmManager->ClearColorOp();
    g_AnmManager->ClearBlendMode();
    g_AnmManager->ClearZWrite();
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
}

// FUNCTION: th08 0x45b490
void __fastcall ScreenEffect::DrawSquareShaded(ZunRect *rect, D3DCOLOR topLeft, D3DCOLOR topRight, D3DCOLOR bottomLeft,
                                               D3DCOLOR bottomRight)
{
    g_AnmManager->FlushVertexBuffer();

    VertexDiffuseXyzrhw vertices[4];

    vertices[0].pos = Float3(rect->left, rect->top, 0.0f);
    vertices[1].pos = Float3(rect->right, rect->top, 0.0f);
    vertices[2].pos = Float3(rect->left, rect->bottom, 0.0f);
    vertices[3].pos = Float3(rect->right, rect->bottom, 0.0f);
    vertices[0].w = vertices[1].w = vertices[2].w = vertices[3].w = 1.0f;
    vertices[0].diffuse = topLeft;
    vertices[1].diffuse = topRight;
    vertices[2].diffuse = bottomLeft;
    vertices[3].diffuse = bottomRight;

    if (!g_Supervisor.IsColorCompositingDisabled())
    {
        g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    }
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    if (!g_Supervisor.IsDepthTestDisabled())
    {
        g_Supervisor.d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    }
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_Supervisor.d3dDevice->SetVertexShader(D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
    g_Supervisor.d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VertexDiffuseXyzrhw));
    g_AnmManager->ClearVertexShader();
    g_AnmManager->ClearSprite();
    g_AnmManager->ClearTexture();
    g_AnmManager->ClearColorOp();
    g_AnmManager->ClearBlendMode();
    g_AnmManager->ClearZWrite();
    if (!g_Supervisor.IsColorCompositingDisabled())
    {
        g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    }
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
}

// FUNCTION: th08 0x45b760
ChainCallbackResult ScreenEffect::CalcFadeOut(ScreenEffect *screenEffect)
{
    if (g_ScreenEffectCounter != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
    }

    if (screenEffect->effectLength != 0)
    {
        screenEffect->fadeAlpha = (((f32)screenEffect->timer * 255.0f) / screenEffect->effectLength);
        if (screenEffect->fadeAlpha < 0)
        {
            screenEffect->fadeAlpha = 0;
        }
    }

    if (screenEffect->timer >= screenEffect->effectLength)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
    }

    if (g_GameManager.showPauseMenu == 0 && g_GameManager.showRetryMenu == 0)
    {
        screenEffect->timer++;
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x45b800
ChainCallbackResult ScreenEffect::CalcPartialFadeOut(ScreenEffect *screenEffect)
{
    if (screenEffect->unusedParam == 0)
    {
        if (screenEffect->effectLength != 0 && screenEffect->timer <= screenEffect->effectLength)
        {
            screenEffect->fadeAlpha = (((f32)screenEffect->timer * 128.0f) / screenEffect->effectLength);
        }
    }
    else if (screenEffect->timer <= 8)
    {
        screenEffect->fadeAlpha = 128 - (i32)(((f32)screenEffect->timer * 128.0f) / 8.0f);
    }
    else
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
    }

    screenEffect->timer++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x45b8b0
ScreenEffect *__fastcall ScreenEffect::RegisterChain(ScreenEffectType effect, i32 ticks, i32 param_3, i32 param_4,
                                                     i32 param_5, i32 drawPriority)
{
    ChainElem *calcChainElem;
    ChainElem *drawChainElem;
    ScreenEffect *createdEffect;

    calcChainElem = NULL;
    drawChainElem = NULL;

    createdEffect = ZUN_NEW(ScreenEffect, "ScreenInf");

    if (createdEffect == NULL)
    {
        return NULL;
    }

    memset(createdEffect, 0, sizeof(*createdEffect));

    switch (effect)
    {
    case SCREEN_EFFECT_FULL_FADE_IN:
        calcChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeIn);
        drawChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawFullFade);
        break;
    case SCREEN_EFFECT_SHAKE:
        calcChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcShake);
        break;
    case SCREEN_EFFECT_ARCADE_FADE_OUT:
        calcChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeOut);
        drawChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawArcadeFade);
        break;
    case SCREEN_EFFECT_FULL_FADE_OUT:
        calcChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeOut);
        drawChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawFullFade);
        break;
    case SCREEN_EFFECT_PULSE:
        calcChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::FUN_0045bc90);
        drawChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::FUN_0045bd70);
        break;
    case SCREEN_EFFECT_UNK5:
        calcChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcPartialFadeOut);
        drawChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawPartialFade);
        break;
    case SCREEN_EFFECT_UNK6:
        calcChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcPartialFadeOut);
        drawChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawArcadeFade);
        break;
    case SCREEN_EFFECT_UNK7:
        calcChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcShakeWithEnvelope);
    }

    calcChainElem->addedCallback = (ChainLifetimeCallback)ScreenEffect::AddedCallback;
    calcChainElem->deletedCallback = (ChainLifetimeCallback)ScreenEffect::DeletedCallback;
    calcChainElem->arg = createdEffect;
    createdEffect->usedEffect = effect;
    createdEffect->effectLength = ticks;
    createdEffect->genericParam = param_3;
    createdEffect->shakinessParam = param_4;
    createdEffect->extraParam = param_5;

    if (g_Chain.AddToCalcChain(calcChainElem, 3) != 0)
    {
        return NULL;
    }

    if (drawChainElem != NULL)
    {
        drawChainElem->arg = createdEffect;
        g_Chain.AddToDrawChain(drawChainElem, drawPriority);
    }

    createdEffect->calcChainElement = calcChainElem;
    createdEffect->drawChainElement = drawChainElem;
    return createdEffect;
}

// FUNCTION: th08 0x45bb50
ChainCallbackResult ScreenEffect::DrawFullFade(ScreenEffect *screenEffect)
{
    ZunRect fadeRect;

    fadeRect.left = 0.0f;
    fadeRect.top = 0.0f;
    fadeRect.right = 640.0f;
    fadeRect.bottom = 480.0f;
    g_AnmManager->FlushVertexBuffer();
    g_Supervisor.viewport.X = 0;
    g_Supervisor.viewport.Y = 0;
    g_Supervisor.viewport.Width = 640;
    g_Supervisor.viewport.Height = 480;
    g_Supervisor.d3dDevice->SetViewport(&g_Supervisor.viewport);
    ScreenEffect::DrawSquare(&fadeRect, (screenEffect->fadeAlpha << 24) | screenEffect->genericParam);
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x45bbf0
ChainCallbackResult ScreenEffect::DrawPartialFade(ScreenEffect *screenEffect)
{
    ZunRect fadeRect;

    fadeRect.left = 0.0f;
    fadeRect.top = 0.0f;
    fadeRect.right = 640.0f;
    fadeRect.bottom = 480.0f;
    ScreenEffect::DrawSquare(&fadeRect, (screenEffect->fadeAlpha << 24) | screenEffect->genericParam);
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x45bc40
ChainCallbackResult ScreenEffect::DrawArcadeFade(ScreenEffect *screenEffect)
{
    ZunRect fadeRect;

    fadeRect.left = 32.0f;
    fadeRect.top = 16.0f;
    fadeRect.right = 416.0f;
    fadeRect.bottom = 464.0f;
    ScreenEffect::DrawSquare(&fadeRect, (screenEffect->fadeAlpha << 24) | screenEffect->genericParam);
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x45bc90
ChainCallbackResult ScreenEffect::FUN_0045bc90(ScreenEffect *screenEffect)
{
    u32 maxAlpha;

    maxAlpha = ((u32)screenEffect->shakinessParam >> 24) & 0xff;
    if (g_ScreenEffectCounter != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
    }

    if (screenEffect->timer < screenEffect->effectLength)
    {
        screenEffect->fadeAlpha =
            maxAlpha - (i32)(((f32)screenEffect->timer * (f32)maxAlpha) / screenEffect->effectLength);
        if (screenEffect->fadeAlpha < 0)
        {
            screenEffect->fadeAlpha = 0;
        }
    }
    else
    {
        screenEffect->fadeAlpha = 0;
        screenEffect->genericParam -= 1;
        if ((i32)screenEffect->genericParam <= 0)
        {
            return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
        }
        screenEffect->timer = 0;
    }

    screenEffect->timer++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x45bd70
ChainCallbackResult ScreenEffect::FUN_0045bd70(ScreenEffect *screenEffect)
{
    ZunRect fadeRect;

    fadeRect.left = 32.0f;
    fadeRect.top = 16.0f;
    fadeRect.right = 416.0f;
    fadeRect.bottom = 464.0f;
    ScreenEffect::DrawSquare(&fadeRect, (screenEffect->fadeAlpha << 24) | (screenEffect->shakinessParam & 0xffffff));
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x45bdc0
ChainCallbackResult ScreenEffect::CalcShake(ScreenEffect *screenEffect)
{
    f32 screenOffset;

    if (g_GameManager.flags.unk10 != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (g_GameManager.unk2C != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (g_ScreenEffectCounter != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
    }

    screenEffect->timer++;
    if (screenEffect->timer >= screenEffect->effectLength)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
    }

    screenOffset = (f32)screenEffect->timer * (screenEffect->shakinessParam - screenEffect->genericParam);
    screenOffset /= screenEffect->effectLength;
    screenOffset = screenEffect->genericParam + screenOffset;

    switch (g_Rng.GetRandomU32InRange(3))
    {
    case 0:
        g_AnmManager->screenShakeOffset.x = 0.0f;
        break;
    case 1:
        g_AnmManager->screenShakeOffset.x = screenOffset;
        break;
    case 2:
        g_AnmManager->screenShakeOffset.x = -screenOffset;
    }

    switch (g_Rng.GetRandomU32InRange(3))
    {
    case 0:
        g_AnmManager->screenShakeOffset.y = 0.0f;
        break;
    case 1:
        g_AnmManager->screenShakeOffset.y = screenOffset;
        break;
    case 2:
        g_AnmManager->screenShakeOffset.y = -screenOffset;
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x45bf10 (not yet in the upstream mapping)
ChainCallbackResult ScreenEffect::CalcShakeWithEnvelope(ScreenEffect *screenEffect)
{
    f32 shakePhase;

    if (g_GameManager.flags.unk10 != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (g_GameManager.unk2C != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if ((i32)g_GameManager.unk3ddc0 <= 1)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
    }

    screenEffect->timer++;
    if (screenEffect->timer < screenEffect->genericParam)
    {
        shakePhase = (f32)screenEffect->timer / (f32)screenEffect->genericParam;
    }
    else if (screenEffect->timer < screenEffect->genericParam + screenEffect->shakinessParam)
    {
        shakePhase = 1.0f;
    }
    else if (screenEffect->timer < screenEffect->genericParam + screenEffect->shakinessParam + screenEffect->extraParam)
    {
        shakePhase = ((f32)(u32)(screenEffect->genericParam + screenEffect->shakinessParam +
                                 screenEffect->extraParam) -
                      (f32)screenEffect->timer) /
                     (f32)(u32)screenEffect->extraParam;
    }
    else
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
    }

    shakePhase = screenEffect->effectLength * shakePhase;

    switch (g_Rng.GetRandomU32InRange(3))
    {
    case 0:
        g_AnmManager->screenShakeOffset.x = 0.0f;
        break;
    case 1:
        g_AnmManager->screenShakeOffset.x = shakePhase;
        break;
    case 2:
        g_AnmManager->screenShakeOffset.x = -shakePhase;
    }

    switch (g_Rng.GetRandomU32InRange(3))
    {
    case 0:
        g_AnmManager->screenShakeOffset.y = 0.0f;
        break;
    case 1:
        g_AnmManager->screenShakeOffset.y = shakePhase;
        break;
    case 2:
        g_AnmManager->screenShakeOffset.y = -shakePhase;
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x45c0e0
ZunResult ScreenEffect::AddedCallback(ScreenEffect *screenEffect)
{
    screenEffect->timer = 0;
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x45c100
ZunResult ScreenEffect::DeletedCallback(ScreenEffect *screenEffect)
{
    screenEffect->calcChainElement->deletedCallback = NULL;
    g_Chain.Cut(screenEffect->drawChainElement);
    screenEffect->drawChainElement = NULL;
    g_ZunMemory.RemoveFromRegistry(screenEffect);
    delete screenEffect;
    screenEffect = NULL;

    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x45c160
void ScreenEffect::FUN_0045c160(ScreenEffect *screenEffect)
{
    screenEffect->unusedParam = 1;
    screenEffect->timer = 0;
}

} /* namespace th08 */
