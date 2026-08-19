#include "th_pch.h"

#include "Player.hpp"

namespace th08
{

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

