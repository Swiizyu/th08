#include "th_pch.h"

#include "ScoreDat.hpp"
#include "GameManager.hpp"
#include "Spellcard.hpp"
#include "Supervisor.hpp"
#include "utils.hpp"
#include <new>

u32 FUN_004338c0();

namespace th08
{

u32 IsDisableResourceReload();
// clang-format off
// TODO: stop clang-format from fucking with whitespace formatting

DIFFABLE_STATIC(Spellcard, g_Spellcard);
DIFFABLE_STATIC(ChainElem *, g_SpellcardCalcChain);
static ChainElem *g_SpellcardDrawChain;

// FUNCTION: th08 0x414590
ZunResult Spellcard::Init()
{
    this->flags.isActive = FALSE;
    this->flags.isCaptured = FALSE;
    *(void **)((u8 *)this + 4) = NULL;
    *(i32 *)((u8 *)this + 8) = -1;
    *(i32 *)((u8 *)this + 0xfc) = 0;
    *(i32 *)((u8 *)this + 0x100) = 0;
    ((ZunTimer *)((u8 *)this + 0x108))->Initialize();
    ((ZunTimer *)((u8 *)this + 0x114))->Initialize();
    for (i32 i = 0; i < 14; i++)
        ((AnmVm *)((u8 *)this + 0x120 + i * sizeof(AnmVm)))->Initialize();
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x4152a0
void Spellcard::StartSpell(i32 spellcardNumber, const char *spellName, i32 faceScript, i32 bonus,
                           void *owner, const char *ownerName, const char *comment1, const char *comment2)
{
    this->flags.isActive = TRUE;
    this->flags.isCaptured = TRUE;
    this->flags.unk4 = FALSE;
    *(i32 *)((u8 *)this + 8) = spellcardNumber;
    *(void **)((u8 *)this + 4) = owner;
    *(i32 *)((u8 *)this + 0xc) = owner != NULL ? *(i32 *)((u8 *)owner + 0x2e0c) : -1;
    *(i32 *)((u8 *)this + 0xfc) = bonus;
    *(i32 *)((u8 *)this + 0x2638) = bonus;
    ((ZunTimer *)((u8 *)this + 0x108))->SetCurrent(0);
    ((ZunTimer *)((u8 *)this + 0x114))->SetCurrent(0);
    if (owner != NULL) *(u32 *)((u8 *)owner + 0x3324) |= 0x08000000;
    if (spellcardNumber >= 0 && spellcardNumber < SPELLCARD_COUNT_SPELLCARDS)
    {
        Catk *catk = &g_GameManager.catkData[spellcardNumber];
        catk->spellcardNumber = (u16)spellcardNumber;
        catk->difficulty = (u8)g_GameManager.difficulty;
        if (spellName != NULL) strncpy(catk->spellName, spellName, sizeof(catk->spellName) - 1);
        if (ownerName != NULL) strncpy(catk->spellOwnerName, ownerName, sizeof(catk->spellOwnerName) - 1);
        if (comment1 != NULL) strncpy(catk->spellCommentLine1, comment1, sizeof(catk->spellCommentLine1) - 1);
        if (comment2 != NULL) strncpy(catk->spellCommentLine2, comment2, sizeof(catk->spellCommentLine2) - 1);
        catk->inGameHistory.attempts[g_GameManager.character]++;
        catk->inGameHistory.attempts[SHOT_ALL]++;
    }
    if (spellName != NULL) this->FUN_00415f00(faceScript, spellName, 0);
}

// FUNCTION: th08 0x416b90
ChainCallbackResult Spellcard::OnUpdateImpl()
{
    if (!this->flags.isActive) return CHAIN_CALLBACK_RESULT_CONTINUE;
    ((ZunTimer *)((u8 *)this + 0x108))->Tick();
    ((ZunTimer *)((u8 *)this + 0x114))->Tick();
    for (i32 i = 0; i < 14; i++)
    {
        AnmVm *vm = (AnmVm *)((u8 *)this + 0x120 + i * sizeof(AnmVm));
        if (vm->beginningOfScript != NULL) g_AnmManager->ExecuteScript(vm);
    }
    void *owner = *(void **)((u8 *)this + 4);
    if (owner != NULL && ((*(u32 *)((u8 *)owner + 0x3324) & 1) == 0 ||
                          *(i32 *)((u8 *)this + 0xc) != *(i32 *)((u8 *)owner + 0x2e0c)))
        this->FUN_00416af0();
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x4178c0
ChainCallbackResult Spellcard::OnDrawImpl()
{
    if (!this->flags.isActive) return CHAIN_CALLBACK_RESULT_CONTINUE;
    for (i32 i = 0; i < 14; i++)
    {
        AnmVm *vm = (AnmVm *)((u8 *)this + 0x120 + i * sizeof(AnmVm));
        if (vm->IsVisible()) g_AnmManager->Draw2D(vm);
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x418010
ChainCallbackResult Spellcard::OnUpdate(Spellcard *spellcard)
{
    return spellcard->OnUpdateImpl();
}

// FUNCTION: th08 0x418030
ChainCallbackResult Spellcard::OnDraw(Spellcard *spellcard)
{
    return spellcard->OnDrawImpl();
}

// FUNCTION: th08 0x417f60
#pragma var_order(spellcard)
ZunResult Spellcard::RegisterChain()
{
    Spellcard *spellcard;

    spellcard = &g_Spellcard;
    if (spellcard->Init() != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }
    *(ChainElem **)((u8 *)spellcard + 0x263c) = g_Chain.CreateElem((ChainCallback)Spellcard::OnUpdate);
    *(ChainElem **)((u8 *)spellcard + 0x2640) = g_Chain.CreateElem((ChainCallback)Spellcard::OnDraw);
    (*(ChainElem **)((u8 *)spellcard + 0x263c))->deletedCallback = (ChainLifetimeCallback)Spellcard::DeletedCallback;
    (*(ChainElem **)((u8 *)spellcard + 0x263c))->arg = spellcard;
    (*(ChainElem **)((u8 *)spellcard + 0x2640))->arg = spellcard;
    g_Chain.AddToCalcChain(*(ChainElem **)((u8 *)spellcard + 0x263c), 12);
    g_Chain.AddToDrawChain(*(ChainElem **)((u8 *)spellcard + 0x2640), 15);
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x4143e0
Spellcard::Spellcard()
{
}

// FUNCTION: th08 0x418050
ZunResult Spellcard::DeletedCallback(Spellcard *spellcard)
{
    if (!IsDisableResourceReload())
    {
        g_AnmManager->ReleaseAnm(18);
        g_AnmManager->ReleaseAnm(19);
    }
    if (FUN_004338c0())
    {
        g_AnmManager->ReleaseAnm(15);
        g_AnmManager->ReleaseAnm(16);
        g_AnmManager->ReleaseAnm(17);
    }
    if (*(ChainElem **)((u8 *)spellcard + 0x263c) != NULL)
    {
        (*(ChainElem **)((u8 *)spellcard + 0x263c))->deletedCallback = NULL;
    }
    g_Chain.Cut(*(ChainElem **)((u8 *)spellcard + 0x2640));
    *(ChainElem **)((u8 *)spellcard + 0x2640) = NULL;
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x416130
void Spellcard::spellcard_fun_00416130()
{
    *(i16 *)((u8 *)this + 0x12f6) = 1;
    ((AnmVm *)((u8 *)this + 0x1b88))->SetInterrupt(2);
}

// FUNCTION: th08 0x416160
void Spellcard::spellcard_fun_00416160()
{
    *(i16 *)((u8 *)this + 0x159a) = 1;
    ((AnmVm *)((u8 *)this + 0x1e2c))->SetInterrupt(2);
    if (this->flags.unk10 == 0)
    {
        ((AnmVm *)((u8 *)this + 0x2374))->SetInterrupt(2);
    }
}

// FUNCTION: th08 0x416af0
void Spellcard::FUN_00416af0()
{
    this->flags.isActive = 0;
    this->spellcard_fun_00416160();
}

// FUNCTION: th08 0x416b10
void Spellcard::FUN_00416b10(i32 amount)
{
    if (!(*(u32 *)this >> 11 & 1))
    {
        *(u32 *)((u8 *)this + 0xfc) += amount;
        if (*(u32 *)((u8 *)this + 0xfc) >= *(u32 *)((u8 *)this + 0x2638))
        {
            *(u32 *)((u8 *)this + 0xfc) = *(u32 *)((u8 *)this + 0x2638);
        }
        else
        {
            *(i32 *)((u8 *)this + 0x100) += amount / 120;
        }
    }
}

// FUNCTION: th08 0x414540
ZunBool Spellcard::IsLastSpell(i32 spellcardNumber)
{
    i32 i;

    for (i = 0; i < g_LastSpellCount; i++)
    {
        if (g_LastSpellNumbers[i] == spellcardNumber)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// FUNCTION: th08 0x40d3b0
i32 ZunTimer::AsFrames()
{
    return this->current;
}

// FUNCTION: th08 0x40d410
i32 ZunTimer::operator%(i32 value)
{
    return this->current % value;
}

// FUNCTION: th08 0x415d10
void Spellcard::FUN_00415d10(const char *name, void *)
{
    *(u32 *)this |= 0x400;
    *(u32 *)this &= ~1;
    *(u32 *)this &= ~0x10;
    this->FUN_00415f00(-1, name, 1);
}

// FUNCTION: th08 0x415d60
void Spellcard::FUN_00415d60(i32 face, const char *name, i32 script)
{
    AnmLoaded *playerFaceAnm = face == 0 ? *(AnmLoaded **)((u8 *)this + 0x2624)
                                             : *(AnmLoaded **)((u8 *)this + 0x2628);
    if (playerFaceAnm != NULL)
    {
        playerFaceAnm->SetAndExecuteScriptIdx((AnmVm *)((u8 *)this + 0x120), 0);
        g_AnmManager->ExecuteScript((AnmVm *)((u8 *)this + 0x120));
    }
    AnmLoaded *enemyFaceAnm = *(AnmLoaded **)((u8 *)this + 0x2634);
    if (enemyFaceAnm != NULL)
    {
        enemyFaceAnm->SetAndExecuteScriptIdx((AnmVm *)((u8 *)this + 0x668), 0);
        enemyFaceAnm->SetAndExecuteScriptIdx((AnmVm *)((u8 *)this + 0xbb0), 2);
        g_AnmManager->ExecuteScript((AnmVm *)((u8 *)this + 0x668));
        g_AnmManager->ExecuteScript((AnmVm *)((u8 *)this + 0xbb0));
    }
    *(f32 *)((u8 *)this + 0x2618) = strlen(name) * 15.0f / 2.0f + 64.0f;
    ((AnmVm *)((u8 *)this + 0x1b88))->SetInterrupt(1);
    *(i32 *)((u8 *)this + 0x260c) = script;
}

// FUNCTION: th08 0x415f00
void Spellcard::FUN_00415f00(i32 faceScript, const char *name, i32 script)
{
    AnmLoaded *faceAnm = *(AnmLoaded **)((u8 *)this + 0x262c);
    if (faceScript >= 0 && faceAnm != NULL)
    {
        faceAnm->SetAndExecuteScriptIdx((AnmVm *)((u8 *)this + 0x3c4), faceScript);
        g_AnmManager->ExecuteScript((AnmVm *)((u8 *)this + 0x3c4));
    }
    AnmLoaded *enemyFaceAnm = *(AnmLoaded **)((u8 *)this + 0x2634);
    if (enemyFaceAnm != NULL)
    {
        enemyFaceAnm->SetAndExecuteScriptIdx((AnmVm *)((u8 *)this + 0x668), 1);
        enemyFaceAnm->SetAndExecuteScriptIdx((AnmVm *)((u8 *)this + 0xbb0), 3);
        g_AnmManager->ExecuteScript((AnmVm *)((u8 *)this + 0x668));
        g_AnmManager->ExecuteScript((AnmVm *)((u8 *)this + 0xbb0));
    }
    *(f32 *)((u8 *)this + 0x261c) = strlen(name) * 15.0f / 2.0f + 64.0f;
    ((AnmVm *)((u8 *)this + 0x1e2c))->SetInterrupt(1);
    if ((*(u32 *)this & 0x400) == 0)
    {
        ((AnmVm *)((u8 *)this + 0x2374))->SetInterrupt(1);
    }
    *(i32 *)((u8 *)this + 0x2610) = script;
}

// FUNCTION: th08 0x4161b0
void Spellcard::EndSpell()
{
    if ((*(u32 *)this & 1) != 0)
    {
        *(u32 *)this &= ~1;
        this->spellcard_fun_00416160();
    }
    if (*(void **)((u8 *)this + 4) != NULL)
    {
        *(u32 *)((u8 *)*(void **)((u8 *)this + 4) + 0x3324) &= ~0x08000000;
    }
    *(void **)((u8 *)this + 4) = NULL;
    *(u32 *)this &= ~0x800;
}

// FUNCTION: th08 0x405260
ZunBool Spellcard::FUN_00405260()
{
    return this->flags.isCaptured;
}

// FUNCTION: th08 0x4178a0
ZunBool Spellcard::spellcard_fun_004178a0()
{
    return this->flags.isActive;
}

// FUNCTION: th08 0x417860
ZunBool Spellcard::spellcard_fun_00417860()
{
    return this->spellcard_fun_004178a0() && this->flags.unk5;
}

// FUNCTION: th08 0x41fdd0
i32 Spellcard::FUN_0041fdd0()
{
    return *(ZunTimer *)((u8 *)this + 0x108);
}

// FUNCTION: th08 0x42dff0
ZunBool Spellcard::FUN_0042dff0()
{
    return this->flags.unk7;
}

// FUNCTION: th08 0x4180f0
void Spellcard::CutChain()
{
    if (g_SpellcardCalcChain != NULL)
    {
        g_Chain.Cut(g_SpellcardCalcChain);
    }
}

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 42, g_SpellcardNumbersEasy) = {
    // Stage 1
    SPELLCARD_ST1_BOSS_1E,    SPELLCARD_ST1_BOSS_2E,
    // Stage 2
    SPELLCARD_ST2_MBOSS_1E,   SPELLCARD_ST2_BOSS_1E,    SPELLCARD_ST2_BOSS_2E,    SPELLCARD_ST2_BOSS_3E,
    // Stage 3
    SPELLCARD_ST3_MBOSS_1E,   SPELLCARD_ST3_BOSS_2E,    SPELLCARD_ST3_BOSS_3E,    SPELLCARD_ST3_BOSS_4E,
    // Stage 4A
    SPELLCARD_ST4A_MBOSS_1E,  SPELLCARD_ST4A_MBOSS_2E,  SPELLCARD_ST4A_BOSS_1E,   SPELLCARD_ST4A_BOSS_2E,
    SPELLCARD_ST4A_BOSS_3E,
    // Stage 4B
    SPELLCARD_ST4B_MBOSS_1E,  SPELLCARD_ST4B_MBOSS_2E,  SPELLCARD_ST4B_BOSS_1E,   SPELLCARD_ST4B_BOSS_2E,
    SPELLCARD_ST4B_BOSS_3E,
    // Stage 5
    SPELLCARD_ST5_BOSS_1E,    SPELLCARD_ST5_BOSS_2E,    SPELLCARD_ST5_BOSS_3E,    SPELLCARD_ST5_BOSS_4E,
    // Stage 6A
    SPELLCARD_ST6A_MBOSS_1E,  SPELLCARD_ST6A_BOSS_1E,   SPELLCARD_ST6A_BOSS_2E,   SPELLCARD_ST6A_BOSS_3E,
    SPELLCARD_ST6A_BOSS_4E,   SPELLCARD_ST6A_BOSS_5E,   SPELLCARD_ST6A_BOSS_LSE,
    // Stage 6B
    SPELLCARD_ST6B_MBOSS_1E,  SPELLCARD_ST6B_BOSS_1E,   SPELLCARD_ST6B_BOSS_2E,   SPELLCARD_ST6B_BOSS_3E,
    SPELLCARD_ST6B_BOSS_4E,   SPELLCARD_ST6B_BOSS_5E,   SPELLCARD_ST6B_BOSS_LS1E, SPELLCARD_ST6B_BOSS_LS2E,
    SPELLCARD_ST6B_BOSS_LS3E, SPELLCARD_ST6B_BOSS_LS4E, SPELLCARD_ST6B_BOSS_LS5E,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 49, g_SpellcardNumbersNormal) = {
    // Stage 1
    SPELLCARD_ST1_BOSS_1N,    SPELLCARD_ST1_BOSS_2N,    SPELLCARD_ST1_BOSS_LSN,
    // Stage 2
    SPELLCARD_ST2_MBOSS_1N,   SPELLCARD_ST2_BOSS_1N,    SPELLCARD_ST2_BOSS_2N,    SPELLCARD_ST2_BOSS_3N,
    SPELLCARD_ST2_BOSS_LSN,
    // Stage 3
    SPELLCARD_ST3_MBOSS_1N,   SPELLCARD_ST3_BOSS_1N,    SPELLCARD_ST3_BOSS_2N,    SPELLCARD_ST3_BOSS_3N,
    SPELLCARD_ST3_BOSS_4N,    SPELLCARD_ST3_BOSS_LSN,
    // Stage 4A
    SPELLCARD_ST4A_MBOSS_1N,  SPELLCARD_ST4A_MBOSS_2N,  SPELLCARD_ST4A_BOSS_1N,   SPELLCARD_ST4A_BOSS_2N,
    SPELLCARD_ST4A_BOSS_3N,   SPELLCARD_ST4A_BOSS_LSN,
    // Stage 4B
    SPELLCARD_ST4B_MBOSS_1N,  SPELLCARD_ST4B_MBOSS_2N,  SPELLCARD_ST4B_BOSS_1N,   SPELLCARD_ST4B_BOSS_2N,
    SPELLCARD_ST4B_BOSS_3N,   SPELLCARD_ST4B_BOSS_LSN,
    // Stage 5
    SPELLCARD_ST5_BOSS_1N,    SPELLCARD_ST5_BOSS_2N,    SPELLCARD_ST5_BOSS_3N,    SPELLCARD_ST5_BOSS_4N,
    SPELLCARD_ST5_BOSS_LSN,
    // Stage 6A
    SPELLCARD_ST6A_MBOSS_1N,  SPELLCARD_ST6A_BOSS_1N,   SPELLCARD_ST6A_BOSS_2N,   SPELLCARD_ST6A_BOSS_3N,
    SPELLCARD_ST6A_BOSS_4N,   SPELLCARD_ST6A_BOSS_5N,   SPELLCARD_ST6A_BOSS_LSN,
    // Stage 6B
    SPELLCARD_ST6B_MBOSS_1N,  SPELLCARD_ST6B_BOSS_1N,   SPELLCARD_ST6B_BOSS_2N,   SPELLCARD_ST6B_BOSS_3N,
    SPELLCARD_ST6B_BOSS_4N,   SPELLCARD_ST6B_BOSS_5N,   SPELLCARD_ST6B_BOSS_LS1N, SPELLCARD_ST6B_BOSS_LS2N,
    SPELLCARD_ST6B_BOSS_LS3N, SPELLCARD_ST6B_BOSS_LS4N, SPELLCARD_ST6B_BOSS_LS5N,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 50, g_SpellcardNumbersHard) = {
    // Stage 1
    SPELLCARD_ST1_MBOSS_1H,   SPELLCARD_ST1_BOSS_1H,    SPELLCARD_ST1_BOSS_2H,    SPELLCARD_ST1_BOSS_LSH,
    // Stage 2
    SPELLCARD_ST2_MBOSS_1H,   SPELLCARD_ST2_BOSS_1H,    SPELLCARD_ST2_BOSS_2H,    SPELLCARD_ST2_BOSS_3H,
    SPELLCARD_ST2_BOSS_LSH,
    // Stage 3
    SPELLCARD_ST3_MBOSS_1H,   SPELLCARD_ST3_BOSS_1H,    SPELLCARD_ST3_BOSS_2H,    SPELLCARD_ST3_BOSS_3H,
    SPELLCARD_ST3_BOSS_4H,    SPELLCARD_ST3_BOSS_LSH,
    // Stage 4A
    SPELLCARD_ST4A_MBOSS_1H,  SPELLCARD_ST4A_MBOSS_2H,  SPELLCARD_ST4A_BOSS_1H,   SPELLCARD_ST4A_BOSS_2H,
    SPELLCARD_ST4A_BOSS_3H,   SPELLCARD_ST4A_BOSS_LSH,
    // Stage 4B
    SPELLCARD_ST4B_MBOSS_1H,  SPELLCARD_ST4B_MBOSS_2H,  SPELLCARD_ST4B_BOSS_1H,   SPELLCARD_ST4B_BOSS_2H,
    SPELLCARD_ST4B_BOSS_3H,   SPELLCARD_ST4B_BOSS_LSH,
    // Stage 5
    SPELLCARD_ST5_BOSS_1H,    SPELLCARD_ST5_BOSS_2H,    SPELLCARD_ST5_BOSS_3H,    SPELLCARD_ST5_BOSS_4H,
    SPELLCARD_ST5_BOSS_LSH,
    // Stage 6A
    SPELLCARD_ST6A_MBOSS_1H,  SPELLCARD_ST6A_BOSS_1H,   SPELLCARD_ST6A_BOSS_2H,   SPELLCARD_ST6A_BOSS_3H,
    SPELLCARD_ST6A_BOSS_4H,   SPELLCARD_ST6A_BOSS_5H,   SPELLCARD_ST6A_BOSS_LSH,
    // Stage 6B
    SPELLCARD_ST6B_MBOSS_1H,  SPELLCARD_ST6B_BOSS_1H,   SPELLCARD_ST6B_BOSS_2H,   SPELLCARD_ST6B_BOSS_3H,
    SPELLCARD_ST6B_BOSS_4H,   SPELLCARD_ST6B_BOSS_5H,   SPELLCARD_ST6B_BOSS_LS1H, SPELLCARD_ST6B_BOSS_LS2H,
    SPELLCARD_ST6B_BOSS_LS3H, SPELLCARD_ST6B_BOSS_LS4H, SPELLCARD_ST6B_BOSS_LS5H,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 50, g_SpellcardNumbersLunatic) = {
    // Stage 1
    SPELLCARD_ST1_MBOSS_1L,   SPELLCARD_ST1_BOSS_1L,    SPELLCARD_ST1_BOSS_2L,    SPELLCARD_ST1_BOSS_LSL,
    // Stage 2
    SPELLCARD_ST2_MBOSS_1L,   SPELLCARD_ST2_BOSS_1L,    SPELLCARD_ST2_BOSS_2L,    SPELLCARD_ST2_BOSS_3L,
    SPELLCARD_ST2_BOSS_LSL,
    // Stage 3
    SPELLCARD_ST3_MBOSS_1L,   SPELLCARD_ST3_BOSS_1L,    SPELLCARD_ST3_BOSS_2L,    SPELLCARD_ST3_BOSS_3L,
    SPELLCARD_ST3_BOSS_4L,    SPELLCARD_ST3_BOSS_LSL,
    // Stage 4A
    SPELLCARD_ST4A_MBOSS_1L,  SPELLCARD_ST4A_MBOSS_2L,  SPELLCARD_ST4A_BOSS_1L,   SPELLCARD_ST4A_BOSS_2L,
    SPELLCARD_ST4A_BOSS_3L,   SPELLCARD_ST4A_BOSS_LSL,
    // Stage 4B
    SPELLCARD_ST4B_MBOSS_1L,  SPELLCARD_ST4B_MBOSS_2L,  SPELLCARD_ST4B_BOSS_1L,   SPELLCARD_ST4B_BOSS_2L,
    SPELLCARD_ST4B_BOSS_3L,   SPELLCARD_ST4B_BOSS_LSL,
    // Stage 5
    SPELLCARD_ST5_BOSS_1L,    SPELLCARD_ST5_BOSS_2L,    SPELLCARD_ST5_BOSS_3L,    SPELLCARD_ST5_BOSS_4L,
    SPELLCARD_ST5_BOSS_LSL,
    // Stage 6A
    SPELLCARD_ST6A_MBOSS_1L,  SPELLCARD_ST6A_BOSS_1L,   SPELLCARD_ST6A_BOSS_2L,   SPELLCARD_ST6A_BOSS_3L,
    SPELLCARD_ST6A_BOSS_4L,   SPELLCARD_ST6A_BOSS_5L,   SPELLCARD_ST6A_BOSS_LSL,
    // Stage 6B
    SPELLCARD_ST6B_MBOSS_1L,  SPELLCARD_ST6B_BOSS_1L,   SPELLCARD_ST6B_BOSS_2L,   SPELLCARD_ST6B_BOSS_3L,
    SPELLCARD_ST6B_BOSS_4L,   SPELLCARD_ST6B_BOSS_5L,   SPELLCARD_ST6B_BOSS_LS1L, SPELLCARD_ST6B_BOSS_LS2L,
    SPELLCARD_ST6B_BOSS_LS3L, SPELLCARD_ST6B_BOSS_LS4L, SPELLCARD_ST6B_BOSS_LS5L,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 14, g_SpellcardNumbersExtra) = {
    SPELLCARD_EX_MBOSS_1, SPELLCARD_EX_MBOSS_2, SPELLCARD_EX_MBOSS_3,
    SPELLCARD_EX_BOSS_1,  SPELLCARD_EX_BOSS_2,  SPELLCARD_EX_BOSS_3,  SPELLCARD_EX_BOSS_4,
    SPELLCARD_EX_BOSS_5,  SPELLCARD_EX_BOSS_6,  SPELLCARD_EX_BOSS_7,  SPELLCARD_EX_BOSS_8,
    SPELLCARD_EX_BOSS_9,  SPELLCARD_EX_BOSS_10, SPELLCARD_EX_BOSS_LS,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, SPELLCARD_COUNT_SPELLCARDS, g_SpellcardNumbersAllDifficulties) = {
    // Stage 1
                                                        SPELLCARD_ST1_MBOSS_1H,   SPELLCARD_ST1_MBOSS_1L,
    SPELLCARD_ST1_BOSS_1E,    SPELLCARD_ST1_BOSS_1N,    SPELLCARD_ST1_BOSS_1H,    SPELLCARD_ST1_BOSS_1L,
    SPELLCARD_ST1_BOSS_2E,    SPELLCARD_ST1_BOSS_2N,    SPELLCARD_ST1_BOSS_2H,    SPELLCARD_ST1_BOSS_2L,
                              SPELLCARD_ST1_BOSS_LSN,   SPELLCARD_ST1_BOSS_LSH,   SPELLCARD_ST1_BOSS_LSL,
    // Stage 2
    SPELLCARD_ST2_MBOSS_1E,   SPELLCARD_ST2_MBOSS_1N,   SPELLCARD_ST2_MBOSS_1H,   SPELLCARD_ST2_MBOSS_1L,
    SPELLCARD_ST2_BOSS_1E,    SPELLCARD_ST2_BOSS_1N,    SPELLCARD_ST2_BOSS_1H,    SPELLCARD_ST2_BOSS_1L,
    SPELLCARD_ST2_BOSS_2E,    SPELLCARD_ST2_BOSS_2N,    SPELLCARD_ST2_BOSS_2H,    SPELLCARD_ST2_BOSS_2L,
    SPELLCARD_ST2_BOSS_3E,    SPELLCARD_ST2_BOSS_3N,    SPELLCARD_ST2_BOSS_3H,    SPELLCARD_ST2_BOSS_3L,
                              SPELLCARD_ST2_BOSS_LSN,   SPELLCARD_ST2_BOSS_LSH,   SPELLCARD_ST2_BOSS_LSL,
    // Stage 3
    SPELLCARD_ST3_MBOSS_1E,   SPELLCARD_ST3_MBOSS_1N,   SPELLCARD_ST3_MBOSS_1H,   SPELLCARD_ST3_MBOSS_1L,
                              SPELLCARD_ST3_BOSS_1N,    SPELLCARD_ST3_BOSS_1H,    SPELLCARD_ST3_BOSS_1L,
    SPELLCARD_ST3_BOSS_2E,    SPELLCARD_ST3_BOSS_2N,    SPELLCARD_ST3_BOSS_2H,    SPELLCARD_ST3_BOSS_2L,
    SPELLCARD_ST3_BOSS_3E,    SPELLCARD_ST3_BOSS_3N,    SPELLCARD_ST3_BOSS_3H,    SPELLCARD_ST3_BOSS_3L,
    SPELLCARD_ST3_BOSS_4E,    SPELLCARD_ST3_BOSS_4N,    SPELLCARD_ST3_BOSS_4H,    SPELLCARD_ST3_BOSS_4L,
                              SPELLCARD_ST3_BOSS_LSN,   SPELLCARD_ST3_BOSS_LSH,   SPELLCARD_ST3_BOSS_LSL,
    // Stage 4A
    SPELLCARD_ST4A_MBOSS_1E,  SPELLCARD_ST4A_MBOSS_1N,  SPELLCARD_ST4A_MBOSS_1H,  SPELLCARD_ST4A_MBOSS_1L,
    SPELLCARD_ST4A_MBOSS_2E,  SPELLCARD_ST4A_MBOSS_2N,  SPELLCARD_ST4A_MBOSS_2H,  SPELLCARD_ST4A_MBOSS_2L,
    SPELLCARD_ST4A_BOSS_1E,   SPELLCARD_ST4A_BOSS_1N,   SPELLCARD_ST4A_BOSS_1H,   SPELLCARD_ST4A_BOSS_1L,
    SPELLCARD_ST4A_BOSS_2E,   SPELLCARD_ST4A_BOSS_2N,   SPELLCARD_ST4A_BOSS_2H,   SPELLCARD_ST4A_BOSS_2L,
    SPELLCARD_ST4A_BOSS_3E,   SPELLCARD_ST4A_BOSS_3N,   SPELLCARD_ST4A_BOSS_3H,   SPELLCARD_ST4A_BOSS_3L,
                              SPELLCARD_ST4A_BOSS_LSN,  SPELLCARD_ST4A_BOSS_LSH,  SPELLCARD_ST4A_BOSS_LSL,
    // Stage 4B
    SPELLCARD_ST4B_MBOSS_1E,  SPELLCARD_ST4B_MBOSS_1N,  SPELLCARD_ST4B_MBOSS_1H,  SPELLCARD_ST4B_MBOSS_1L,
    SPELLCARD_ST4B_MBOSS_2E,  SPELLCARD_ST4B_MBOSS_2N,  SPELLCARD_ST4B_MBOSS_2H,  SPELLCARD_ST4B_MBOSS_2L,
    SPELLCARD_ST4B_BOSS_1E,   SPELLCARD_ST4B_BOSS_1N,   SPELLCARD_ST4B_BOSS_1H,   SPELLCARD_ST4B_BOSS_1L,
    SPELLCARD_ST4B_BOSS_2E,   SPELLCARD_ST4B_BOSS_2N,   SPELLCARD_ST4B_BOSS_2H,   SPELLCARD_ST4B_BOSS_2L,
    SPELLCARD_ST4B_BOSS_3E,   SPELLCARD_ST4B_BOSS_3N,   SPELLCARD_ST4B_BOSS_3H,   SPELLCARD_ST4B_BOSS_3L,
                              SPELLCARD_ST4B_BOSS_LSN,  SPELLCARD_ST4B_BOSS_LSH,  SPELLCARD_ST4B_BOSS_LSL,
    // Stage 5
    SPELLCARD_ST5_BOSS_1E,    SPELLCARD_ST5_BOSS_1N,    SPELLCARD_ST5_BOSS_1H,    SPELLCARD_ST5_BOSS_1L,
    SPELLCARD_ST5_BOSS_2E,    SPELLCARD_ST5_BOSS_2N,    SPELLCARD_ST5_BOSS_2H,    SPELLCARD_ST5_BOSS_2L,
    SPELLCARD_ST5_BOSS_3E,    SPELLCARD_ST5_BOSS_3N,    SPELLCARD_ST5_BOSS_3H,    SPELLCARD_ST5_BOSS_3L,
    SPELLCARD_ST5_BOSS_4E,    SPELLCARD_ST5_BOSS_4N,    SPELLCARD_ST5_BOSS_4H,    SPELLCARD_ST5_BOSS_4L,
                              SPELLCARD_ST5_BOSS_LSN,   SPELLCARD_ST5_BOSS_LSH,   SPELLCARD_ST5_BOSS_LSL,
    // Stage 6A
    SPELLCARD_ST6A_MBOSS_1E,  SPELLCARD_ST6A_MBOSS_1N,  SPELLCARD_ST6A_MBOSS_1H,  SPELLCARD_ST6A_MBOSS_1L,
    SPELLCARD_ST6A_BOSS_1E,   SPELLCARD_ST6A_BOSS_1N,   SPELLCARD_ST6A_BOSS_1H,   SPELLCARD_ST6A_BOSS_1L,
    SPELLCARD_ST6A_BOSS_2E,   SPELLCARD_ST6A_BOSS_2N,   SPELLCARD_ST6A_BOSS_2H,   SPELLCARD_ST6A_BOSS_2L,
    SPELLCARD_ST6A_BOSS_3E,   SPELLCARD_ST6A_BOSS_3N,   SPELLCARD_ST6A_BOSS_3H,   SPELLCARD_ST6A_BOSS_3L,
    SPELLCARD_ST6A_BOSS_4E,   SPELLCARD_ST6A_BOSS_4N,   SPELLCARD_ST6A_BOSS_4H,   SPELLCARD_ST6A_BOSS_4L,
    SPELLCARD_ST6A_BOSS_5E,   SPELLCARD_ST6A_BOSS_5N,   SPELLCARD_ST6A_BOSS_5H,   SPELLCARD_ST6A_BOSS_5L,
    SPELLCARD_ST6A_BOSS_LSE,  SPELLCARD_ST6A_BOSS_LSN,  SPELLCARD_ST6A_BOSS_LSH,  SPELLCARD_ST6A_BOSS_LSL,
    // Stage 6B
    SPELLCARD_ST6B_MBOSS_1E,  SPELLCARD_ST6B_MBOSS_1N,  SPELLCARD_ST6B_MBOSS_1H,  SPELLCARD_ST6B_MBOSS_1L,
    SPELLCARD_ST6B_BOSS_1E,   SPELLCARD_ST6B_BOSS_1N,   SPELLCARD_ST6B_BOSS_1H,   SPELLCARD_ST6B_BOSS_1L,
    SPELLCARD_ST6B_BOSS_2E,   SPELLCARD_ST6B_BOSS_2N,   SPELLCARD_ST6B_BOSS_2H,   SPELLCARD_ST6B_BOSS_2L,
    SPELLCARD_ST6B_BOSS_3E,   SPELLCARD_ST6B_BOSS_3N,   SPELLCARD_ST6B_BOSS_3H,   SPELLCARD_ST6B_BOSS_3L,
    SPELLCARD_ST6B_BOSS_4E,   SPELLCARD_ST6B_BOSS_4N,   SPELLCARD_ST6B_BOSS_4H,   SPELLCARD_ST6B_BOSS_4L,
    SPELLCARD_ST6B_BOSS_5E,   SPELLCARD_ST6B_BOSS_5N,   SPELLCARD_ST6B_BOSS_5H,   SPELLCARD_ST6B_BOSS_5L,
    SPELLCARD_ST6B_BOSS_LS1E, SPELLCARD_ST6B_BOSS_LS1N, SPELLCARD_ST6B_BOSS_LS1H, SPELLCARD_ST6B_BOSS_LS1L,
    SPELLCARD_ST6B_BOSS_LS2E, SPELLCARD_ST6B_BOSS_LS2N, SPELLCARD_ST6B_BOSS_LS2H, SPELLCARD_ST6B_BOSS_LS2L,
    SPELLCARD_ST6B_BOSS_LS3E, SPELLCARD_ST6B_BOSS_LS3N, SPELLCARD_ST6B_BOSS_LS3H, SPELLCARD_ST6B_BOSS_LS3L,
    SPELLCARD_ST6B_BOSS_LS4E, SPELLCARD_ST6B_BOSS_LS4N, SPELLCARD_ST6B_BOSS_LS4H, SPELLCARD_ST6B_BOSS_LS4L,
    SPELLCARD_ST6B_BOSS_LS5E, SPELLCARD_ST6B_BOSS_LS5N, SPELLCARD_ST6B_BOSS_LS5H, SPELLCARD_ST6B_BOSS_LS5L,
    // Extra Stage
    SPELLCARD_EX_MBOSS_1,     SPELLCARD_EX_MBOSS_2,     SPELLCARD_EX_MBOSS_3,
    SPELLCARD_EX_BOSS_1,      SPELLCARD_EX_BOSS_2,      SPELLCARD_EX_BOSS_3,      SPELLCARD_EX_BOSS_4,
    SPELLCARD_EX_BOSS_5,      SPELLCARD_EX_BOSS_6,      SPELLCARD_EX_BOSS_7,      SPELLCARD_EX_BOSS_8,
    SPELLCARD_EX_BOSS_9,      SPELLCARD_EX_BOSS_10,     SPELLCARD_EX_BOSS_LS,
    // Last Word
    SPELLCARD_LW_WRIGGLE,     SPELLCARD_LW_MYSTIA,      SPELLCARD_LW_KEINE,       SPELLCARD_LW_REISEN,
    SPELLCARD_LW_EIRIN,       SPELLCARD_LW_KAGUYA,      SPELLCARD_LW_MOKOU,       SPELLCARD_LW_TEWI,
    SPELLCARD_LW_KEINEEX,     SPELLCARD_LW_REIMU,       SPELLCARD_LW_MARISA,      SPELLCARD_LW_SAKUYA,
    SPELLCARD_LW_YOUMU,       SPELLCARD_LW_ALICE,       SPELLCARD_LW_REMILIA,     SPELLCARD_LW_YUYUKO,
    SPELLCARD_LW_YUKARI,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32 *, 6, g_SpellcardNumbersPerDifficulty) = {
    g_SpellcardNumbersEasy,    g_SpellcardNumbersNormal, g_SpellcardNumbersHard,
    g_SpellcardNumbersLunatic, g_SpellcardNumbersExtra,  g_SpellcardNumbersAllDifficulties};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 6, g_SpellcardCountsPerDifficulty) = {
    ARRAY_SIZE_SIGNED(g_SpellcardNumbersEasy),  ARRAY_SIZE_SIGNED(g_SpellcardNumbersNormal),
    ARRAY_SIZE_SIGNED(g_SpellcardNumbersHard),  ARRAY_SIZE_SIGNED(g_SpellcardNumbersLunatic),
    ARRAY_SIZE_SIGNED(g_SpellcardNumbersExtra), ARRAY_SIZE_SIGNED(g_SpellcardNumbersAllDifficulties)};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 13, g_SpellcardNumbersStage1) = {
                                                   SPELLCARD_ST1_MBOSS_1H, SPELLCARD_ST1_MBOSS_1L,
    SPELLCARD_ST1_BOSS_1E, SPELLCARD_ST1_BOSS_1N,  SPELLCARD_ST1_BOSS_1H,  SPELLCARD_ST1_BOSS_1L,
    SPELLCARD_ST1_BOSS_2E, SPELLCARD_ST1_BOSS_2N,  SPELLCARD_ST1_BOSS_2H,  SPELLCARD_ST1_BOSS_2L,
                           SPELLCARD_ST1_BOSS_LSN, SPELLCARD_ST1_BOSS_LSH, SPELLCARD_ST1_BOSS_LSL,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 19, g_SpellcardNumbersStage2) = {
    SPELLCARD_ST2_MBOSS_1E, SPELLCARD_ST2_MBOSS_1N, SPELLCARD_ST2_MBOSS_1H, SPELLCARD_ST2_MBOSS_1L,
    SPELLCARD_ST2_BOSS_1E,  SPELLCARD_ST2_BOSS_1N,  SPELLCARD_ST2_BOSS_1H,  SPELLCARD_ST2_BOSS_1L,
    SPELLCARD_ST2_BOSS_2E,  SPELLCARD_ST2_BOSS_2N,  SPELLCARD_ST2_BOSS_2H,  SPELLCARD_ST2_BOSS_2L,
    SPELLCARD_ST2_BOSS_3E,  SPELLCARD_ST2_BOSS_3N,  SPELLCARD_ST2_BOSS_3H,  SPELLCARD_ST2_BOSS_3L,
                            SPELLCARD_ST2_BOSS_LSN, SPELLCARD_ST2_BOSS_LSH, SPELLCARD_ST2_BOSS_LSL,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 22, g_SpellcardNumbersStage3) = {
    SPELLCARD_ST3_MBOSS_1E, SPELLCARD_ST3_MBOSS_1N, SPELLCARD_ST3_MBOSS_1H, SPELLCARD_ST3_MBOSS_1L,
                            SPELLCARD_ST3_BOSS_1N,  SPELLCARD_ST3_BOSS_1H,  SPELLCARD_ST3_BOSS_1L,
    SPELLCARD_ST3_BOSS_2E,  SPELLCARD_ST3_BOSS_2N,  SPELLCARD_ST3_BOSS_2H,  SPELLCARD_ST3_BOSS_2L,
    SPELLCARD_ST3_BOSS_3E,  SPELLCARD_ST3_BOSS_3N,  SPELLCARD_ST3_BOSS_3H,  SPELLCARD_ST3_BOSS_3L,
    SPELLCARD_ST3_BOSS_4E,  SPELLCARD_ST3_BOSS_4N,  SPELLCARD_ST3_BOSS_4H,  SPELLCARD_ST3_BOSS_4L,
                            SPELLCARD_ST3_BOSS_LSN, SPELLCARD_ST3_BOSS_LSH, SPELLCARD_ST3_BOSS_LSL,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 23, g_SpellcardNumbersStage4A) = {
    SPELLCARD_ST4A_MBOSS_1E, SPELLCARD_ST4A_MBOSS_1N, SPELLCARD_ST4A_MBOSS_1H, SPELLCARD_ST4A_MBOSS_1L,
    SPELLCARD_ST4A_MBOSS_2E, SPELLCARD_ST4A_MBOSS_2N, SPELLCARD_ST4A_MBOSS_2H, SPELLCARD_ST4A_MBOSS_2L,
    SPELLCARD_ST4A_BOSS_1E,  SPELLCARD_ST4A_BOSS_1N,  SPELLCARD_ST4A_BOSS_1H,  SPELLCARD_ST4A_BOSS_1L,
    SPELLCARD_ST4A_BOSS_2E,  SPELLCARD_ST4A_BOSS_2N,  SPELLCARD_ST4A_BOSS_2H,  SPELLCARD_ST4A_BOSS_2L,
    SPELLCARD_ST4A_BOSS_3E,  SPELLCARD_ST4A_BOSS_3N,  SPELLCARD_ST4A_BOSS_3H,  SPELLCARD_ST4A_BOSS_3L,
                             SPELLCARD_ST4A_BOSS_LSN, SPELLCARD_ST4A_BOSS_LSH, SPELLCARD_ST4A_BOSS_LSL,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 23, g_SpellcardNumbersStage4B) = {
    SPELLCARD_ST4B_MBOSS_1E, SPELLCARD_ST4B_MBOSS_1N, SPELLCARD_ST4B_MBOSS_1H, SPELLCARD_ST4B_MBOSS_1L,
    SPELLCARD_ST4B_MBOSS_2E, SPELLCARD_ST4B_MBOSS_2N, SPELLCARD_ST4B_MBOSS_2H, SPELLCARD_ST4B_MBOSS_2L,
    SPELLCARD_ST4B_BOSS_1E,  SPELLCARD_ST4B_BOSS_1N,  SPELLCARD_ST4B_BOSS_1H,  SPELLCARD_ST4B_BOSS_1L,
    SPELLCARD_ST4B_BOSS_2E,  SPELLCARD_ST4B_BOSS_2N,  SPELLCARD_ST4B_BOSS_2H,  SPELLCARD_ST4B_BOSS_2L,
    SPELLCARD_ST4B_BOSS_3E,  SPELLCARD_ST4B_BOSS_3N,  SPELLCARD_ST4B_BOSS_3H,  SPELLCARD_ST4B_BOSS_3L,
                             SPELLCARD_ST4B_BOSS_LSN, SPELLCARD_ST4B_BOSS_LSH, SPELLCARD_ST4B_BOSS_LSL,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 19, g_SpellcardNumbersStage5) = {
    SPELLCARD_ST5_BOSS_1E, SPELLCARD_ST5_BOSS_1N,  SPELLCARD_ST5_BOSS_1H,  SPELLCARD_ST5_BOSS_1L,
    SPELLCARD_ST5_BOSS_2E, SPELLCARD_ST5_BOSS_2N,  SPELLCARD_ST5_BOSS_2H,  SPELLCARD_ST5_BOSS_2L,
    SPELLCARD_ST5_BOSS_3E, SPELLCARD_ST5_BOSS_3N,  SPELLCARD_ST5_BOSS_3H,  SPELLCARD_ST5_BOSS_3L,
    SPELLCARD_ST5_BOSS_4E, SPELLCARD_ST5_BOSS_4N,  SPELLCARD_ST5_BOSS_4H,  SPELLCARD_ST5_BOSS_4L,
                           SPELLCARD_ST5_BOSS_LSN, SPELLCARD_ST5_BOSS_LSH, SPELLCARD_ST5_BOSS_LSL,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 28, g_SpellcardNumbersStage6A) = {
    SPELLCARD_ST6A_MBOSS_1E, SPELLCARD_ST6A_MBOSS_1N, SPELLCARD_ST6A_MBOSS_1H, SPELLCARD_ST6A_MBOSS_1L,
    SPELLCARD_ST6A_BOSS_1E,  SPELLCARD_ST6A_BOSS_1N,  SPELLCARD_ST6A_BOSS_1H,  SPELLCARD_ST6A_BOSS_1L,
    SPELLCARD_ST6A_BOSS_2E,  SPELLCARD_ST6A_BOSS_2N,  SPELLCARD_ST6A_BOSS_2H,  SPELLCARD_ST6A_BOSS_2L,
    SPELLCARD_ST6A_BOSS_3E,  SPELLCARD_ST6A_BOSS_3N,  SPELLCARD_ST6A_BOSS_3H,  SPELLCARD_ST6A_BOSS_3L,
    SPELLCARD_ST6A_BOSS_4E,  SPELLCARD_ST6A_BOSS_4N,  SPELLCARD_ST6A_BOSS_4H,  SPELLCARD_ST6A_BOSS_4L,
    SPELLCARD_ST6A_BOSS_5E,  SPELLCARD_ST6A_BOSS_5N,  SPELLCARD_ST6A_BOSS_5H,  SPELLCARD_ST6A_BOSS_5L,
    SPELLCARD_ST6A_BOSS_LSE, SPELLCARD_ST6A_BOSS_LSN, SPELLCARD_ST6A_BOSS_LSH, SPELLCARD_ST6A_BOSS_LSL,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 44, g_SpellcardNumbersStage6B) = {
    SPELLCARD_ST6B_MBOSS_1E,  SPELLCARD_ST6B_MBOSS_1N,  SPELLCARD_ST6B_MBOSS_1H,  SPELLCARD_ST6B_MBOSS_1L,
    SPELLCARD_ST6B_BOSS_1E,   SPELLCARD_ST6B_BOSS_1N,   SPELLCARD_ST6B_BOSS_1H,   SPELLCARD_ST6B_BOSS_1L,
    SPELLCARD_ST6B_BOSS_2E,   SPELLCARD_ST6B_BOSS_2N,   SPELLCARD_ST6B_BOSS_2H,   SPELLCARD_ST6B_BOSS_2L,
    SPELLCARD_ST6B_BOSS_3E,   SPELLCARD_ST6B_BOSS_3N,   SPELLCARD_ST6B_BOSS_3H,   SPELLCARD_ST6B_BOSS_3L,
    SPELLCARD_ST6B_BOSS_4E,   SPELLCARD_ST6B_BOSS_4N,   SPELLCARD_ST6B_BOSS_4H,   SPELLCARD_ST6B_BOSS_4L,
    SPELLCARD_ST6B_BOSS_5E,   SPELLCARD_ST6B_BOSS_5N,   SPELLCARD_ST6B_BOSS_5H,   SPELLCARD_ST6B_BOSS_5L,
    SPELLCARD_ST6B_BOSS_LS1E, SPELLCARD_ST6B_BOSS_LS1N, SPELLCARD_ST6B_BOSS_LS1H, SPELLCARD_ST6B_BOSS_LS1L,
    SPELLCARD_ST6B_BOSS_LS2E, SPELLCARD_ST6B_BOSS_LS2N, SPELLCARD_ST6B_BOSS_LS2H, SPELLCARD_ST6B_BOSS_LS2L,
    SPELLCARD_ST6B_BOSS_LS3E, SPELLCARD_ST6B_BOSS_LS3N, SPELLCARD_ST6B_BOSS_LS3H, SPELLCARD_ST6B_BOSS_LS3L,
    SPELLCARD_ST6B_BOSS_LS4E, SPELLCARD_ST6B_BOSS_LS4N, SPELLCARD_ST6B_BOSS_LS4H, SPELLCARD_ST6B_BOSS_LS4L,
    SPELLCARD_ST6B_BOSS_LS5E, SPELLCARD_ST6B_BOSS_LS5N, SPELLCARD_ST6B_BOSS_LS5H, SPELLCARD_ST6B_BOSS_LS5L,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 14, g_SpellcardNumbersStageExtra) = {
    SPELLCARD_EX_MBOSS_1, SPELLCARD_EX_MBOSS_2, SPELLCARD_EX_MBOSS_3,
    SPELLCARD_EX_BOSS_1,  SPELLCARD_EX_BOSS_2,  SPELLCARD_EX_BOSS_3,  SPELLCARD_EX_BOSS_4,
    SPELLCARD_EX_BOSS_5,  SPELLCARD_EX_BOSS_6,  SPELLCARD_EX_BOSS_7,  SPELLCARD_EX_BOSS_8,
    SPELLCARD_EX_BOSS_9,  SPELLCARD_EX_BOSS_10, SPELLCARD_EX_BOSS_LS,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 17, g_SpellcardNumbersLastWord) = {
    SPELLCARD_LW_WRIGGLE,
    SPELLCARD_LW_MYSTIA,
    SPELLCARD_LW_KEINE,
    SPELLCARD_LW_REISEN,
    SPELLCARD_LW_EIRIN,
    SPELLCARD_LW_KAGUYA,
    SPELLCARD_LW_MOKOU,
    SPELLCARD_LW_TEWI,
    SPELLCARD_LW_KEINEEX,
    SPELLCARD_LW_REIMU,
    SPELLCARD_LW_MARISA,
    SPELLCARD_LW_SAKUYA,
    SPELLCARD_LW_YOUMU,
    SPELLCARD_LW_ALICE,
    SPELLCARD_LW_REMILIA,
    SPELLCARD_LW_YUYUKO,
    SPELLCARD_LW_YUKARI,
};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 43, g_LastSpellNumbers) = {
    // Wriggle Nightbug
    SPELLCARD_ST1_BOSS_LSN,   SPELLCARD_ST1_BOSS_LSH,   SPELLCARD_ST1_BOSS_LSL,
    // Mystia Lorelei
    SPELLCARD_ST2_BOSS_LSN,   SPELLCARD_ST2_BOSS_LSH,   SPELLCARD_ST2_BOSS_LSL,
    // Keine Kamishirasawa
    SPELLCARD_ST3_BOSS_LSN,   SPELLCARD_ST3_BOSS_LSH,   SPELLCARD_ST3_BOSS_LSL,
    // Reimu Hakurei
    SPELLCARD_ST4A_BOSS_LSN,  SPELLCARD_ST4A_BOSS_LSH,  SPELLCARD_ST4A_BOSS_LSL,
    // Marisa Kirisame
    SPELLCARD_ST4B_BOSS_LSN,  SPELLCARD_ST4B_BOSS_LSH,  SPELLCARD_ST4B_BOSS_LSL,
    // Reisen U. Inaba
    SPELLCARD_ST5_BOSS_LSN,   SPELLCARD_ST5_BOSS_LSH,   SPELLCARD_ST5_BOSS_LSL,
    // Eirin Yagokoro
    SPELLCARD_ST6A_BOSS_LSE,  SPELLCARD_ST6A_BOSS_LSN,  SPELLCARD_ST6A_BOSS_LSH,  SPELLCARD_ST6A_BOSS_LSL,

    // Kaguya Houraisan, Spell 1
    SPELLCARD_ST6B_BOSS_LS1E, SPELLCARD_ST6B_BOSS_LS1N, SPELLCARD_ST6B_BOSS_LS1H, SPELLCARD_ST6B_BOSS_LS1L,
    // Kaguya Houraisan, Spell 2
    SPELLCARD_ST6B_BOSS_LS2E, SPELLCARD_ST6B_BOSS_LS2N, SPELLCARD_ST6B_BOSS_LS2H, SPELLCARD_ST6B_BOSS_LS2L,
    // Kaguya Houraisan, Spell 3
    SPELLCARD_ST6B_BOSS_LS3E, SPELLCARD_ST6B_BOSS_LS3N, SPELLCARD_ST6B_BOSS_LS3H, SPELLCARD_ST6B_BOSS_LS3L,
    // Kaguya Houraisan, Spell 4
    SPELLCARD_ST6B_BOSS_LS4E, SPELLCARD_ST6B_BOSS_LS4N, SPELLCARD_ST6B_BOSS_LS4H, SPELLCARD_ST6B_BOSS_LS4L,
    // Kaguya Houraisan, Spell 5
    SPELLCARD_ST6B_BOSS_LS5E, SPELLCARD_ST6B_BOSS_LS5N, SPELLCARD_ST6B_BOSS_LS5H, SPELLCARD_ST6B_BOSS_LS5L,

    // Fujiwara no Mokou
    SPELLCARD_EX_BOSS_LS,
};

DIFFABLE_STATIC_ASSIGN(i32, g_LastSpellCount) = 43;

DIFFABLE_STATIC_ARRAY_ASSIGN(i32 *, 10, g_SpellcardNumbersPerStage) = {
    g_SpellcardNumbersStage1,     g_SpellcardNumbersStage2,  g_SpellcardNumbersStage3,  g_SpellcardNumbersStage4A,
    g_SpellcardNumbersStage4B,    g_SpellcardNumbersStage5,  g_SpellcardNumbersStage6A, g_SpellcardNumbersStage6B,
    g_SpellcardNumbersStageExtra, g_SpellcardNumbersLastWord};

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 10, g_SpellcardCountPerStage) = {
    ARRAY_SIZE_SIGNED(g_SpellcardNumbersStage1),  ARRAY_SIZE_SIGNED(g_SpellcardNumbersStage2),
    ARRAY_SIZE_SIGNED(g_SpellcardNumbersStage3),  ARRAY_SIZE_SIGNED(g_SpellcardNumbersStage4A),
    ARRAY_SIZE_SIGNED(g_SpellcardNumbersStage4B), ARRAY_SIZE_SIGNED(g_SpellcardNumbersStage5),
    ARRAY_SIZE_SIGNED(g_SpellcardNumbersStage6A), ARRAY_SIZE_SIGNED(g_SpellcardNumbersStage6B),
    ARRAY_SIZE_SIGNED(g_SpellcardNumbersExtra),   ARRAY_SIZE_SIGNED(g_SpellcardNumbersLastWord)};

// clang-format on
// TODO: stop clang-format from fucking with whitespace formatting

// FUNCTION: th08 0x4144d0
#pragma var_order(difficulty, i)
i32 Spellcard::GetDifficultyFromSpellCard(i32 spellCardNumber)
{
    i32 difficulty;
    i32 i;
    for (difficulty = 0; difficulty < MAX_DIFFICULTIES; difficulty++)
    {
        for (i = 0; i < g_SpellcardCountsPerDifficulty[difficulty]; i++)
        {
            if (g_SpellcardNumbersPerDifficulty[difficulty][i] == spellCardNumber)
            {
                return difficulty;
            }
        }
    }
    return MAX_DIFFICULTIES;
}

} /* namespace th08 */
