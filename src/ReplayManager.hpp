#pragma once

#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "utils.hpp"

#include "Global.hpp"
#include "ScoreDat.hpp"

#include <windows.h>

#define REPLAY_MAGIC "T8RP"
#define REPLAY_VERSION 6
#define REPLAY_OBFUSCATION_VALUE 0x3f000318

namespace th08
{

struct StageReplayData
{
    u32 score;
    i32 pointItemsCollected;
    i32 graze;
    i32 pointItemExteds;
    i32 nextPointItemExtendThreshold;
    i32 pointItemValue;
    i16 youkaiGauge;
    u16 rngSeed;
    u8 power;
    u8 lives;
    u8 bombs;
    u8 rank;
    u8 character;
    u8 unk0x21;
    i8 clockTime;

    unknown_fields(0x23, 0x1d);
};

C_ASSERT(sizeof(StageReplayData) == 0x40);

struct ReplayDataHeader
{
    u32 magic;
    u16 version;
    u8 unk0x6;
    u8 unk0x7;

    unknown_fields(0x8, 0x4);

    i32 fileSize;
    i32 checksum;

    u8 unk0x14;
    u8 value1;
    u8 unk0x16;
    u8 unk0x17;

    i32 compressedSize;
    i32 decompressedSize;

    StageReplayData *stageReplayData[MAX_STAGES];
    StageReplayData *stageReplayData2[MAX_STAGES];
};

struct ReplayData
{
    ReplayDataHeader header;

    u8 unk0x68;
    u8 minorVersion;
    u8 shotType;
    u8 difficulty;

    char date[6];
    char playerName[8];

    u8 unk0x7a;
    u8 isPractice;
    i16 spellcardNumber;

    char spellcardName[48];

    u16 majorVersion;

    u32 spellcardScore;

    GameConfiguration gameConfiguration;

    unknown_fields(0xf0, 0x28);

    float slowDownRate;
    i8 clearState;

    i32 unk0x120;
    i32 exeSize;
    i32 exeChecksum;
    char exeVersion[6];
};

C_ASSERT(sizeof(ReplayData) == 0x134);

struct ReplayManager
{
    ReplayManager();

    static ZunResult RegisterChain(i32 isDemo, char *replayFile);
    static ChainCallbackResult OnUpdateLowPrio(ReplayManager *mgr);
    static ChainCallbackResult OnUpdateHighPrio(ReplayManager *mgr);
    static ChainCallbackResult OnUpdateHighPrioDemo(ReplayManager *mgr);
    static ZunResult AddedCallback(ReplayManager *mgr);
    static ZunResult AddedCallbackDemo(ReplayManager *mgr);
    static ZunResult DeletedCallback(ReplayManager *mgr);
    static void SaveReplay(const char *replayPath, const char *replayName);
    static ReplayData *LoadReplayData(void *replayData, int fileSize);

    i32 frameId;
    i32 unk4;
    ReplayData *replayData;
    void *unk0xc;
    i32 isDemo;
    char *replayFile;

    Float3 unk18;
    Float3 unk24;
    Float3 unk30;
    Float3 unk3c;

    unknown_fields(0x48, 6);
    u16 unk4e;

    u8 *recordingCursor;
    u8 *recordingStageBookmarks[MAX_STAGES];
    u8 *recordingCursor2;

    unknown_fields(0x7c, 0x24);

    u8 *fpsCursor;
    u8 *fpsStageBookmarks[MAX_STAGES];

    ChainElem *calcChain;
    i32 unk0xcc;
    ChainElem *altCalcChain;
    ChainElem *lowPrioCalcChain;
    u16 rngSeed;
    u16 inputFlags;
};

C_ASSERT(sizeof(ReplayManager) == 0xdc);

DIFFABLE_EXTERN(ReplayManager *, g_ReplayManager);

} // namespace th08
