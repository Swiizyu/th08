#include "th_pch.h"

#include "EnemyManager.hpp"
#include "AsciiManager.hpp"
#include "Background.hpp"
#include "EffectManager.hpp"
#include "EclManager.hpp"
#include "Spellcard.hpp"
#include "ScreenEffect.hpp"
#include "Player.hpp"

namespace th08
{

DIFFABLE_STATIC(EnemyManager, g_EnemyManager);
DIFFABLE_STATIC(EffectManager, g_EffectManager);
DIFFABLE_STATIC(ChainElem, g_EffectManagerCalcChain);
DIFFABLE_STATIC(ChainElem, g_EffectManagerDrawChain);
DIFFABLE_STATIC(ChainElem, g_EnemyManagerCalcChain);
DIFFABLE_STATIC(ChainElem, g_EnemyManagerDrawChainHighPrio);
DIFFABLE_STATIC(ChainElem, g_EnemyManagerDrawChainLowPrio);

// FUNCTION: th08 0x41fdf0
void ZunTimer::operator+=(i32 value)
{
    this->Increment(value);
}

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

// FUNCTION: th08 0x428700
void Float3::FUN_00428700(float angle, float magnitudeX, float magnitudeY)
{
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul [magnitudeX]
        fstp [eax]
        fmul [magnitudeY]
        fstp [eax + 4]
    }
}

// FUNCTION: th08 0x428720
ZunBool AnmVm::FUN_00428720()
{
    return this->currentInstruction == NULL;
}

// FUNCTION: th08 0x42a410
EnemyUnkStruct4::EnemyUnkStruct4()
{
    memset(this, 0, sizeof(EnemyUnkStruct4));
    this->unk0x204 = -1;
}

// FUNCTION: th08 0x425e60
i32 Effect::FUN_00425e60()
{
    *(Float3 *)((u8 *)this + 0x2a4) += *(Float3 *)((u8 *)this + 0x2bc);
    *(Float3 *)((u8 *)this + 0x2bc) += *(Float3 *)((u8 *)this + 0x2c8);
    return 1;
}

// FUNCTION: th08 0x425fe0
i32 Effect::FUN_00425fe0()
{
    *(u8 *)((u8 *)this + 0x354) = 2;
    *(i32 *)((u8 *)this + 0x2ec) = 0;
    *(i32 *)((u8 *)this + 0x2f0) = 0;
    *(i32 *)((u8 *)this + 0x2f4) = 0;
    *(i32 *)((u8 *)this + 0x314) = 0;
    return 0;
}

// FUNCTION: th08 0x426b20
i32 Effect::FUN_00426b20()
{
    f32 angle;

    *(Float3 *)((u8 *)this + 0x2e0) = this->position;
    *(f32 *)((u8 *)this + 0x2e8) = 0.0f;
    angle = g_Rng.GetRandomF32InRange(ZUN_2PI) - ZUN_PI;
    *(f32 *)((u8 *)this + 0x2ec) = cosf(angle);
    *(f32 *)((u8 *)this + 0x2f0) = sinf(angle);
    *(f32 *)((u8 *)this + 0x2f4) = 0.0f;
    return 0;
}

// FUNCTION: th08 0x426bb0
i32 Effect::FUN_00426bb0()
{
    f32 distance;

    distance = 256.0f - (f32)*(ZunTimer *)((u8 *)this + 0x338) * 256.0f / 60.0f;
    this->position = *(Float3 *)((u8 *)this + 0x2ec) * distance + *(Float3 *)((u8 *)this + 0x2e0);
    this->position.z = 0.0f;
    return 1;
}

// FUNCTION: th08 0x426c40
i32 Effect::FUN_00426c40()
{
    if (this->vm.FUN_00428720())
    {
        return 0;
    }
    this->position = g_Player.position;
    return 1;
}

// FUNCTION: th08 0x426c90
i32 Effect::FUN_00426c90()
{
    f32 distance;

    distance = 256.0f - (f32)*(ZunTimer *)((u8 *)this + 0x338) * 256.0f / 240.0f;
    this->position = *(Float3 *)((u8 *)this + 0x2ec) * distance + *(Float3 *)((u8 *)this + 0x2e0);
    return 1;
}

// FUNCTION: th08 0x426d10
#pragma var_order(effect, i)
void Float3::FUN_00426d10()
{
    Effect *effect;
    i32 i;

    effect = (Effect *)((u8 *)&g_EffectManager + 0x1c);
    for (i = 0; i < 0x200; i++, effect = (Effect *)((u8 *)effect + 0x360))
    {
        if (*(i8 *)((u8 *)effect + 0x351) == 0x33)
        {
            *(Float3 *)((u8 *)effect + 0x2d4) += *this;
        }
    }
}

