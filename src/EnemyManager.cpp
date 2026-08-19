#include "th_pch.h"

#include "EnemyManager.hpp"
#include "EffectManager.hpp"
#include "EclManager.hpp"

namespace th08
{

DIFFABLE_STATIC(EnemyManager, g_EnemyManager);
DIFFABLE_STATIC(EffectManager, g_EffectManager);
DIFFABLE_STATIC(ChainElem, g_EffectManagerCalcChain);
DIFFABLE_STATIC(ChainElem, g_EffectManagerDrawChain);
DIFFABLE_STATIC(ChainElem, g_EnemyManagerCalcChain);
DIFFABLE_STATIC(ChainElem, g_EnemyManagerDrawChainHighPrio);
DIFFABLE_STATIC(ChainElem, g_EnemyManagerDrawChainLowPrio);

// FUNCTION: th08 0x42a450
EnemyUnkStruct2::EnemyUnkStruct2()
{
}

// FUNCTION: th08 0x42a490
EnemyUnkStruct0x1c::EnemyUnkStruct0x1c()
{
}

// FUNCTION: th08 0x42a4c0
EnemyUnkStruct3::EnemyUnkStruct3()
{
}

// FUNCTION: th08 0x4286e0
void Float3::FromAngleMagnitude(float angle, float magnitude)
{
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul [magnitude]
        fstp [eax]
        fmul [magnitude]
        fstp [eax + 4]
    }
}

// FUNCTION: th08 0x449f50
EclTimeline::EclTimeline()
{
}

// FUNCTION: th08 0x42dfb0
i32 EclManager::GetTimelineCount()
{
    return this->timelineFile->timelineCount;
}

// FUNCTION: th08 0x42dfd0
EclTimeline *EclManager::GetTimeline(i32 timelineIdx)
{
    return this->timelineFile->timelines[timelineIdx];
}

// FUNCTION: th08 0x425410
void EffectManager::ResetEffects()
{
    memset(this, 0, sizeof(EffectManager));
}

// FUNCTION: th08 0x4286b0
void EffectManager::CutChain()
{
    g_Chain.Cut(&g_EffectManagerCalcChain);
    g_Chain.Cut(&g_EffectManagerDrawChain);
}

// FUNCTION: th08 0x41fd20
ZunBool Enemy::FUN_0041fd20()
{
    return *(i32 *)((u8 *)this + 0x2da4) != 0;
}

// STUB: th08 0x41fd40
i32 Enemy::GetFamiliarCount()
{
    return 0;
}

// STUB: th08 0x429e00
void EnemyManager::Initialize()
{
}

// STUB: th08 0x42c590
ZunResult EnemyManager::RegisterChain()
{
    return ZUN_ERROR;
}

// STUB: th08 0x42c660
ChainCallbackResult EnemyManager::OnUpdate()
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// STUB: th08 0x42e120
ChainCallbackResult EnemyManager::OnDrawHighPrio(EnemyManager *enemyManager)
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// STUB: th08 0x42e140
ChainCallbackResult EnemyManager::OnDrawImpl()
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// STUB: th08 0x42eb90
ChainCallbackResult EnemyManager::OnDrawLowPrio(EnemyManager *enemyManager)
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// STUB: th08 0x42ebf0
ZunResult EnemyManager::AddedCallback(EnemyManager *enemyManager)
{
    return ZUN_ERROR;
}

// STUB: th08 0x42ee80
ZunResult EnemyManager::DeletedCallback(EnemyManager *enemyManager)
{
    return ZUN_ERROR;
}

// STUB: th08 0x42ef70
void EnemyManager::CutChain()
{
}

// STUB: th08 0x42efb0
i32 EnemyManager::DespawnAllEnemies(i32 param_1, i32 param_2)
{
    return 0;
}

} /* namespace th08 */

// FUNCTION: th08 0x427250
i32 FUN_00427250(void *)
{
    return 1;
}

// FUNCTION: th08 0x40d3d0
ZunBool FUN_0040d3d0(void *data)
{
    return *((i32 *)data + 2) != *(i32 *)data;
}

