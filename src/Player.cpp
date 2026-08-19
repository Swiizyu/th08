#include "th_pch.h"

#include "Player.hpp"
#include "EffectManager.hpp"

namespace th08
{

// FUNCTION: th08 0x449e50
PlayerUnkStruct0x2ec::PlayerUnkStruct0x2ec()
{
}

// FUNCTION: th08 0x449ea0
PlayerUnkStruct0xb7858::PlayerUnkStruct0xb7858()
{
}

// FUNCTION: th08 0x449ef0
PlayerUnkStruct0x460::PlayerUnkStruct0x460()
{
}

// FUNCTION: th08 0x449f70
PlayerUnkStruct0x16f0::PlayerUnkStruct0x16f0()
{
}

// FUNCTION: th08 0x44e9e0
i32 __fastcall FUN_0044e9e0(void *, Effect *effect)
{
    effect->vm.pos.x = g_GameManager.arcadeRegionTopLeftPos.x + effect->position.x;
    effect->vm.pos.y = g_GameManager.arcadeRegionTopLeftPos.y + effect->position.y;
    effect->vm.pos.z = 0.49f;
    g_AnmManager->Draw2D(&effect->vm);
    return 0;
}

// FUNCTION: th08 0x450580
i32 __fastcall FUN_00450580(void *, void *data)
{
    if (*(i16 *)((u8 *)data + 0x462) == 1)
    {
        *(f32 *)((u8 *)data + 0x440) -= g_Rng.GetRandomF32InRange(0.1f) + 0.27f;
    }
    return 0;
}

// FUNCTION: th08 0x441850
void GameManager::AddPower(i32 power)
{
    if (this->IsTampered())
    {
        CRASH_GAME();
    }
    this->globals->playerPower += power;
    this->UpdateAntiTamper();
}

// FUNCTION: th08 0x44e260
void GameManager::AddToDeaths(i32 amount)
{
    if (this->IsTampered())
    {
        CRASH_GAME();
    }
    this->globals->deaths += amount;
    this->globals->deathInStage += amount;
    (*(i32 *)((u8 *)this + 0x3da9c))++;
    this->UpdateAntiTamper();
}

// FUNCTION: th08 0x44e2e0
void GameManager::AddToBombsUsed(i32 amount)
{
    if (this->IsTampered())
    {
        CRASH_GAME();
    }
    this->globals->bombsUsed += amount;
    this->globals->bombsUsedInStage += amount;
    this->UpdateAntiTamper();
}

// FUNCTION: th08 0x44e0f0
void AnmVm::FUN_0044e0f0()
{
    this->blendMode = 1;
}

// FUNCTION: th08 0x44e120
void AnmVm::FUN_0044e120()
{
    this->blendMode = 0;
}

// FUNCTION: th08 0x44cba0
void Player::FUN_0044cba0()
{
    ((PlayerFlags *)this)->flag2 = 0;
    *(i32 *)((u8 *)this + 0xfc) = 0;
    ((PlayerFlags *)this)->flag7 = ((PlayerFlags *)this)->flag0;
}

// FUNCTION: th08 0x44d150
void Player::FUN_0044d150()
{
    *(u32 *)this &= ~4;
    *(i32 *)((u8 *)this + 0xfc) = 0;
}

// FUNCTION: th08 0x451640
void Player::FUN_00451640()
{
    if ((i32)*(ZunTimer *)((u8 *)this + 0xe2ac4) < 0)
    {
        *(ZunTimer *)((u8 *)this + 0xe2ac4) = 0;
    }
}

// FUNCTION: th08 0x441830
ZunBool ZunTimer::operator!=(i32 value)
{
    return this->current != value;
}

// FUNCTION: th08 0x44e350
void PlayerUnkStruct0x40::FUN_0044e350()
{
    this->flag = 0;
}

// FUNCTION: th08 0x44e140
void GameManager::SetYoukaiGauge(i16 gauge)
{
    this->globals->youkaiGauge = gauge;
}

// FUNCTION: th08 0x451d50
ZunBool Player::FUN_00451d50()
{
    return *(i32 *)((u8 *)this + 0xfdc) != 0 &&
           *(i32 *)((u8 *)this + 0xfe0) == 4;
}

// FUNCTION: th08 0x40bc20
ZunBool Player::IsHuman()
{
    return this->isYoukai == 0;
}

// FUNCTION: th08 0x40bc40
ZunBool Player::IsYoukai()
{
    return this->isYoukai;
}

DIFFABLE_STATIC(Player, g_Player);
DIFFABLE_STATIC(ChainElem *, g_PlayerCalcChain);
DIFFABLE_STATIC(ChainElem *, g_PlayerDrawChainHighPrio);
DIFFABLE_STATIC(ChainElem *, g_PlayerDrawChainLowPrio);

// STUB: th08 0x44c230
ZunResult Player::RegisterChain(u32 param)
{
    return ZUN_SUCCESS;
}

// STUB: th08 0x44c390
ChainCallbackResult Player::OnUpdate(Player *player)
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// STUB: th08 0x44d530
ChainCallbackResult Player::OnDrawHighPrio(Player *player)
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// STUB: th08 0x44d630
ChainCallbackResult Player::OnDrawLowPrio(Player *player)
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// STUB: th08 0x44d650
ZunResult Player::AddedCallback(Player *player)
{
    return ZUN_SUCCESS;
}

// STUB: th08 0x44dc60
ZunResult Player::DeletedCallback(Player *player)
{
    return ZUN_SUCCESS;
}

void Player::CutChain()
{
    g_Chain.Cut(g_PlayerCalcChain);
    g_PlayerCalcChain = NULL;
    g_Chain.Cut(g_PlayerDrawChainHighPrio);
    g_PlayerDrawChainHighPrio = NULL;
    g_Chain.Cut(g_PlayerDrawChainLowPrio);
    g_PlayerDrawChainLowPrio = NULL;
}

// STUB: th08 0x44dd70
ZunResult Player::LoadShtFile(PlayerRawShtFile **header, const char *path)
{
    return ZUN_SUCCESS;
}

} /* namespace th08 */

// FUNCTION: th08 0x44e370
void FUN_0044e370(void *data)
{
    memset(data, 0, 0x40);
    *(i32 *)((u8 *)data + 0x38) = 1;
}

