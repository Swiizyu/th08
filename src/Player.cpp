#include "th_pch.h"

#include "Player.hpp"
#include "AsciiManager.hpp"
#include "EffectManager.hpp"
#include "SoundPlayer.hpp"

u32 FUN_004338c0();
u32 FUN_0044e0e0();

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

struct PlayerCollisionObject
{
    Float3 position;
    u8 unknown0xc[4];
    f32 width;
    f32 height;
    u8 unknown0x18[8];
    f32 rotation;
    u8 unknown0x24[4];
    i32 itemType;
    u8 unknown0x2c[4];
    i32 collisionCount;
    u8 unknown0x34[8];
    u8 isActive;
    u8 padding[3];
};
C_ASSERT(sizeof(PlayerCollisionObject) == 0x40);

// FUNCTION: th08 0x44a5a0
#pragma var_order(maxPosition, minPosition)
i32 Player::CalcItemBoxCollision(Float3 *position, Float3 *hitbox)
{
    Float3 minPosition;
    Float3 maxPosition;

    if (this->playerState != 0 && this->playerState != 3 && this->playerState != 4)
    {
        return 0;
    }
    minPosition = *position - *hitbox / 2.0f;
    maxPosition = *position + *hitbox / 2.0f;
    if (*(f32 *)((u8 *)this + 0x3bc) >= maxPosition.x)
    {
        return 0;
    }
    if (*(f32 *)((u8 *)this + 0x3c8) <= minPosition.x)
    {
        return 0;
    }
    if (*(f32 *)((u8 *)this + 0x3c0) >= maxPosition.y)
    {
        return 0;
    }
    if (*(f32 *)((u8 *)this + 0x3cc) <= minPosition.y)
    {
        return 0;
    }
    return 1;
}

// FUNCTION: th08 0x449ff0
#pragma var_order(halfSize, deltaY, deltaX, i, rotated, delta, collision, rectMax)
i32 Player::FUN_00449ff0(Float3 *position, Float3 *hitbox)
{
    PlayerCollisionObject *collision;
    i32 i;
    f32 deltaX;
    f32 deltaY;
    Float3 delta;
    Float3 rotated;
    Float3 halfSize;
    Float3 rectMax;

    collision = (PlayerCollisionObject *)((u8 *)this + 0xbb834);
    for (i = 0; i < 0xc0; i++, collision++)
    {
        if (!collision->isActive)
        {
            continue;
        }
        if (collision->position.z != 0.0)
        {
            deltaX = position->x - collision->position.x;
            deltaY = position->y - collision->position.y;
            if (deltaX * deltaX + deltaY * deltaY <= collision->position.z * collision->position.z)
            {
                goto collided;
            }
            continue;
        }
        if (collision->rotation != 0.0f)
        {
            delta.x = position->x - collision->position.x;
            delta.y = position->y - collision->position.y;
            Rotate(&rotated, &delta, -collision->rotation);
            halfSize.x = collision->width / 2.0f;
            halfSize.y = collision->height / 2.0f;
            if (rotated.x >= -halfSize.x && rotated.x <= halfSize.x && rotated.y >= -halfSize.y &&
                rotated.y <= halfSize.y)
            {
                goto collided;
            }
            continue;
        }

        halfSize.x = collision->position.x - collision->width / 2.0f;
        halfSize.y = collision->position.y - collision->height / 2.0f;
        rectMax.x = collision->position.x + collision->width / 2.0f;
        rectMax.y = collision->position.y + collision->height / 2.0f;
        if (position->x > halfSize.x && position->x < rectMax.x && position->y > halfSize.y &&
            position->y < rectMax.y)
        {
            goto collided;
        }
        continue;
    }
    return 0;

collided:
    *(i32 *)((u8 *)this + 0xe2a90) = collision->itemType;
    collision->collisionCount++;
    return 2;
}

