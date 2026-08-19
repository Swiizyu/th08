#include "th_pch.h"

#include "GameManager.hpp"
#include "AsciiManager.hpp"
#include "Background.hpp"
#include "BulletManager.hpp"
#include "EffectManager.hpp"
#include "EnemyManager.hpp"
#include "Global.hpp"
#include "Gui.hpp"
#include "ItemManager.hpp"
#include "Player.hpp"
#include "ReplayManager.hpp"
#include "ScreenEffect.hpp"
#include "SoundPlayer.hpp"
#include "SpellCard.hpp"

void FUN_004531a0(void);

struct RankParams
{
    i32 rank;
    i32 minRank;
    i32 maxRank;
};

DIFFABLE_STATIC_ARRAY_ASSIGN(RankParams, 6, g_RankParams) = {
    {10, 8, 16}, {10, 8, 16}, {8, 8, 12}, {8, 8, 12}, {16, 15, 16}, {16, 15, 16},
};

namespace th08
{

// FUNCTION: th08 0x43be09
void __fastcall IncrementTruncate(u32 *value, i32)
{
    if (*value < 999999)
    {
        (*value)++;
    }
}

// FUNCTION: th08 0x43b936
void GameManager::InitRankParams()
{
    this->rank = ::g_RankParams[g_GameManager.difficulty].rank;
    this->minRank = ::g_RankParams[g_GameManager.difficulty].minRank;
    this->maxRank = ::g_RankParams[g_GameManager.difficulty].maxRank;
}

DIFFABLE_STATIC(GameManager, g_GameManager);
DIFFABLE_STATIC(i32, g_GameManagerUnknown4e3d28);
DIFFABLE_STATIC(ChainElem, g_GameManagerCalcChain);
DIFFABLE_STATIC(ChainElem, g_GameManagerDrawChain);

// FUNCTION: th08 0x43bbe1
int FUN_0043bbe1()
{
    memset(g_GameManager.catkData, 0, sizeof(g_GameManager.catkData));
    memset(g_GameManager.catkData2, 0, sizeof(g_GameManager.catkData2));
    for (i32 i = 0; i < SPELLCARD_COUNT_SPELLCARDS; i++)
    {
        Catk *catk = &g_GameManager.catkData[i];
        catk->base.magic = CATK_MAGIC;
        catk->base.unkLen = sizeof(Catk);
        catk->base.th8kLen = sizeof(Catk);
        catk->base.version = CATK_VERSION;
        catk->spellcardNumber = (u16)i;
        Catk *copy = &g_GameManager.catkData2[i];
        *copy = *catk;
    }
    return 0;
}

struct SpellcardMusicEntry
{
    i32 spellcardNumber;
    i32 songNumber;
    const char *songPath;
    i32 songNameSpriteIdx;
    ZunBool musicPausesInSpellPractice;
};

SpellcardMusicEntry g_SpellcardMusicInfo[] = {{1, 1, "th08_00.mid", 0, FALSE},
                                              {12, 2, "th08_03.mid", 1, FALSE},
                                              {16, 3, "th08_04.mid", 0, FALSE},
                                              {31, 4, "th08_05.mid", 1, FALSE},
                                              {35, 5, "th08_06.mid", 0, FALSE},
                                              {53, 6, "th08_07.mid", 1, FALSE},
                                              {76, 8, "th08_09.mid", 1, FALSE},
                                              {99, 9, "th08_10.mid", 1, FALSE},
                                              {118, 11, "th08_12.mid", 1, FALSE},
                                              {122, 12, "th08_13.mid", 0, FALSE},
                                              {142, 13, "th08_14.mid", 1, FALSE},
                                              {146, 15, "th08_13b.mid", 2, TRUE},
                                              {150, 12, "th08_13.mid", 0, FALSE},
                                              {170, 14, "th08_15.mid", 1, FALSE},
                                              {190, 15, "th08_13b.mid", 2, TRUE},
                                              {193, 16, "th08_18.mid", 0, FALSE},
                                              {204, 17, "th08_19.mid", 1, FALSE},
                                              {222, 20, "th08_20.mid", 2, FALSE},
                                              {-1, 0, " ", 0, FALSE}};

// FUNCTION: th08 0x439916
ZunBool GameManager::ShouldPauseMusicInSpellPractice(i32 spellcardNumber)
{
    i32 i = 0;

    while (g_SpellcardMusicInfo[i].spellcardNumber >= 0)
    {
        if (g_GameManager.currentSpellCardNumber <= g_SpellcardMusicInfo[i].spellcardNumber)
        {
            return g_SpellcardMusicInfo[i].musicPausesInSpellPractice;
        }

        i++;
    }

    return FALSE;
}

// FUNCTION: th08 0x439961
i32 GameManager::GetSongNameSpriteIdx(i32 spellcardNumber)
{
    i32 i = 0;

    while (g_SpellcardMusicInfo[i].spellcardNumber >= 0)
    {
        if (g_GameManager.currentSpellCardNumber <= g_SpellcardMusicInfo[i].spellcardNumber)
        {
            return g_SpellcardMusicInfo[i].songNameSpriteIdx;
        }

        i++;
    }

    return FALSE;
}

// FUNCTION: th08 0x44e160
void GameManager::RandomizeAntiTamper()
{
    this->globals->rng1[0] = g_Rng.GetRandomU32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    this->globals->rng1[1] = g_Rng.GetRandomU32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    this->globals->rng1[2] = g_Rng.GetRandomU32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    this->globals->rng1[3] = g_Rng.GetRandomU32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    this->globals->rng1[4] = g_Rng.GetRandomU32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    this->globals->rng4[0] = g_Rng.GetRandomF32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    this->globals->rng4[1] = g_Rng.GetRandomF32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    this->globals->rng4[2] = g_Rng.GetRandomF32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
}

// FUNCTION: th08 0x418220
void GameManager::AddTimeOrbs(i32 amount)
{
    if (amount < 0 && this->globals->currentTimeOrbs < -amount)
    {
        goto setToZero;
    }

    this->globals->currentTimeOrbs = this->globals->currentTimeOrbs + amount;
    this->globals->totalTimeOrbs = this->globals->totalTimeOrbs + amount;
    *(i32 *)((u8 *)this + 0x3daac) = *(i32 *)((u8 *)this + 0x3daac) + amount;
    this->UpdateAntiTamper();
    if (amount > 0)
    {
        amount += this->globals->totalTimeOrbs & 1;
        this->globals->pointItemValue = this->globals->pointItemValue + amount / 2 * 10;
    }
    goto end;

setToZero:
    this->globals->currentTimeOrbs = 0;
end:
    return;
}

// FUNCTION: th08 0x4399ac
ZunBool GameManager::IsWithinPlayfield(f32 x, f32 y, f32 width, f32 height)
{
    if (x + width / 2.0f < 0.0f)
    {
        return FALSE;
    }
    if (x - width / 2.0f > 384.0f)
    {
        return FALSE;
    }
    if (y + height / 2.0f < 0.0f)
    {
        return FALSE;
    }
    if (y - height / 2.0f > 448.0f)
    {
        return FALSE;
    }
    return TRUE;
}

i32 GameManager::CalcAntiTamperChecksum()
{
    i32 sum;

    // There is zero chance ZUN actually used intptr_t here, but the codegen matches
    // and not making assumptions about pointer size is always nice
    sum = CalcChecksum((u8 *)&g_GameManager.globals->rng1,
                       (intptr_t)&globals->antiTamperValue - (intptr_t)&globals->rng1);
    sum += CalcChecksum((u8 *)&g_GameManager.globals->rng8, sizeof(g_GameManager.globals->rng8));
    sum += CalcChecksum((u8 *)g_GameManager.cfg, sizeof(GameConfiguration));
    sum += CalcChecksum((u8 *)&g_Supervisor.cfg, sizeof(GameConfiguration));
    sum += CalcChecksum((u8 *)&this->hscr, sizeof(Hscr));

    return sum;
}

i32 GameManager::CalcChecksum(u8 *address, i32 size)
{
    i32 sum;
    i32 i;

    for (sum = 0, i = 0; i < size; i++, address++)
    {
        sum += *address;
        g_GameManager.globals->antiTamperValue += g_GameManager.globals->rng8[2];
    }

    return sum;
}

void GameManager::CollectExtend()
{
    if (this->GetLives() < 8)
    {
        this->AddLives(1);
        g_SoundPlayer.PlaySoundByIdx(SOUND_1UP, 0);
        this->IncreaseSubrank(200);
        g_Gui.flags.lifeDisplayUpdateFrames = 2;
    }
    else if (this->GetBombsRemaining() < 8)
    {
        this->AddToBombCount(1);
        g_SoundPlayer.PlaySoundByIdx(SOUND_1UP, 0);
        this->IncreaseSubrank(200);
        g_Gui.flags.bombDisplayUpdateFrames = 2;
    }
}

// FUNCTION: th08 0x439bc7
ChainCallbackResult GameManager::OnUpdate(GameManager *gameManager)
{
    if (g_Supervisor.curState != SupervisorState_GameManager) return CHAIN_CALLBACK_RESULT_BREAK;
    if (gameManager->unk38 != 0 || gameManager->globals == NULL) return CHAIN_CALLBACK_RESULT_CONTINUE;
    if (gameManager->globals->displayScore < gameManager->globals->score)
    {
        u32 difference = gameManager->globals->score - gameManager->globals->displayScore;
        gameManager->globals->displayScore += difference < 100 ? 1 : difference / 10;
    }
    else if (gameManager->globals->displayScore > gameManager->globals->score)
    {
        gameManager->globals->displayScore = gameManager->globals->score;
    }
    gameManager->globals->youkaiGaugeCopy = gameManager->globals->youkaiGauge;
    if (gameManager->flags.isDemoMode) gameManager->demoFrameCount++;
    if (gameManager->showPauseMenu == 2) gameManager->showPauseMenu = 0;
    if (gameManager->globals->pointItemValue > 999990) gameManager->globals->pointItemValue = 999990;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x43aa03
ChainCallbackResult GameManager::OnDraw(GameManager *gameManager)
{
    if (gameManager->showPauseMenu)
    {
        gameManager->showPauseMenu = 2;
    }
    if (g_Supervisor.curState != SupervisorState_GameManager)
    {
        return CHAIN_CALLBACK_RESULT_BREAK;
    }
    if (gameManager->flags.unk5 == 1)
    {
        return CHAIN_CALLBACK_RESULT_BREAK;
    }
    if (gameManager->unk38 != 0)
    {
        return CHAIN_CALLBACK_RESULT_BREAK;
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x43aa5c
ZunResult GameManager::RegisterChain()
{
    GameManager *gameManager;

    gameManager = &g_GameManager;
    g_GameManagerCalcChain.callback = (ChainCallback)GameManager::OnUpdate;
    g_GameManagerCalcChain.addedCallback = NULL;
    g_GameManagerCalcChain.deletedCallback = NULL;
    g_GameManagerCalcChain.addedCallback = (ChainLifetimeCallback)GameManager::AddedCallback;
    g_GameManagerCalcChain.deletedCallback = (ChainLifetimeCallback)GameManager::DeletedCallback;
    g_GameManagerCalcChain.arg = gameManager;
    gameManager->unk3ddc0 = 0;
    if (g_Chain.AddToCalcChain(&g_GameManagerCalcChain, 2) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    g_GameManagerDrawChain.callback = (ChainCallback)GameManager::OnDraw;
    g_GameManagerDrawChain.addedCallback = NULL;
    g_GameManagerDrawChain.deletedCallback = NULL;
    g_GameManagerDrawChain.arg = gameManager;
    g_Chain.AddToDrawChain(&g_GameManagerDrawChain, 5);
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x43aaf4
ZunResult GameManager::AddedCallback(GameManager *gameManager)
{
    if (g_Supervisor.curState != SupervisorState_GameManagerReInit &&
        g_Supervisor.curState != SupervisorState_SpellcardPracticeRestart &&
        g_Supervisor.curState != SupervisorState_GameManagerNextStageWeird)
    {
        g_Supervisor.isInitialStageLoad = TRUE;
    }
    else
    {
        g_Supervisor.isInitialStageLoad = FALSE;
    }
    g_GameManager.unk38 = 1;
    if (g_Supervisor.wantedState2 == 1)
    {
        Float3 position;
        position.x = 500.0f;
        position.y = 440.0f;
        position.z = 0.0f;
        g_Supervisor.SetupLoadingVmsAndInitCapture(&position);
        g_Supervisor.StartEffect(0);
    }
    else
    {
        Float3 position;
        position.x = 280.0f;
        position.y = 430.0f;
        position.z = 0.0f;
        g_Supervisor.SetupLoadingVmsAndInitCapture(&position);
    }
    if (gameManager->flags.unk5 >= 2)
    {
        gameManager->flags.unk5 = 1;
    }
    g_Supervisor.ThreadStart((LPTHREAD_START_ROUTINE)GameManager::GameplaySetupThread, NULL);
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x43abd7
void GameManager::GameplaySetupThread()
{
    if (g_GameManager.cfg == NULL)
    {
        g_GameManager.cfg = new GameConfiguration;
        *g_GameManager.cfg = g_Supervisor.cfg;
    }
    if (g_GameManager.globals == NULL)
    {
        g_GameManager.globals = new ZunGlobals;
        memset(g_GameManager.globals, 0, sizeof(ZunGlobals));
        g_GameManager.globals->livesRemaining = g_GameManager.cfg->lifeCount;
        g_GameManager.globals->bombsRemaining = g_GameManager.cfg->bombCount;
    }
    FUN_0043bbe1();
    g_GameManager.InitializeAntiTamper();
    g_GameManager.InitRankParams();
    g_GameManager.InitArcadeRegionParams();
    ItemManager::UpdatePointItemExtendThreshold();
    Player::RegisterChain(g_GameManager.character);
    EffectManager::RegisterChain();
    Background::RegisterChain(g_GameManager.currentStage);
    BulletManager::RegisterChain("bullet.anm");
    EnemyManager::RegisterChain();
    Spellcard::RegisterChain();
    Gui::RegisterChain();
    g_GameManager.unk38 = 0;
}

#pragma var_order(sum, i)
void GameManager::InitializeAntiTamper()
{
    i32 sum;
    u32 i;

    g_GameManager.globals->rng6 = g_Rng.GetRandomU32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    for (i = 0; i < ARRAY_SIZE(g_GameManager.globals->rng1); i++)
    {
        g_GameManager.globals->rng1[i] = g_Rng.GetRandomU32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    }
    for (i = 0; i < ARRAY_SIZE(g_GameManager.globals->rng7); i++)
    {
        g_GameManager.globals->rng7[i] = g_Rng.GetRandomU32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    }
    for (i = 0; i < ARRAY_SIZE(g_GameManager.globals->rng2); i++)
    {
        g_GameManager.globals->rng2[i] = g_Rng.GetRandomF32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    }
    for (i = 0; i < ARRAY_SIZE(g_GameManager.globals->rng3); i++)
    {
        g_GameManager.globals->rng3[i] = g_Rng.GetRandomF32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    }
    for (i = 0; i < ARRAY_SIZE(g_GameManager.globals->rng4); i++)
    {
        g_GameManager.globals->rng4[i] = g_Rng.GetRandomF32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    }
    for (i = 0; i < ARRAY_SIZE(g_GameManager.globals->rng5); i++)
    {
        g_GameManager.globals->rng5[i] = g_Rng.GetRandomF32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    }
    for (i = 0; i < ARRAY_SIZE(g_GameManager.globals->rng8); i++)
    {
        g_GameManager.globals->rng8[i] = g_Rng.GetRandomU32InRange(ANTITAMPER_RNG_RANGE) + ANTITAMPER_RNG_ADD;
    }
    g_GameManager.globals->antiTamperValue = g_GameManager.globals->rng1[2];
    sum = g_GameManager.CalcAntiTamperChecksum();
    g_GameManager.globals->antiTamperChecksum = sum;
    g_GameManager.antiTamperExpectedValue = (f32)sum + (f32)g_GameManager.globals->rng7[3];
}

// FUNCTION: th08 0x43be2c
ZunResult GameManager::DeletedCallback(GameManager *gameManager)
{
    MidiOutput *midiOutput;

    g_ScreenEffectCounter = 1;
    g_GameManagerUnknown4e3d28 = 0;
    if (g_Supervisor.curState != SupervisorState_GameManagerReInit &&
        g_Supervisor.curState != SupervisorState_SpellcardPracticeRestart &&
        g_Supervisor.curState != SupervisorState_GameManagerNextStageWeird)
    {
        g_Supervisor.keepStageResources = TRUE;
    }
    else
    {
        g_Supervisor.keepStageResources = FALSE;
    }

    if (!g_GameManager.flags.isSpellPractice || g_Supervisor.keepStageResources)
    {
        g_Supervisor.StopAudio();
        if (g_Supervisor.cfg.musicMode == 2 && g_Supervisor.midiOutput != NULL)
        {
            midiOutput = g_Supervisor.midiOutput;
            midiOutput->StopPlayback();
            midiOutput->ParseFile(30);
            midiOutput->Play();
        }
    }
    while (g_SoundPlayer.ProcessQueues() != 0)
    {
    }

    Spellcard::CutChain();
    Background::CutChain();
    BulletManager::CutChain();
    Player::CutChain();
    EnemyManager::CutChain();
    EffectManager::CutChain();
    Gui::CutChain();
    if (!g_GameManager.flags.isReplay)
    {
        ::FUN_004531a0();
    }
    if (!g_GameManager.flags.isReplay)
    {
        g_Supervisor.UpdateGameTime();
    }
    *(i32 *)((u8 *)&g_Supervisor + 0x1ac) = 0;
    g_Supervisor.UpdatePlayTime();
    gameManager->flags.unk2 = 0;
    g_AsciiManager.Reset();
    *(u8 *)((u8 *)&g_GameManager + 0x2d) = 0;
    g_GameManager.unk3ddc0 = 0;
    return ZUN_SUCCESS;
}

void GameManager::IncreaseSubrank(int amount)
{
    this->subRank += amount;
    while (this->subRank >= 100)
    {
        this->rank++;
        this->subRank -= 100;
    }
    if (this->rank > this->maxRank)
    {
        this->rank = this->maxRank;
    }
}

void GameManager::DecreaseSubrank(int amount)
{
    this->subRank -= amount;
    while (this->subRank < 0)
    {
        this->rank--;
        this->subRank += 100;
    }
    if (this->rank < this->minRank)
    {
        this->rank = this->minRank;
    }
}

// FUNCTION: th08 0x43c0bb
void GameManager::AddToYoukaiGauge(i32 amount, i32 force)
{
    if (*(i32 *)((u8 *)&g_Player + 0xfdc) != 0 && force == 0)
    {
        return;
    }

    this->globals->youkaiGauge += amount;
    if (this->globals->youkaiGauge < this->youkaiGaugeHumanLimit)
    {
        this->globals->youkaiGauge = this->youkaiGaugeHumanLimit;
    }
    else if (this->globals->youkaiGauge > this->youkaiGaugeYoukaiLimit)
    {
        this->globals->youkaiGauge = this->youkaiGaugeYoukaiLimit;
    }
    this->globals->youkaiGaugeCopy = this->globals->youkaiGauge;
}

ZunBool GameManager::IsExtraUnlockedForCharacter(i32 character)
{
    return (character > SHOT_YOUMU_YUYUKO) ||
           (this->clrdData[character].difficultiesClearedWithoutRetries[EASY] & EXTRA_UNLOCKED_FLAG ||
            this->clrdData[character].difficultiesClearedWithoutRetries[NORMAL] & EXTRA_UNLOCKED_FLAG ||
            this->clrdData[character].difficultiesClearedWithoutRetries[HARD] & EXTRA_UNLOCKED_FLAG ||
            this->clrdData[character].difficultiesClearedWithoutRetries[LUNATIC] & EXTRA_UNLOCKED_FLAG);
}

ZunBool GameManager::IsExtraUnlocked()
{
    return this->IsExtraUnlockedForCharacter(SHOT_REIMU_YUKARI) ||
           this->IsExtraUnlockedForCharacter(SHOT_MARISA_ALICE) ||
           this->IsExtraUnlockedForCharacter(SHOT_SAKUYA_REMILIA) ||
           this->IsExtraUnlockedForCharacter(SHOT_YOUMU_YUYUKO);
}

ZunBool GameManager::IsSpellPracticeUnlockedForCharacter(i32 character)
{
    return (character > SHOT_YOUMU_YUYUKO) ||
           (this->clrdData[character].difficultiesClearedWithRetries[EASY] & SPELL_PRACTICE_UNLOCKED_FLAG ||
            this->clrdData[character].difficultiesClearedWithRetries[NORMAL] & SPELL_PRACTICE_UNLOCKED_FLAG ||
            this->clrdData[character].difficultiesClearedWithRetries[HARD] & SPELL_PRACTICE_UNLOCKED_FLAG ||
            this->clrdData[character].difficultiesClearedWithRetries[LUNATIC] & SPELL_PRACTICE_UNLOCKED_FLAG);
}

ZunBool GameManager::IsSpellPracticeUnlocked()
{
    return this->IsSpellPracticeUnlockedForCharacter(SHOT_REIMU_YUKARI) ||
           this->IsSpellPracticeUnlockedForCharacter(SHOT_MARISA_ALICE) ||
           this->IsSpellPracticeUnlockedForCharacter(SHOT_SAKUYA_REMILIA) ||
           this->IsSpellPracticeUnlockedForCharacter(SHOT_YOUMU_YUYUKO);
}

// Leftover from PCB.
ZunBool GameManager::IsPhantasmUnlocked()
{
    return FALSE;
}

// FUNCTION: th08 0x43c322
ZunBool GameManager::IsReplayPractice()
{
    ZunBool result;

    if (this->flags.isReplay && g_ReplayManager->replayData->isPractice)
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    return result;
}

void GameManager::CutChain()
{
    g_Chain.Cut(&g_GameManagerCalcChain);
    g_Chain.Cut(&g_GameManagerDrawChain);
    if (g_GameManager.globals->score >= 1000000000)
    {
        g_GameManager.globals->score = 999999999;
    }
    g_GameManager.globals->displayScore = g_GameManager.globals->score;
    g_Supervisor.framerateMultiplier = 1.0f;
}

i32 GameManager::GetClockTimeIncrement()
{
    // ZUN bloat: Why not use switch case fallthrough?
    switch (g_GameManager.currentStage)
    {
    case STAGE1:
        if (g_GameManager.GetTimeOrbs() >= g_GameManager.GetLastSpellTimeOrbThreshold())
        {
            return 1;
        }
        else
        {
            return 2;
        }
    case STAGE2:
        if (g_GameManager.GetTimeOrbs() >= g_GameManager.GetLastSpellTimeOrbThreshold())
        {
            return 1;
        }
        else
        {
            return 2;
        }
    case STAGE3:
        if (g_GameManager.GetTimeOrbs() >= g_GameManager.GetLastSpellTimeOrbThreshold())
        {
            return 1;
        }
        else
        {
            return 2;
        }
    case STAGE4A:
        if (g_GameManager.GetTimeOrbs() >= g_GameManager.GetLastSpellTimeOrbThreshold())
        {
            return 1;
        }
        else
        {
            return 2;
        }
    case STAGE4B:
        if (g_GameManager.GetTimeOrbs() >= g_GameManager.GetLastSpellTimeOrbThreshold())
        {
            return 1;
        }
        else
        {
            return 2;
        }
    case STAGE5:
        if (g_GameManager.GetTimeOrbs() >= g_GameManager.GetLastSpellTimeOrbThreshold())
        {
            return 1;
        }
        else
        {
            return 2;
        }
    case STAGE6A:
        return 0;
    case STAGE6B:
        return 0;
    default:
        return 4;
    }
}

void GameManager::AdvanceToNextStage()
{
    switch (this->currentStage)
    {
    case STAGE1:
        this->currentStage = STAGE2;
        break;
    case STAGE2:
        this->currentStage = STAGE3;
        break;
    case STAGE3:
        switch (g_GameManager.shotType)
        {
        case SHOT_REIMU_YUKARI:
        case SHOT_REIMU:
        case SHOT_YUKARI:
            this->currentStage = STAGE4B;
            break;
        case SHOT_MARISA_ALICE:
        case SHOT_MARISA:
        case SHOT_ALICE:
            this->currentStage = STAGE4A;
            break;
        case SHOT_SAKUYA_REMILIA:
        case SHOT_SAKUYA:
        case SHOT_REMILIA:
            this->currentStage = STAGE4A;
            break;
        case SHOT_YOUMU_YUYUKO:
        case SHOT_YOUMU:
        case SHOT_YUYUKO:
            this->currentStage = STAGE4B;
            break;
        }
        break;
    case STAGE4A:
    case STAGE4B:
        this->currentStage = STAGE5;
        break;
    case STAGE5:
        this->currentStage = g_GameManager.flags.isGoingToFinalB ? STAGE6B : STAGE6A;
        break;
    case STAGE6A:
        this->currentStage = STAGE6B; // Was Kaguya meant to be a TLB at one point???
        break;
    }
}

GameManager::GameManager()
{
    memset(this, 0, sizeof(GameManager));
    this->arcadeRegionTopLeftPos.x = 32.0f;
    this->arcadeRegionTopLeftPos.y = 16.0f;
    this->arcadeRegionSize.x = 384.0f;
    this->arcadeRegionSize.y = 448.0f;
    this->currentDemoReplay = 3;
}

void GameManager::InitArcadeRegionParams()
{
    this->arcadeRegionTopLeftPos.x = 32.0f;
    this->arcadeRegionTopLeftPos.y = 16.0f;
    this->arcadeRegionSize.x = 384.0f;
    this->arcadeRegionSize.y = 448.0f;
    this->playerMovementTopLeftPos.x = 8.0f;
    this->playerMovementTopLeftPos.y = 16.0f;
    this->playerMovementAreaSize.x = 368.0f;
    this->playerMovementAreaSize.y = 416.0f;
}

}; // Namespace th08
