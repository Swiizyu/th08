#pragma once

#include "ZunBool.hpp"
#include "ZunResult.hpp"
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "Supervisor.hpp"
#include "AnmManager.hpp"

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
    Spellcard();

    static i32 GetDifficultyFromSpellCard(i32 spellcardNumber);
    static ZunBool IsLastSpell(i32 spellcardNumber);
    static void CutChain();
    static ZunResult RegisterChain();
    ZunResult Init();
    void StartSpell(i32 spellcardNumber, const char *spellName, i32 duration, i32 bonus,
                    void *owner, const char *ownerName, const char *comment1, const char *comment2);
    ChainCallbackResult OnUpdateImpl();
    ChainCallbackResult OnDrawImpl();
    static ChainCallbackResult OnUpdate(Spellcard *spellcard);
    static ChainCallbackResult OnDraw(Spellcard *spellcard);
    static ZunResult DeletedCallback(Spellcard *spellcard);
    ZunBool FUN_00405260();
    ZunBool spellcard_fun_00417860();
    ZunBool spellcard_fun_004178a0();
    void spellcard_fun_00416130();
    void spellcard_fun_00416160();
    void FUN_00415d10(const char *name, void *owner);
    void FUN_00415d60(i32 face, const char *name, i32 script);
    void FUN_00415f00(i32 faceScript, const char *name, i32 script);
    void EndSpell();
    void FUN_00416af0();
    void FUN_00416b10(i32 amount);
    i32 FUN_0041fdd0();
    ZunBool FUN_0042dff0();

    SpellcardFlags flags;
    unknown_fields(0x4, 0x104);
    ZunTimer timer0x108;
    ZunTimer timer0x114;
    AnmVm vm0x120;
    AnmVm vm0x3c4;
    AnmVm vm0x668;
    AnmVm vm0x90c;
    AnmVm vm0xbb0;
    AnmVm vm0xe54;
    AnmVm vm0x10f8;
    AnmVm vm0x139c;
    AnmVm vm0x1640;
    AnmVm vm0x18e4;
    AnmVm vm0x1b88;
    AnmVm vm0x1e2c;
    AnmVm vm0x20d0;
    AnmVm vm0x2374;
    unknown_fields(0x2618, 0xc);
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
DIFFABLE_EXTERN_ARRAY(i32, 49, g_SpellcardNumbersNormal);
DIFFABLE_EXTERN(i32, g_LastSpellCount);
DIFFABLE_EXTERN_ARRAY(i32 *, 10, g_SpellcardNumbersPerStage)
DIFFABLE_EXTERN_ARRAY(i32, 10, g_SpellcardCountPerStage)

} /* namespace th08 */