// FUNCTION: th08 0x44a6a0
#pragma var_order(playerRelativeTopLeft, laserBottomRight, laserTopLeft, playerRelativeBottomRight)
i32 Player::CalcLaserHitbox(Float3 *center, Float3 *size, Float3 *origin, f32 rotation, i32 canGraze)
{
    Float3 playerRelativeTopLeft;
    Float3 playerRelativeBottomRight;
    Float3 laserTopLeft;
    Float3 laserBottomRight;

    laserTopLeft = this->position - *origin;
    Rotate(&laserBottomRight, &laserTopLeft, -rotation);
    laserBottomRight.z = 0.0f;
    laserTopLeft = laserBottomRight + *origin;
    playerRelativeTopLeft = laserTopLeft - this->unk0x3D4;
    playerRelativeBottomRight = laserTopLeft + this->unk0x3D4;

    laserTopLeft = *center - *size / 2.0f;
    laserBottomRight = *center + *size / 2.0f;
    if (!(playerRelativeTopLeft.x > laserBottomRight.x || playerRelativeBottomRight.x < laserTopLeft.x ||
          playerRelativeTopLeft.y > laserBottomRight.y || playerRelativeBottomRight.y < laserTopLeft.y))
    {
        goto laser_collision;
    }

    if (!canGraze)
    {
        return 0;
    }

    laserTopLeft.x -= 48.0f;
    laserTopLeft.y -= 48.0f;
    laserBottomRight.x += 48.0f;
    laserBottomRight.y += 48.0f;
    if (playerRelativeTopLeft.x > laserBottomRight.x || playerRelativeBottomRight.x < laserTopLeft.x ||
        playerRelativeTopLeft.y > laserBottomRight.y || playerRelativeBottomRight.y < laserTopLeft.y)
    {
        return 0;
    }
    if (this->playerState == PLAYER_STATE_DEAD || this->playerState == PLAYER_STATE_SPAWNING)
    {
        return 0;
    }
    this->ScoreGraze(&this->position, 1);
    return 2;

laser_collision:
    if (this->playerState != PLAYER_STATE_ALIVE)
    {
        return 0;
    }
    this->Die();
    return 1;
}

// FUNCTION: th08 0x44a930
#pragma var_order(score, grazePos)
void Player::ScoreGraze(Float3 *position, i32 suppressTimeOrbEffects)
{
    Float3 grazePos;
    i32 score;

    grazePos = (this->position + *position) / 2.0f;
    g_EffectManager.SpawnEffect(8, &grazePos, 1, -1);
    g_GameManager.IncreaseSubrank(6);
    g_SoundPlayer.PlaySoundPositionedByIdx(SOUND_GRAZE, position->x);

    if (g_GameManager.GaugeIsModeratelyYoukai())
    {
        score = 4000;
    }
    else
    {
        score = 2000;
    }
    g_GameManager.AddScore(score);

    if (!suppressTimeOrbEffects && g_GameManager.GaugeIsExtremelyYoukai())
    {
        g_EffectManager.SpawnEffect(10, position, 1, -1);
    }
}

// FUNCTION: th08 0x44ab40
void Player::Die()
{
    g_GameManager.UpdateAntiTamper();
    g_EffectManager.SpawnEffect(6, &this->position, 16, -1);
    this->playerState = PLAYER_STATE_DEAD;
    ((ZunTimer *)((u8 *)this + 0xe2af4))->SetCurrent(0);
    g_SoundPlayer.PlaySoundPositionedByIdx(SOUND_PICHUN, this->position.x);
}

// FUNCTION: th08 0x4512f0
#pragma var_order(i, data)
void Player::FUN_004512f0()
{
    u8 *data;
    i32 i;

    data = (u8 *)this + 0xbe838;
    for (i = 0; i < 0x80; i++, data += 0x484)
    {
        if (*(i16 *)(data + 0x462) != 1)
        {
            continue;
        }
        if (*(i16 *)(data + 0x1fc) != 0)
        {
            ((AnmVm *)data)->SetZRotation(*(f32 *)(data + 0x450));
        }
        ((AnmVm *)data)->pos.x = g_GameManager.arcadeRegionTopLeftPos.x + *(f32 *)(data + 0x2a4);
        ((AnmVm *)data)->pos.y = g_GameManager.arcadeRegionTopLeftPos.y + *(f32 *)(data + 0x2a8);
        ((AnmVm *)data)->pos.z = 0.4f;
        if (*(i8 *)(data + 0x470) != 0)
        {
            *(u8 *)(data + 0x1f2) = 0xff;
            *(u8 *)(data + 0x1f1) = 0x40;
            *(u8 *)(data + 0x1f0) = 0x40;
        }
        g_AnmManager->Draw2D((AnmVm *)data);
        if (*(void **)(data + 0x478) != NULL)
        {
            ((void(__fastcall *)(Player *, void *))*(void **)(data + 0x478))(this, data);
        }
    }
}

// FUNCTION: th08 0x451400
#pragma var_order(i, data)
void Player::FUN_00451400()
{
    u8 *data;
    i32 i;

    data = (u8 *)this + 0xbe838;
    for (i = 0; i < 0x80; i++, data += 0x484)
    {
        if (*(i16 *)(data + 0x462) != 2)
        {
            continue;
        }
        if (*(i16 *)(data + 0x1fc) != 0)
        {
            ((AnmVm *)data)->SetZRotation(*(f32 *)(data + 0x450));
        }
        ((AnmVm *)data)->pos.x = g_GameManager.arcadeRegionTopLeftPos.x + *(f32 *)(data + 0x2a4);
        ((AnmVm *)data)->pos.y = g_GameManager.arcadeRegionTopLeftPos.y + *(f32 *)(data + 0x2a8);
        ((AnmVm *)data)->pos.z = 0.2f;
        if (*(i8 *)(data + 0x470) != 0)
        {
            *(u8 *)(data + 0x1f2) = 0xff;
            *(u8 *)(data + 0x1f1) = 0x40;
            *(u8 *)(data + 0x1f0) = 0x40;
        }
        g_AnmManager->DrawPlayerBullet((AnmVm *)data);
    }
}

