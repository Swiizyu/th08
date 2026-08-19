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

struct EclManagerSub
{
    EclManagerSub();

    u8 unknown[0x100];
    Float3 vectors[8];
};

struct EclManager
{
    EclManager();
    i32 GetTimelineCount();
    EclTimeline *GetTimeline(i32 timelineIdx);

    EclTimelineHeader *timelineFile;
    void *unknown;
    EclManagerSub sub;
};

} /* namespace th08 */
