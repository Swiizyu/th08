#include "th_pch.h"

#include "Global.hpp"
#include "Config.hpp"
#include "EnemyManager.hpp"
#include "Gui.hpp"
#include "ReplayManager.hpp"
#include "ResultScreen.hpp"
#include "Supervisor.hpp"

#include "pbg/Lzss.hpp"

#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

namespace th08
{

DIFFABLE_STATIC(ReplayManager *, g_ReplayManager);

static const char *g_ReplayDifficultyNames[6] = {"Easy", "Normal", "Hard", "Lunatic", "Extra", "LastWord"};

} // namespace th08

extern "C" int __cdecl vsprintf(char *buffer, const char *format, va_list args);

i32 FUN_00453cc0(th08::ReplayManager *mgr);
i32 FUN_00437dc7(th08::Gui *gui);
void FUN_004531a0(void);
th08::ChainCallbackResult FUN_00452490(th08::ReplayManager *mgr);
th08::ChainCallbackResult FUN_004526c0(th08::ReplayManager *mgr);

// FUNCTION: th08 0x453b80
char *sprintf(char *dst, const char *fmt, ...)
{
    va_list args;
    char *cur;
    char *next;
    u8 byte;
    i32 len;

    va_start(args, fmt);
    vsprintf(dst, fmt, args);
    va_end(args);

    cur = dst;
    next = cur + 1;
    do
    {
        byte = *cur;
        cur = cur + 1;
    } while (byte);
    len = cur - next;
    return dst + len;
}

// FUNCTION: th08 0x42f1f0
i32 FUN_0042f1f0(th08::EnemyManager *enemyManager)
{
    i32 i;

    for (i = 0; i < 8; i++)
    {
        if (*(i32 *)((u8 *)enemyManager + i * 4 + 0x9dcda0) != 0)
        {
            return 1;
        }
    }
    return 0;
}

