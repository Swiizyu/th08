#pragma once

#include "ZunBool.hpp"
#include "diffbuild.hpp"
#include "inttypes.hpp"

namespace th08
{

struct SpellcardFlags
{
    u32 isActive : 1;
    u32 unk1 : 1;
    u32 unk2 : 1;
    u32 unk3 : 1;
    u32 unk4 : 1;
    u32 unk5 : 1;
    u32 unk6 : 1;
    u32 unk7 : 1;
    u32 unk8 : 1;
    u32 isCaptured : 1;
    u32 unk10 : 1;
};

struct Spellcard
{
    static i32 GetDifficultyFromSpellCard(i32 spellcardNumber);
    static ZunBool IsLastSpell(i32 spellcardNumber);
    static void CutChain();
    ZunBool FUN_00405260();
    ZunBool spellcard_fun_00417860();
    ZunBool spellcard_fun_004178a0();
    void spellcard_fun_00416130();
    void spellcard_fun_00416160();
    void FUN_00415d10(const char *name, void *owner);
    void EndSpell();
    void FUN_00416af0();
    void FUN_00416b10(i32 amount);
    i32 FUN_0041fdd0();
    ZunBool FUN_0042dff0();

    SpellcardFlags flags;
    unknown_fields(0x4, 0x2620);
    AnmLoaded *playerHumanFaceAnm;
    AnmLoaded *playerYoukaiFaceAnm;
    AnmLoaded *enemyFaceAnm;
    AnmLoaded *enemyFaceAnm2;
    unknown_fields(0x2634, 0xc);

    ZunBool IsActive()
    {
        return this->flags.isActive;
    }

    ZunBool IsCaptured()
    {
        return this->flags.isCaptured;
    }
};
C_ASSERT(sizeof(Spellcard) == 0x2640);

DIFFABLE_EXTERN(Spellcard, g_Spellcard);
DIFFABLE_EXTERN_ARRAY(i32 *, 6, g_SpellcardNumbersPerDifficulty);
DIFFABLE_EXTERN_ARRAY(i32, 6, g_SpellcardCountsPerDifficulty);
DIFFABLE_EXTERN_ARRAY(i32, 43, g_LastSpellNumbers);
DIFFABLE_EXTERN(i32, g_LastSpellCount);
DIFFABLE_EXTERN_ARRAY(i32 *, 10, g_SpellcardNumbersPerStage)
DIFFABLE_EXTERN_ARRAY(i32, 10, g_SpellcardCountPerStage)

} /* namespace th08 */
