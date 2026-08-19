#pragma once

#include "Global.hpp"

namespace th08
{

struct EffectManager
{
    void ResetEffects();
    static void CutChain();

    u8 data[0x8b05c];
};
C_ASSERT(sizeof(EffectManager) == 0x8b05c);

DIFFABLE_EXTERN(EffectManager, g_EffectManager);

} /* namespace th08 */
