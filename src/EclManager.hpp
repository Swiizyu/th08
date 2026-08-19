#pragma once

#include "Supervisor.hpp"
#include "inttypes.hpp"

namespace th08
{

struct EclTimeline
{
    EclTimeline();

    ZunTimer timer;
};

struct EclTimelineHeader
{
    u8 magic[6];
    i16 timelineCount;
    EclTimeline *timelines[1];
};

struct EclManager
{
    i32 GetTimelineCount();
    EclTimeline *GetTimeline(i32 timelineIdx);

    EclTimelineHeader *timelineFile;
};

} /* namespace th08 */