// FUNCTION: th08 0x44c230
#pragma var_order(player1ShtFile, player, player2ShtFile)
ZunResult Player::RegisterChain(u32 param)
{
    Player *player;
    PlayerRawShtFile *player1ShtFile;
    PlayerRawShtFile *player2ShtFile;

    player = &g_Player;
    if (FUN_0044e0e0())
    {
        player1ShtFile = player->player1ShtFile;
        player2ShtFile = player->player2ShtFile;
    }
    memset(player, 0, 0xe2b30);
    if (FUN_0044e0e0())
    {
        player->player1ShtFile = player1ShtFile;
        player->player2ShtFile = player2ShtFile;
    }
    *(ZunTimer *)((u8 *)player + 0xe2af4) = 0;
    *(u8 *)((u8 *)player + 1) = param;

    player->calcChain = g_Chain.CreateElem((ChainCallback)Player::OnUpdate);
    player->calcChain->arg = player;
    player->calcChain->addedCallback = (ChainLifetimeCallback)Player::AddedCallback;
    player->calcChain->deletedCallback = (ChainLifetimeCallback)Player::DeletedCallback;
    if (g_Chain.AddToCalcChain(player->calcChain, 9) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    player->drawChainHighPrio = g_Chain.CreateElem((ChainCallback)Player::OnDrawHighPrio);
    player->drawChainLowPrio = g_Chain.CreateElem((ChainCallback)Player::OnDrawLowPrio);
    player->drawChainHighPrio->arg = player;
    player->drawChainLowPrio->arg = player;
    g_Chain.AddToDrawChain(player->drawChainHighPrio, 9);
    g_Chain.AddToDrawChain(player->drawChainLowPrio, 10);
    return ZUN_SUCCESS;
}

// STUB: th08 0x44c390
ChainCallbackResult Player::OnUpdate(Player *player)
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x44d530
#pragma var_order(i)
ChainCallbackResult Player::OnDrawHighPrio(Player *player)
{
    i32 i;

    player->FUN_004512f0();
    if (*(i32 *)((u8 *)player + 0xfdc) != 0)
    {
        ((void(__fastcall *)(Player *))*(void **)((u8 *)player + 0x1014 +
                                                  *(i32 *)((u8 *)player + 0xfe0) * 4))(player);
    }
    if (!g_GameManager.showRetryMenu)
    {
        player->playerSprite.pos.x = g_GameManager.arcadeRegionTopLeftPos.x + player->position.x;
        player->playerSprite.pos.y = g_GameManager.arcadeRegionTopLeftPos.y + player->position.y;
        player->playerSprite.pos.z = 0.1f;
        g_AnmManager->DrawNoRotation(&player->playerSprite);
    }
    for (i = 0; (u32)i < 4; i++)
    {
        if (*(void **)((u8 *)player + 0x6fc + i * 0x2f4) != NULL)
        {
            ((void(__fastcall *)(Player *, void *))*(void **)((u8 *)player + 0x6fc + i * 0x2f4))(
                player, (u8 *)player + 0x40c + i * 0x2f4);
        }
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x44d630
ChainCallbackResult Player::OnDrawLowPrio(Player *player)
{
    player->FUN_00451400();
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// STUB: th08 0x44d650
ZunResult Player::AddedCallback(Player *player)
{
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x44dc60
ZunResult Player::DeletedCallback(Player *player)
{
    if (::FUN_004338c0())
    {
        g_AnmManager->ReleaseAnm(5);
        g_AsciiManager.SetGaugeInterrupt(99);
        g_AsciiManager.FUN_00422bb0(0, 99);
        g_AsciiManager.FUN_00422bb0(1, 99);
        g_AsciiManager.FUN_00422bb0(2, 99);
        if (g_Player.player1ShtFile != NULL)
        {
            g_ZunMemory.Free(g_Player.player1ShtFile);
            g_Player.player1ShtFile = NULL;
        }
        if (g_Player.player2ShtFile != NULL)
        {
            g_ZunMemory.Free(g_Player.player2ShtFile);
            g_Player.player2ShtFile = NULL;
        }
    }
    return ZUN_SUCCESS;
}

void Player::CutChain()
{
    g_Chain.Cut(g_Player.calcChain);
    g_Player.calcChain = NULL;
    g_Chain.Cut(g_Player.drawChainHighPrio);
    g_Player.drawChainHighPrio = NULL;
    g_Chain.Cut(g_Player.drawChainLowPrio);
    g_Player.drawChainLowPrio = NULL;
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

