#pragma once

#include "AnmManager.hpp"
#include "Global.hpp"

namespace th08
{

struct Effect;
typedef void(__fastcall *EffectDrawCallback)(Effect *effect);

struct Effect
{
    i32 FUN_00425e60();
    i32 FUN_00425fe0();
    i32 FUN_00426b20();
    i32 FUN_00426bb0();
    i32 FUN_00426c40();
    i32 FUN_00426c90();
    i32 FUN_00427990();

    AnmVm vm;
    Float3 position;
    u8 unknown0x2b0[0x9c];
    EffectDrawCallback drawCallback;
    u8 unknown0x350[0xc];
    Effect *next;
};
C_ASSERT(sizeof(Effect) == 0x360);

struct EffectManager
{
    void ResetEffects();
    ChainCallbackResult DrawUnkTypeEffects();
    static void CutChain();

    u8 data[0x8acd8];
    Effect *drawListHead;
    u8 unknown0x8acdc[0x380];
};
C_ASSERT(sizeof(EffectManager) == 0x8b05c);

DIFFABLE_EXTERN(EffectManager, g_EffectManager);

} /* namespace th08 */
