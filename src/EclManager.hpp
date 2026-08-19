#pragma once

#include "Supervisor.hpp"
#include "inttypes.hpp"

namespace th08
{

struct EclTimeline
{
    EclTimeline();

    ZunTimer timer;
    u32 unknown;
};
C_ASSERT(sizeof(EclTimeline) == 0x10);

struct EclExIns
{
    void FUN_004230c0(i32 value);
    void FUN_004230e0(i32 index, i32 value0, i32 value1);
    void FUN_00423110(i32 index, i32 value);
    void FUN_00423130(i32 value);
    void __fastcall FUN_004233d0(void *instruction);
    void __fastcall FUN_00424a00(void *instruction);
    void __fastcall FUN_00424e00(void *instruction);
    void __fastcall FUN_00424e20(void *instruction);
    void __fastcall FUN_00424f60(void *instruction);
    void __fastcall FUN_00424f90(void *instruction);
    void __fastcall FUN_00424fc0(void *instruction);
    void __fastcall FUN_00425020(void *instruction);
    void __fastcall FUN_00425040(void *instruction);
    void __fastcall MystiaNightBlindness(void *unused);

    u8 data[0x2ca0];
    void *enemyData;
};

struct EclTimelineHeader
{
    u8 magic[6];
    i16 timelineCount;
    EclTimeline *timelines[1];
};

struct EclTimelineContext
{
    void *timeline;
    ZunTimer timer1;
    u8 unknown0x10[0x80];
    ZunTimer timer2;
    u8 unknown0x9c[0x188];
    i16 timelineIndex;
    u8 padding[2];
};
C_ASSERT(sizeof(EclTimelineContext) == 0x228);

struct EclManagerSub
{
    EclManagerSub();

    u8 unknown[0x100];
    Float3 vectors[8];
};

struct EclManager
{
    EclManager();
    void FUN_00418420();
    i32 FUN_00418450(EclTimelineContext *context, i16 timelineIndex);
    i32 GetTimelineCount();
    EclTimeline *GetTimeline(i32 timelineIdx);

    EclTimelineHeader *timelineFile;
    void *unknown;
    EclManagerSub sub;
    u8 unknown0x168[0x20];
};
C_ASSERT(sizeof(EclManager) == 0x188);

DIFFABLE_EXTERN(EclManager, g_EclManager);

} /* namespace th08 */