// FUNCTION: th08 0x427990
i32 Effect::FUN_00427990()
{
    *(u8 *)((u8 *)this + 0x356) = 1;
    *(i32 *)((u8 *)this + 0x320) = *(i32 *)((u8 *)this + 0x18);
    *(i32 *)((u8 *)this + 0x314) = *(i32 *)((u8 *)this + 0x208);
    return 1;
}

// FUNCTION: th08 0x4230c0
void EclExIns::FUN_004230c0(i32 value)
{
    *(i32 *)((u8 *)this + 0x30) = value;
}

// FUNCTION: th08 0x4230e0
void EclExIns::FUN_004230e0(i32 index, i32 value0, i32 value1)
{
    *(i32 *)((u8 *)this + index * 4 + 0x5c) = value0;
    *(i32 *)((u8 *)this + index * 4 + 0x3c) = value1;
}

// FUNCTION: th08 0x423110
void EclExIns::FUN_00423110(i32 index, i32 value)
{
    *(i32 *)((u8 *)this + index * 4 + 0x7c) = value;
}

// FUNCTION: th08 0x423130
void EclExIns::FUN_00423130(i32 value)
{
    *(i32 *)((u8 *)this + 0x20) = value;
}

// FUNCTION: th08 0x4233d0
void __fastcall EclExIns::FUN_004233d0(void *)
{
    ScreenEffect::RegisterChain(SCREEN_EFFECT_FULL_FADE_OUT, 60, 1, -1, 0, 21);
}

// FUNCTION: th08 0x424e00
void __fastcall EclExIns::FUN_00424e00(void *)
{
    g_Background.FUN_00409160(0xffc03030);
}

// FUNCTION: th08 0x424e20
void __fastcall EclExIns::FUN_00424e20(void *)
{
    ScreenEffect::RegisterChain(SCREEN_EFFECT_UNK7, 16, 20, 20, 20, 21);
}

// FUNCTION: th08 0x424f60
void __fastcall EclExIns::FUN_00424f60(void *)
{
    ScreenEffect::RegisterChain(SCREEN_EFFECT_FULL_FADE_OUT, 180, 1, -1, 0, 21);
}

// FUNCTION: th08 0x424f90
void __fastcall EclExIns::FUN_00424f90(void *instruction)
{
    i32 divisor;
    f32 result;

    divisor = *(i32 *)((u8 *)instruction + 0x10);
    result = 1.0f / divisor;
    *(f32 *)((u8 *)&g_Supervisor + 0x188) = result;
}

// FUNCTION: th08 0x424fc0
void __fastcall EclExIns::FUN_00424fc0(void *)
{
    *(i32 *)((u8 *)this->enemyData + 0x18) = g_GameManager.currentSpellCardNumber;
}

// FUNCTION: th08 0x425020
void __fastcall EclExIns::FUN_00425020(void *)
{
    g_Spellcard.spellcard_fun_00416160();
}

// FUNCTION: th08 0x425040
void __fastcall EclExIns::FUN_00425040(void *)
{
    *(i32 *)((u8 *)this->enemyData + 0x18) = g_GameManager.globals->spellcardsCaptured;
}

// FUNCTION: th08 0x423390
void __fastcall EclExIns::MystiaNightBlindness(void *)
{
    *(i32 *)((u8 *)&g_AsciiManager + 93960) = *(i32 *)((u8 *)this->enemyData + 0x18);
    *(i32 *)((u8 *)&g_AsciiManager + 93956) = *(i32 *)((u8 *)this->enemyData + 0x38);
}

// FUNCTION: th08 0x4182e0
EclManager::EclManager()
{
}

// FUNCTION: th08 0x418300
EclManagerSub::EclManagerSub()
{
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

// FUNCTION: th08 0x428100
ChainCallbackResult EffectManager::DrawUnkTypeEffects()
{
    Effect *effect;

    effect = this->drawListHead;
    while (effect != NULL)
    {
        if (effect->drawCallback != NULL)
        {
            effect->drawCallback(effect);
        }
        else
        {
            effect->vm.pos = effect->position;
            effect->vm.pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
            effect->vm.pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
            effect->vm.pos += effect->vm.pos2;
            effect->vm.pos.z = 0.04f;
            g_AnmManager->Draw2D(&effect->vm);
        }
        effect = effect->next;
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
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