namespace th08
{

// FUNCTION: th08 0x453160
ReplayManager::ReplayManager()
{
}

// FUNCTION: th08 0x451f90
ZunResult ReplayManager::RegisterChain(i32 isDemo, char *replayFile)
{
    ReplayManager *replayMgr;

    g_ReplayLastFrameInput = 0;
    g_ReplayCurFrameInput = 0;

    if (g_ReplayManager == NULL)
    {
        replayMgr = ZUN_NEW(ReplayManager, "ReplayInf");
        g_ReplayManager = replayMgr;
        memset(replayMgr, 0, sizeof(ReplayManager));
        replayMgr->replayData = NULL;
        replayMgr->isDemo = isDemo;
        replayMgr->replayFile = replayFile;

        switch (isDemo)
        {
        case 0:
            replayMgr->calcChain = g_Chain.CreateElem((ChainCallback)ReplayManager::OnUpdateHighPrio);
            replayMgr->calcChain->addedCallback = (ChainLifetimeCallback)ReplayManager::AddedCallback;
            replayMgr->calcChain->deletedCallback = (ChainLifetimeCallback)ReplayManager::DeletedCallback;
            replayMgr->calcChain->arg = replayMgr;
            if (g_Chain.AddToCalcChain(replayMgr->calcChain, 17) != 0)
            {
                return ZUN_ERROR;
            }
            replayMgr->altCalcChain = NULL;
            replayMgr->lowPrioCalcChain = g_Chain.CreateElem((ChainCallback)ReplayManager::OnUpdateLowPrio);
            replayMgr->lowPrioCalcChain->arg = replayMgr;
            g_Chain.AddToCalcChain(replayMgr->lowPrioCalcChain, 7);
            ReplayManager::OnUpdateLowPrio(replayMgr);
            break;
        case 1:
            replayMgr->calcChain = g_Chain.CreateElem((ChainCallback)ReplayManager::OnUpdateHighPrioDemo);
            replayMgr->calcChain->addedCallback = (ChainLifetimeCallback)ReplayManager::AddedCallbackDemo;
            replayMgr->calcChain->deletedCallback = (ChainLifetimeCallback)ReplayManager::DeletedCallback;
            replayMgr->calcChain->arg = replayMgr;
            if (g_Chain.AddToCalcChain(replayMgr->calcChain, 6) != 0)
            {
                return ZUN_ERROR;
            }
            if (replayMgr->replayData->header.unk0x6 != 0)
            {
                replayMgr->calcChain->callback = (ChainCallback)::FUN_004526c0;
            }
            replayMgr->altCalcChain = g_Chain.CreateElem((ChainCallback)::FUN_00452490);
            replayMgr->altCalcChain->arg = replayMgr;
            g_Chain.AddToCalcChain(replayMgr->altCalcChain, 18);
            replayMgr->lowPrioCalcChain = NULL;
            if (replayMgr->replayData->header.unk0x6 != 0)
            {
                replayMgr->lowPrioCalcChain = g_Chain.CreateElem((ChainCallback)ReplayManager::OnUpdateLowPrio);
                replayMgr->lowPrioCalcChain->arg = replayMgr;
                g_Chain.AddToCalcChain(replayMgr->lowPrioCalcChain, 7);
                ReplayManager::OnUpdateLowPrio(replayMgr);
            }
            break;
        }
    }
    else
    {
        switch (isDemo)
        {
        case 0:
            ReplayManager::AddedCallback(g_ReplayManager);
            break;
        case 1:
            ReplayManager::AddedCallbackDemo(g_ReplayManager);
            break;
        }
    }

    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x4522a0
ChainCallbackResult ReplayManager::OnUpdateLowPrio(ReplayManager *mgr)
{
    mgr->inputFlags = 0;
    mgr->rngSeed = g_Rng.GetSeed();
    g_Rng.ResetGenerationCount();
    if (*(u32 *)((u8 *)&g_GameManager + 0x3db98) != 0)
    {
        mgr->inputFlags |= 0x100;
    }
    *(u32 *)((u8 *)&g_GameManager + 0x3db98) = 0;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x452310
ChainCallbackResult ReplayManager::OnUpdateHighPrio(ReplayManager *mgr)
{
    i32 stage;

    if (g_GameManager.flags.unk2 == 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }

    g_ReplayLastFrameInput = g_ReplayCurFrameInput;
    g_ReplayCurFrameInput = g_CurFrameInput;

    if (g_GameManager.cfg->slowMode != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (g_Supervisor.IsClearBackBufferOnRefreshEnabled())
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }

    if (g_GameManager.flags.unk9 != 0)
    {
        if (mgr->unk4 >= 3)
        {
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        mgr->unk4++;
    }

    stage = g_GameManager.currentStage2;
    u16 input = g_CurFrameInput;
    g_ReplayCurFrameInput = input;

    mgr->recordingCursor += 2;
    mgr->recordingStageBookmarks[stage] = mgr->recordingCursor + 2;
    *(u16 *)mgr->recordingCursor = input;

    if (mgr->frameId % 30 == 0)
    {
        *mgr->fpsCursor = *(u8 *)&g_Supervisor.curFps | (g_Supervisor.unk0x338 != 0 ? 0x80 : 0);
        mgr->fpsCursor[1] = *(u8 *)&g_Supervisor.unk198;
        mgr->fpsStageBookmarks[stage] = mgr->fpsCursor + 2;
        mgr->fpsCursor += 1;
    }

    mgr->frameId++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x452550
ChainCallbackResult ReplayManager::OnUpdateHighPrioDemo(ReplayManager *mgr)
{
    i32 unused;

    if (g_GameManager.flags.unk2 == 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (g_GameManager.cfg->slowMode != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (g_GameManager.flags.unk9 != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }

    unused = 0;

    g_ReplayLastFrameInput = g_ReplayCurFrameInput;
    g_ReplayCurFrameInput = *(u16 *)mgr->recordingCursor;
    mgr->recordingCursor += 2;

    g_IsEighthFrameOfHeldInput = 0;
    if (g_ReplayLastFrameInput == g_ReplayCurFrameInput)
    {
        if (g_NumOfFramesInputsWereHeld >= 30)
        {
            if (g_NumOfFramesInputsWereHeld % 8 == 0)
            {
                g_IsEighthFrameOfHeldInput = 1;
            }
            if (g_NumOfFramesInputsWereHeld >= 38)
            {
                g_NumOfFramesInputsWereHeld = 30;
            }
        }
        g_NumOfFramesInputsWereHeld++;
    }
    else
    {
        g_NumOfFramesInputsWereHeld = 0;
    }

    if (mgr->frameId % 30 == 0)
    {
        *(u16 *)&g_Supervisor.unk198 = ((i8)mgr->fpsCursor[1]) & 0x7f;
        g_Supervisor.unk0x33c = ((i8)mgr->fpsCursor[1]) >> 7;
        mgr->fpsCursor += 1;
    }

    mgr->frameId++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x452830
ZunResult ReplayManager::AddedCallback(ReplayManager *mgr)
{
    i32 i;
    StageReplayData *lastBookmark;
    u8 *fpsReplayData;
    StageReplayData *stageReplay;

    mgr->frameId = 0;
    mgr->unk0xc = NULL;

    if (mgr->replayData == NULL)
    {
        i32 clearState;
        i32 spellcardNumber;
        i32 stage6Cleared;

        mgr->replayData = ZUN_NEW(ReplayData, "ReplayDataInf");
        memset(mgr->replayData, 0, sizeof(ReplayData));
        mgr->replayData->header.magic = *(u32 *)REPLAY_MAGIC;
        mgr->replayData->header.unk0x7 = 0;
        mgr->replayData->shotType = g_GameManager.shotType;
        mgr->replayData->header.version = REPLAY_VERSION;
        mgr->replayData->header.unk0x6 = 0;
        mgr->replayData->majorVersion = 0x100;
        mgr->replayData->minorVersion = 100;
        memcpy(mgr->replayData->exeVersion, CONFIG_VERSION_STRING, 6);
        mgr->replayData->exeSize = g_Supervisor.exeSize;
        mgr->replayData->exeChecksum = g_Supervisor.exeChecksum;
        mgr->replayData->isPractice = g_GameManager.IsPracticeMode();

        if (g_GameManager.flags.isSpellPractice != 0)
        {
            spellcardNumber = g_GameManager.currentSpellCardNumber;
        }
        else
        {
            spellcardNumber = -1;
        }
        mgr->replayData->spellcardNumber = spellcardNumber;

        if (g_GameManager.IsStageClearedWithoutRetries(7, g_GameManager.shotType, 0))
        {
            goto clearStateTwo;
        }
        if (g_GameManager.IsStageClearedWithoutRetries(7, g_GameManager.shotType, 1))
        {
            goto clearStateTwo;
        }
        if (g_GameManager.IsStageClearedWithoutRetries(7, g_GameManager.shotType, 2))
        {
            goto clearStateTwo;
        }
        if (g_GameManager.IsStageClearedWithoutRetries(7, g_GameManager.shotType, 3))
        {
            goto clearStateTwo;
        }
        if (g_GameManager.shotType > 3)
        {
            goto clearStateTwo;
        }

        if (g_GameManager.IsStageClearedWithoutRetries(6, g_GameManager.shotType, 0))
        {
            goto stageSixCleared;
        }
        if (g_GameManager.IsStageClearedWithoutRetries(6, g_GameManager.shotType, 1))
        {
            goto stageSixCleared;
        }
        if (g_GameManager.IsStageClearedWithoutRetries(6, g_GameManager.shotType, 2))
        {
            goto stageSixCleared;
        }
        if (g_GameManager.IsStageClearedWithoutRetries(6, g_GameManager.shotType, 3))
        {
            goto stageSixCleared;
        }
        stage6Cleared = 0;
        goto setClearState;

    stageSixCleared:
        stage6Cleared = 1;
    setClearState:
        clearState = stage6Cleared;
        goto clearStateDone;
    clearStateTwo:
        clearState = 2;
    clearStateDone:
        mgr->replayData->clearState = clearState;

        mgr->replayData->difficulty = g_GameManager.difficulty;
        *(u32 *)mgr->replayData->playerName = *(u32 *)"NO NAME";
        mgr->replayData->gameConfiguration = *g_GameManager.cfg;

        for (i = 0; i < MAX_STAGES; i++)
        {
            mgr->replayData->header.stageReplayData[i] = NULL;
            mgr->replayData->header.stageReplayData2[i] = NULL;
        }
    }
    else
    {
        lastBookmark = NULL;
        for (i = 0; i < g_GameManager.currentStage; i++)
        {
            if (mgr->replayData->header.stageReplayData[i] != NULL)
            {
                lastBookmark = mgr->replayData->header.stageReplayData[i];
            }
        }
        if (lastBookmark != NULL)
        {
            lastBookmark->score = g_GameManager.globals->score;
        }
    }

    i = g_GameManager.currentStage;

    if (mgr->replayData->header.stageReplayData[i] != NULL)
    {
        g_ZunMemory.Free(mgr->replayData->header.stageReplayData[i]);
    }
    if (mgr->replayData->header.stageReplayData2[i] != NULL)
    {
        g_ZunMemory.Free(mgr->replayData->header.stageReplayData2[i]);
    }

    mgr->replayData->header.stageReplayData[i] = (StageReplayData *)g_ZunMemory.Alloc(0xd2f00, "rep data");
    mgr->replayData->header.stageReplayData2[i] = (StageReplayData *)g_ZunMemory.Alloc(0xd2f00, "rep data");

    stageReplay = mgr->replayData->header.stageReplayData[i];
    fpsReplayData = (u8 *)mgr->replayData->header.stageReplayData2[i];

    stageReplay->graze = g_GameManager.globals->graze;
    stageReplay->bombs = g_GameManager.GetBombsRemaining();
    stageReplay->lives = g_GameManager.GetLives();
    stageReplay->power = g_GameManager.GetPower();
    stageReplay->rank = g_GameManager.rank;
    stageReplay->pointItemsCollected = g_GameManager.globals->pointItemsCollected;
    stageReplay->rngSeed = (u16)g_GameManager.unk3ddbc;
    stageReplay->character = g_GameManager.character;
    stageReplay->unk0x21 = (u8)g_GameManager.globals->spellcardsCaptured;
    stageReplay->pointItemExteds = g_GameManager.globals->pointItemExtendsSoFar;
    stageReplay->nextPointItemExtendThreshold = g_GameManager.globals->nextPointItemExtendThreshold;
    stageReplay->youkaiGauge = g_GameManager.globals->youkaiGauge;
    stageReplay->clockTime = g_GameManager.GetClockTime();
    stageReplay->pointItemValue = g_GameManager.globals->pointItemValue;

    mgr->recordingCursor = (u8 *)stageReplay + 0x24;
    mgr->recordingCursor2 = mgr->recordingCursor;
    mgr->fpsCursor = fpsReplayData;

    *(u16 *)mgr->recordingCursor = 0;
    *(u16 *)(mgr->recordingCursor2 + 2) = 0;
    *(u16 *)(mgr->recordingCursor2 + 4) = g_Rng.GetSeed();

    mgr->unk4 = 0;
    mgr->unk4e = 0;

    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x452d60
ZunResult ReplayManager::AddedCallbackDemo(ReplayManager *mgr)
{
    i32 i;
    u8 *fpsReplayData;
    StageReplayData *lastBookmark;
    i32 fileSize;
    StageReplayData *stageReplay;

    mgr->frameId = 0;

    if (mgr->replayData == NULL)
    {
        mgr->replayData =
            (ReplayData *)FileSystem::OpenFile(mgr->replayFile, &fileSize, !g_GameManager.flags.isDemoMode);
        mgr->replayData = ReplayManager::LoadReplayData(mgr->replayData, fileSize);
        if (mgr->replayData == NULL)
        {
            return ZUN_ERROR;
        }
        mgr->unk0xc = NULL;

        for (i = 0; i < MAX_STAGES; i++)
        {
            if (mgr->replayData->header.stageReplayData[i] != NULL)
            {
                mgr->replayData->header.stageReplayData[i] =
                    (StageReplayData *)((u32)mgr->replayData->header.stageReplayData[i] + (u32)mgr->replayData);
            }
            if (mgr->replayData->header.stageReplayData2[i] != NULL)
            {
                mgr->replayData->header.stageReplayData2[i] =
                    (StageReplayData *)((u32)mgr->replayData->header.stageReplayData2[i] + (u32)mgr->replayData);
            }
        }
    }

    i = g_GameManager.currentStage;

    if (mgr->replayData->header.stageReplayData[i] == NULL)
    {
        return ZUN_ERROR;
    }

    stageReplay = mgr->replayData->header.stageReplayData[i];
    fpsReplayData = (u8 *)mgr->replayData->header.stageReplayData2[i];

    g_GameManager.shotType = mgr->replayData->shotType;
    g_GameManager.fullShotType = mgr->replayData->shotType % 1;
    g_GameManager.shotType = mgr->replayData->shotType;
    g_GameManager.difficulty = mgr->replayData->difficulty;

    g_GameManager.globals->pointItemsCollected = stageReplay->pointItemsCollected;
    g_GameManager.rank = stageReplay->rank;
    g_GameManager.SetLives(stageReplay->lives);
    g_GameManager.SetBombCount(stageReplay->bombs);
    g_GameManager.SetPower(stageReplay->power);
    g_GameManager.globals->graze = stageReplay->graze;

    mgr->recordingCursor = (u8 *)stageReplay + 0x24;
    mgr->recordingCursor2 = mgr->recordingCursor;

    g_GameManager.character = stageReplay->character;
    g_GameManager.globals->pointItemValue = stageReplay->pointItemValue;
    *g_GameManager.cfg = mgr->replayData->gameConfiguration;
    g_Rng.SetSeed(stageReplay->rngSeed);
    g_GameManager.globals->spellcardsCaptured = stageReplay->unk0x21;
    g_GameManager.globals->pointItemExtendsSoFar = stageReplay->pointItemExteds;
    g_GameManager.globals->nextPointItemExtendThreshold = stageReplay->nextPointItemExtendThreshold;
    g_GameManager.globals->youkaiGauge = stageReplay->youkaiGauge;
    g_GameManager.SetClockTime(stageReplay->clockTime);

    mgr->fpsCursor = fpsReplayData;
    mgr->unk4 = 0;

    lastBookmark = NULL;
    for (i = 0; i < g_GameManager.currentStage; i++)
    {
        if (mgr->replayData->header.stageReplayData[i] != NULL)
        {
            lastBookmark = mgr->replayData->header.stageReplayData[i];
        }
    }
    if (lastBookmark != NULL)
    {
        g_GameManager.globals->score = lastBookmark->score;
        g_GameManager.globals->displayScore = g_GameManager.globals->score;
    }

    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x453080
ZunResult ReplayManager::DeletedCallback(ReplayManager *mgr)
{
    if (mgr->altCalcChain != NULL)
    {
        g_Chain.Cut(mgr->altCalcChain);
        mgr->altCalcChain = NULL;
    }
    if (mgr->lowPrioCalcChain != NULL)
    {
        g_Chain.Cut(mgr->lowPrioCalcChain);
        mgr->lowPrioCalcChain = NULL;
    }

    g_ZunMemory.Free(g_ReplayManager->replayData);
    if (mgr->unk0xc != NULL)
    {
        g_ZunMemory.Free(mgr->unk0xc);
    }

    g_ZunMemory.RemoveFromRegistry(g_ReplayManager);
    delete g_ReplayManager;
    g_ReplayManager = NULL;
    g_ReplayManager = NULL;

    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x4531f0
#pragma var_order(i, mgr, textCursor, writtenBytes, compressedData, clampedSlowRate, compressedSize, stageInputLen, \
                  encodedData, replayDataCopy, userSectionText, fileHandle, userSectionPad, userSectionSize, \
                  userSectionMagic, encodeCursor, timeInfo, timeValue, timeString, checksum, checksumCursor, \
                  obfuscateOffset, obfuscateCursor, replayPath, replayName, stageName, strlenCursor, strlenNext, \
                  strlenOut)
void ReplayManager::SaveReplay(const char *replayPath, const char *replayName)
{
    ReplayManager *mgr;
    i32 i;
    i32 stageInputLen;
    u8 *encodedData;
    i32 encodeCursor;
    i32 compressedSize;
    u8 *compressedData;
    u32 checksum;
    u8 *checksumCursor;
    u8 obfuscateOffset;
    u8 *obfuscateCursor;
    f32 clampedSlowRate;
    i32 writtenBytes;
    HANDLE fileHandle;
    ReplayData replayDataCopy;

    u8 userSectionMagic[4];
    i32 userSectionSize;
    u8 userSectionPad[4];
    char userSectionText[0x400];

    char *textCursor;
    const char *stageName;
    long timeValue;
    struct tm *timeInfo;
    char timeString[0xfc];
    char *strlenCursor;
    char *strlenNext;
    i32 strlenOut;

    if (g_ReplayManager == NULL)
    {
        return;
    }
    mgr = g_ReplayManager;

    if (::FUN_00453cc0(mgr) != 0)
    {
        goto cutChain;
    }

    if (!g_GameManager.IsPracticeMode() && g_GameManager.difficulty < 4 &&
        memcmp(&g_Supervisor.cfg, &mgr->replayData->gameConfiguration, sizeof(GameConfiguration)) != 0)
    {
        goto abortSave;
    }
    if (mgr->replayData->gameConfiguration.slowMode != 0)
    {
        goto abortSave;
    }
    if (replayPath == NULL)
    {
        goto abortSave;
    }

    utils::DebugPrint("info : Replay File write %s\r\n", replayPath);

    encodedData = (u8 *)g_ZunMemory.Alloc(0x400000, "rep tmp");

    memcpy(&replayDataCopy, mgr->replayData, sizeof(ReplayData));

    FUN_004531a0();

    i = g_GameManager.currentStage2;
    mgr->replayData->header.stageReplayData[i]->score = g_GameManager.globals->score;

    encodeCursor = 0x68;
    encodeCursor += 0xcc;

    for (i = 0; i < MAX_STAGES; i++)
    {
        if (mgr->replayData->header.stageReplayData[i] != NULL)
        {
            stageInputLen = mgr->recordingStageBookmarks[i] - (u8 *)mgr->replayData->header.stageReplayData[i];
            memcpy(encodedData + encodeCursor - 0x68, mgr->replayData->header.stageReplayData[i], stageInputLen);
            *(i32 *)&replayDataCopy.header.stageReplayData[i] = encodeCursor;
            encodeCursor += stageInputLen;
        }
    }

    for (i = 0; i < MAX_STAGES; i++)
    {
        if (mgr->replayData->header.stageReplayData2[i] != NULL)
        {
            stageInputLen = mgr->fpsStageBookmarks[i] - (u8 *)mgr->replayData->header.stageReplayData2[i];
            memcpy(encodedData + encodeCursor - 0x68, mgr->replayData->header.stageReplayData2[i], stageInputLen);
            *(i32 *)&replayDataCopy.header.stageReplayData2[i] = encodeCursor;
            encodeCursor += stageInputLen;
        }
    }

    replayDataCopy.spellcardScore = g_GameManager.globals->displayScore;

    if (replayDataCopy.spellcardNumber >= 0)
    {
        memcpy(replayDataCopy.spellcardName, g_GameManager.catkData[replayDataCopy.spellcardNumber].spellName,
               sizeof(replayDataCopy.spellcardName));
    }

    clampedSlowRate = (g_Supervisor.lagNumerator / g_Supervisor.lagDenominator - 0.5f) * 2.0f;
    if (clampedSlowRate < 0.0f)
    {
        clampedSlowRate = 0.0f;
    }
    else if (clampedSlowRate >= 1.0f)
    {
        clampedSlowRate = 1.0f;
    }
    replayDataCopy.slowDownRate = (1.0f - clampedSlowRate) * 100.0f;

    *(u32 *)userSectionMagic = *(u32 *)"USER";
    userSectionPad[0] = 0;
    memset(userSectionText, 0, sizeof(userSectionText));

    textCursor = userSectionText;
    textCursor = sprintf(textCursor, "\x83\x76\x83\x8c\x83\x43\x83\x84\x81\x5b\x96\xbc\t%s\r\n", replayName);

    time((time_t *)&timeValue);
    timeInfo = localtime((time_t *)&timeValue);
    strftime(timeString, 20, "%Y/%m/%d %H:%M:%S", timeInfo);
    textCursor = sprintf(textCursor, "\x83\x76\x83\x8c\x83\x43\x8e\x9e\x8d\x8f\t%s\r\n", timeString);
    textCursor = sprintf(textCursor, "\x83\x4c\x83\x83\x83\x89\x96\xbc\t%s\r\n",
                         ResultScreen::GetCharacterName(g_GameManager.shotType));
    textCursor = sprintf(textCursor, "\x83\x58\x83\x52\x83\x41\t\t%d0\r\n", g_GameManager.globals->displayScore);
    textCursor = sprintf(textCursor, "\x93\xef\x88\xd5\x93\x78\t\t%s\r\n",
                         g_ReplayDifficultyNames[g_GameManager.difficulty]);

    if (replayDataCopy.spellcardNumber >= 0)
    {
        textCursor = sprintf(textCursor, "\x83\x4a\x81\x5b\x83\x68\x96\xbc\tNo.%3d %s\r\n",
                             replayDataCopy.spellcardNumber + 1, replayDataCopy.spellcardName);
    }
    else
    {
        if (g_GameManager.flags.unk4 != 0)
        {
            stageName = "Clear";
        }
        else
        {
            stageName = ResultScreen::GetStageName(g_GameManager.currentStage);
        }
        textCursor = sprintf(textCursor, "\x8d\xc5\x8f\x49\x83\x58\x83\x65\x81\x5b\x83\x57\t%s\r\n", stageName);
    }
    textCursor = sprintf(textCursor, "\x83\x7e\x83\x58\x89\xf1\x90\x94\t%d\r\n", g_GameManager.GetDeaths());
    textCursor = sprintf(textCursor, "\x83\x7b\x83\x80\x89\xf1\x90\x94\t%d\r\n", g_GameManager.GetBombsUsed());
    textCursor = sprintf(textCursor, "\x8f\x88\x97\x9d\x97\x8e\x82\xbf\x97\xa6\t%f%%\r\n", replayDataCopy.slowDownRate);

    *(i32 *)((u8 *)&g_GameManager + 0x3dab0) =
        (i32)((f32)g_GameManager.unk3DBA0 / (f32)g_GameManager.unk3DBA4 * 10000.0f);
    textCursor = sprintf(textCursor, "\x90\x6c\x8a\xd4\x97\xa6\t\t%3.2f\x81\x93\r\n",
                         (f32) * (i32 *)((u8 *)&g_GameManager + 0x3dab0) / 100.0f);
    textCursor = sprintf(textCursor, "\x83\x51\x81\x5b\x83\x80\x82\xcc\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\t%d.%.2d%c\r\n",
                         1, 0, 'd');

    strlenCursor = userSectionText;
    strlenNext = strlenCursor + 1;
    while (*strlenCursor++ != 0)
    {
    }
    strlenOut = strlenCursor - strlenNext;
    userSectionSize = strlenOut + 0xc;
    userSectionSize += userSectionSize & 1;

    replayDataCopy.header.unk0x7 = 1;

    {
        char *srcCursor;
        char *dstCursor;
        u8 copiedByte;

        srcCursor = (char *)replayName;
        dstCursor = replayDataCopy.playerName;
        do
        {
            copiedByte = *srcCursor;
            *dstCursor = copiedByte;
            srcCursor++;
            dstCursor++;
        } while (copiedByte);
    }

    ResultScreen::FormatDate(replayDataCopy.date);
    replayDataCopy.header.value1 = g_Rng.GetRandomU16InRange(0x80) + 0x40;
    replayDataCopy.unk0x68 = g_Rng.GetRandomU16InRange(0x100);
    replayDataCopy.header.unk0x14 = g_Rng.GetRandomU16InRange(0x100);
    *(f32 *)((u8 *)&replayDataCopy + 0x114) = replayDataCopy.slowDownRate + 1.12f;
    replayDataCopy.unk0x120 = 30;

    memcpy(encodedData, (u8 *)&replayDataCopy + 0x68, 0xcc);

    utils::DebugPrint("info : original size %d\r\n", encodeCursor);

    replayDataCopy.header.decompressedSize = encodeCursor - 0x68;
    compressedData = Lzss::Encode(encodedData, replayDataCopy.header.decompressedSize, &replayDataCopy.header.compressedSize);
    g_ZunMemory.Free(encodedData);
    compressedSize = replayDataCopy.header.compressedSize;

    checksumCursor = &replayDataCopy.header.value1;
    checksum = REPLAY_OBFUSCATION_VALUE;
    for (i = 0; (u32)i < 0x53; i++, checksumCursor++)
    {
        checksum += *checksumCursor;
    }
    checksumCursor = compressedData;
    for (i = 0; i < compressedSize; i++, checksumCursor++)
    {
        checksum += *checksumCursor;
    }
    replayDataCopy.header.checksum = checksum;

    obfuscateCursor = (u8 *)&replayDataCopy.header.compressedSize;
    obfuscateOffset = replayDataCopy.header.value1;
    for (i = 0; (u32)i < 0x50; i++, obfuscateCursor++)
    {
        *obfuscateCursor += obfuscateOffset;
        obfuscateOffset += 7;
    }
    obfuscateCursor = compressedData;
    for (i = 0; i < compressedSize; i++, obfuscateCursor++)
    {
        *obfuscateCursor += obfuscateOffset;
        obfuscateOffset += 7;
    }

    replayDataCopy.header.fileSize = compressedSize + 0x68;

    fileHandle = CreateFileA(replayPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == (HANDLE)-1)
    {
        goto abortSave;
    }
    WriteFile(fileHandle, &replayDataCopy, 0x68, (LPDWORD)&writtenBytes, NULL);
    WriteFile(fileHandle, compressedData, compressedSize, (LPDWORD)&writtenBytes, NULL);
    WriteFile(fileHandle, userSectionMagic, 0xc, (LPDWORD)&writtenBytes, NULL);
    WriteFile(fileHandle, userSectionText, userSectionSize - 0xc, (LPDWORD)&writtenBytes, NULL);
    CloseHandle(fileHandle);

    utils::DebugPrint("info : Size %d -> %d\r\n", encodeCursor, compressedSize + 0x68);
    GlobalFree(compressedData);

abortSave:
    for (i = 0; i < MAX_STAGES; i++)
    {
        if (g_ReplayManager->replayData->header.stageReplayData[i] != NULL)
        {
            g_ZunMemory.Free(g_ReplayManager->replayData->header.stageReplayData[i]);
        }
        if (mgr->replayData->header.stageReplayData2[i] != NULL)
        {
            g_ZunMemory.Free(mgr->replayData->header.stageReplayData2[i]);
        }
    }

cutChain:
    g_Chain.Cut(g_ReplayManager->calcChain);
}

// FUNCTION: th08 0x451d90
#pragma var_order(decodedReplay, i, replayData, obfuscateOffset, obfuscateCursor, checksum, checksumCursor)
ReplayData *ReplayManager::LoadReplayData(void *data, int fileSize)
{
    u8 *obfuscateCursor;
    u8 *checksumCursor;
    u32 checksum;
    i32 i;
    ReplayData *decodedReplay;
    ReplayData *replayData = (ReplayData *)data;
    u8 obfuscateOffset;

    if (replayData == NULL)
    {
        goto err1;
    }

    if (replayData->header.magic != *(u32 *)REPLAY_MAGIC)
    {
        goto err1;
    }

    if (replayData->header.version != REPLAY_VERSION)
    {
        goto err1;
    }

    obfuscateCursor = (u8 *)&replayData->header.compressedSize;
    obfuscateOffset = replayData->header.value1;

    for (i = 0; i < replayData->header.fileSize - (i32)offsetof(ReplayDataHeader, compressedSize);
         i++, obfuscateCursor++)
    {
        *obfuscateCursor -= obfuscateOffset;
        obfuscateOffset += 7;
    }

    checksumCursor = &replayData->header.value1;
    checksum = REPLAY_OBFUSCATION_VALUE;

    for (i = 0; i < replayData->header.fileSize - (i32)offsetof(ReplayDataHeader, value1); i++, checksumCursor++)
    {
        checksum += *checksumCursor;
    }

    if (checksum != replayData->header.checksum)
    {
        goto err1;
    }

    decodedReplay = (ReplayData *)g_ZunMemory.Alloc(replayData->header.decompressedSize + sizeof(ReplayDataHeader) +
                                                    (fileSize - replayData->header.fileSize));

    memcpy(&decodedReplay->header, data, sizeof(ReplayDataHeader));

    Lzss::Decode((u8 *)replayData + sizeof(ReplayDataHeader), replayData->header.compressedSize,
                 (u8 *)decodedReplay + sizeof(ReplayDataHeader), replayData->header.decompressedSize);

    memcpy((u8 *)decodedReplay + sizeof(ReplayDataHeader) + replayData->header.decompressedSize,
           (u8 *)data + replayData->header.fileSize, fileSize - replayData->header.fileSize);

    replayData = decodedReplay;

    if (replayData->gameConfiguration.slowMode != 0)
    {
        goto err2;
    }

    if (g_Supervisor.CheckVersion(replayData->exeVersion, replayData->exeSize, replayData->exeChecksum) != ZUN_SUCCESS)
    {
        goto err2;
    }

    g_ZunMemory.Free(data);

    return decodedReplay;

err1:
    g_ZunMemory.Free(data);
    return NULL;

err2:
    g_ZunMemory.Free(data);
    g_ZunMemory.Free(decodedReplay);

    return NULL;
}

} // namespace th08

// FUNCTION: th08 0x452490
th08::ChainCallbackResult FUN_00452490(th08::ReplayManager *mgr)
{
    using namespace th08;

    if (g_GameManager.flags.unk2 == 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (g_GameManager.flags.unk9 != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (g_Gui.IsDialoguePresent() && ::FUN_00437dc7(&g_Gui) && mgr->frameId % 3 != 2)
    {
        return CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB;
    }
    if (g_GameManager.flags.unk9 == 0 && g_GameManager.replayMode == 2 && ::FUN_0042f1f0(&g_EnemyManager) == 0 &&
        mgr->frameId % 5 != 4)
    {
        return CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB;
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x4526c0
th08::ChainCallbackResult FUN_004526c0(th08::ReplayManager *mgr)
{
    using namespace th08;
    i32 unused;

    if (g_GameManager.flags.unk2 == 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (g_GameManager.cfg->slowMode != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (g_GameManager.flags.unk9 != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }

    unused = 0;

    g_ReplayLastFrameInput = g_ReplayCurFrameInput;
    g_ReplayCurFrameInput = *(u16 *)mgr->recordingCursor2;
    mgr->recordingCursor2 += 6;

    g_IsEighthFrameOfHeldInput = 0;
    if (g_ReplayLastFrameInput == g_ReplayCurFrameInput)
    {
        if (g_NumOfFramesInputsWereHeld >= 30)
        {
            if (g_NumOfFramesInputsWereHeld % 8 == 0)
            {
                g_IsEighthFrameOfHeldInput = 1;
            }
            if (g_NumOfFramesInputsWereHeld >= 38)
            {
                g_NumOfFramesInputsWereHeld = 30;
            }
        }
        g_NumOfFramesInputsWereHeld++;
    }
    else
    {
        g_NumOfFramesInputsWereHeld = 0;
    }

    if (mgr->frameId % 30 == 0)
    {
        *(u16 *)&g_Supervisor.unk198 = ((i8)mgr->fpsCursor[1]) & 0x7f;
        g_Supervisor.unk0x33c = ((i8)mgr->fpsCursor[1]) >> 7;
        mgr->fpsCursor += 1;
    }

    mgr->frameId++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x4531a0
void FUN_004531a0(void)
{
    using namespace th08;
    ReplayManager *mgr;
    i32 stage;

    mgr = g_ReplayManager;
    if (mgr != NULL)
    {
        mgr->recordingCursor += 2;
        *(u16 *)mgr->recordingCursor = 0;
        stage = g_GameManager.currentStage;
        mgr->recordingStageBookmarks[stage] = mgr->recordingCursor + 6;
    }
}

// FUNCTION: th08 0x453cc0
i32 FUN_00453cc0(th08::ReplayManager *mgr)
{
    return mgr->isDemo;
}
