#pragma once

#include "AnmManager.hpp"
#include "Global.hpp"

namespace th08
{

struct Effect;
typedef i32 (Effect::*EffectCallback)();
typedef void(__fastcall *EffectDrawCallback)(Effect *effect);

struct Effect
{
    Effect();

    i32 FUN_0040e040();
    i32 FUN_0040e120();
    i32 FUN_0040e200();
    i32 FUN_0040e2d0();
    i32 FUN_0040eb50();
    i32 FUN_00410bb0();
    i32 FUN_00413070();
    i32 FUN_00425d70();
    i32 FUN_00425e60();
    i32 FUN_00425ea0();
    i32 FUN_00425fe0();
    i32 FUN_00426030();
    i32 FUN_00426b20();
    i32 FUN_00426bb0();
    i32 FUN_00426c40();
    i32 FUN_00426c90();
    i32 FUN_00426d70();
    i32 FUN_00426e70();
    i32 FUN_004270c0();
    i32 FUN_004271a0();
    i32 FUN_00427250();
    i32 FUN_00427260();
    i32 FUN_004272e0();
    i32 FUN_00427970();
    i32 FUN_00427990();
    i32 FUN_004279d0();
    i32 FUN_00427a60();
    i32 FUN_00427ae0();
    i32 FUN_00427b50();

    AnmVm vm;
    Float3 position;
    Float3 custom;
    Float3 velocity;
    Float3 acceleration;
    Float3 basePosition;
    Float3 emitterPosition;
    Float3 direction;
    Float3 unk0x2f8;
    Float3 unk0x304;
    u8 unknown0x310[0x28];
    ZunTimer timer;
    u8 unknown0x344[4];
    EffectCallback updateCallback;
    EffectDrawCallback drawCallback;
    i8 active;
    i8 effectId;
    u8 unknown0x352[2];
    i8 drawType;
    u8 unknown0x355[3];
    void *resource;
    Effect *next;
};
C_ASSERT(sizeof(Effect) == 0x360);

struct EffectManager
{
    EffectManager();

    void ResetEffects();
    ChainCallbackResult DrawUnkTypeEffects();
    ChainCallbackResult FUN_004281e0();
    ZunResult FUN_00428590();
    Effect *FUN_004253e0(i32 index);
    Effect *SpawnEffect(i32 effectId, Float3 *position, i32 count, D3DCOLOR color);
    static void CutChain();

    i32 unk0x0;
    i32 unk0x4;
    i32 unk0x8;
    f32 colorMultiplierR;
    f32 colorMultiplierG;
    f32 colorMultiplierB;
    f32 colorMultiplierA;
    Effect effects[0x28e];
    Effect specialEffect0;
    Effect specialEffect1;
    Effect specialEffect2;
    Effect specialEffect3;
    Effect specialEffect4;
    u8 unknown0x8b03c[0x18];
    AnmLoaded *effectAnm;
    u8 unknown0x8b058[4];
};
C_ASSERT(sizeof(EffectManager) == 0x8b05c);

DIFFABLE_EXTERN(EffectManager, g_EffectManager);

} /* namespace th08 */
