#pragma once

#include "Global.hpp"
#include "Supervisor.hpp"
#include "ZunMath.hpp"
#include "inttypes.hpp"
#include "utils.hpp"
#include <d3dx8.h>

namespace th08
{

enum ScreenEffectType
{
    SCREEN_EFFECT_FULL_FADE_IN,
    SCREEN_EFFECT_SHAKE,
    SCREEN_EFFECT_ARCADE_FADE_OUT,
    SCREEN_EFFECT_FULL_FADE_OUT,
    SCREEN_EFFECT_PULSE,
    SCREEN_EFFECT_UNK5,
    SCREEN_EFFECT_UNK6,
    SCREEN_EFFECT_UNK7,
};

struct ScreenEffect
{
    ScreenEffect();

    static void Clear(D3DCOLOR color);
    static void SetViewport(D3DCOLOR clearColor);

    static ChainCallbackResult CalcFadeIn(ScreenEffect *screenEffect);
    static void __fastcall DrawSquare(ZunRect *rectDimensions, D3DCOLOR color);
    static void __fastcall DrawSquareShaded(ZunRect *rect, D3DCOLOR topLeft, D3DCOLOR topRight, D3DCOLOR bottomLeft,
                                            D3DCOLOR bottomRight);
    static ChainCallbackResult CalcFadeOut(ScreenEffect *screenEffect);
    static ChainCallbackResult CalcPartialFadeOut(ScreenEffect *screenEffect);

    static ScreenEffect *__fastcall RegisterChain(ScreenEffectType effect, i32 ticks, i32 param_3, i32 param_4,
                                                  i32 param_5, i32 drawPriority);

    static ChainCallbackResult DrawFullFade(ScreenEffect *screenEffect);
    static ChainCallbackResult DrawPartialFade(ScreenEffect *screenEffect);
    static ChainCallbackResult DrawArcadeFade(ScreenEffect *screenEffect);
    static ChainCallbackResult FUN_0045bc90(ScreenEffect *screenEffect);
    static ChainCallbackResult FUN_0045bd70(ScreenEffect *screenEffect);
    static void FUN_0045c160(ScreenEffect *screenEffect);
    static ChainCallbackResult CalcShake(ScreenEffect *screenEffect);
    static ChainCallbackResult CalcShakeWithEnvelope(ScreenEffect *screenEffect);

    static ZunResult AddedCallback(ScreenEffect *screenEffect);
    static ZunResult DeletedCallback(ScreenEffect *screenEffect);

    // In fade effects, genericParam is an RGB color to fade to.
    // In shake effects, genericParam controls the base view offset and shakinessParam the shakiness over time.
    i32 usedEffect;
    ChainElem *calcChainElement;
    ChainElem *drawChainElement;
    i32 unused;
    i32 fadeAlpha;
    i32 effectLength;
    i32 genericParam;
    i32 shakinessParam;
    i32 extraParam;
    i32 unusedParam;
    ZunTimer timer;
};

DIFFABLE_EXTERN(i32, g_ScreenEffectCounter);

}; // Namespace th08
