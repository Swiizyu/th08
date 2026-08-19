#include "th_pch.h"

#include "Player.hpp"
#include "AsciiManager.hpp"
#include "Background.hpp"
#include "EffectManager.hpp"
#include "ItemManager.hpp"
#include "SoundPlayer.hpp"
#include "ScreenEffect.hpp"

u32 FUN_004338c0();
u32 FUN_0044e0e0();
void FUN_0044e370(void *data);

namespace th08
{

DIFFABLE_STATIC(i32, g_BackgroundTintActive);

// FUNCTION: th08 0x40bc60
#pragma var_order(result, blue, green, red, factor)
void __fastcall Player::FUN_0040bc60(D3DCOLOR color)
{
    i32 factor;
    i32 red;
    i32 green;
    i32 blue;
    D3DCOLOR result;
    i32 timer = ((ZunTimer *)((u8 *)this + 0xe2af4))->AsFrames();
    i32 duration = *(i32 *)((u8 *)this + 0xe2ae4);

    red = (color >> 16) & 0xff;
    green = (color >> 8) & 0xff;
    blue = color & 0xff;
    if (timer < 60)
    {
        factor = timer;
    }
    else if (timer > duration - 60)
    {
        factor = duration - timer;
    }
    else
    {
        factor = 60;
    }
    red = 128 - (128 - red) * factor / 60;
    green = 128 - (128 - green) * factor / 60;
    blue = 128 - (128 - blue) * factor / 60;
    result = 0x80000000 | (red << 16) | (green << 8) | blue;
    g_Background.FUN_00409160(result);
    g_BackgroundTintActive = 1;
}

// FUNCTION: th08 0x40be30
#pragma var_order(i, entry, bomb)
void __fastcall Player::FUN_0040be30(i32, i32, i32 arg2, i32 duration, i32)
{
    u8 *bomb;
    u8 *entry;
    i32 i;

    bomb = (u8 *)this + 0xfdc;
    *(i32 *)(bomb + 8) = arg2;
    ((ZunTimer *)((u8 *)this + 0xe2af4))->SetCurrent(duration);
    this->playerState = 3;
    this->FUN_0040bf00();
    entry = bomb + 0x4c;
    for (i = 0; i < 0x80; i++, entry += 0x16f0)
    {
        *(i32 *)entry = 0;
    }
    g_ItemManager.AutoCollectAllItems();
    *(Float3 *)(bomb + 0xb784c) = this->position;
}

// FUNCTION: th08 0x40bf00
#pragma var_order(effect)
void Player::FUN_0040bf00()
{
    Effect *effect;

    effect = *(Effect **)((u8 *)this + 0xe2b1c);
    if (effect != NULL)
    {
        effect->active = 0;
    }
    effect = g_EffectManager.SpawnSpecialEffect(23, &this->position, 0, 1, -1);
    ((ZunTimer *)((u8 *)&effect->vm + 0x80))->SetCurrent(0);
    *(ZunTimer *)((u8 *)&effect->vm + 0xd4) = *(ZunTimer *)((u8 *)this + 0xe2af4);
    *(u8 *)((u8 *)&effect->vm + 0xfc) = 0;
    *(f32 *)((u8 *)&effect->vm + 0x268) = effect->vm.scale.x;
    *(f32 *)((u8 *)&effect->vm + 0x26c) = effect->vm.scale.y;
    *(f32 *)((u8 *)&effect->vm + 0x270) = 0.0625f;
    *(f32 *)((u8 *)&effect->vm + 0x274) = 0.0625f;
    *(i32 *)((u8 *)&effect->vm + 0x100) = ((ZunTimer *)((u8 *)this + 0xe2af4))->AsFrames();
    *(f32 *)((u8 *)&effect->vm + 0x14) = -*(f32 *)((u8 *)&effect->vm + 0x14);
    *(u8 *)((u8 *)&effect->vm + 0x1f2) = 0xff;
    *(u8 *)((u8 *)&effect->vm + 0x1f1) = 0x40;
    *(u8 *)((u8 *)&effect->vm + 0x1f0) = 0x40;
    *(Effect **)((u8 *)this + 0xe2b1c) = effect;
}

// FUNCTION: th08 0x40c820
#pragma var_order(vm, i, entry)
void Player::FUN_0040c820()
{
    u8 *entry;
    i32 i;
    AnmVm *vm;

    this->FUN_0040bc60(0x80404040);
    entry = (u8 *)this + 0x1028;
    for (i = 0; i < 16; i++, entry += 0x16f0)
    {
        if (*(i32 *)entry == 0)
        {
            continue;
        }
        vm = (AnmVm *)(entry + 0x1b8);
        vm->pos = *(Float3 *)(entry + 0x14) + vm->pos2;
        vm->pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
        vm->pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
        vm->pos.z = 0.0f;
        g_AnmManager->DrawNoRotation(vm);
    }
}

// FUNCTION: th08 0x410300
#pragma var_order(i, entry)
void Player::FUN_00410300()
{
    u8 *entry;
    i32 i;

    this->FUN_0040bc60(0x80404040);
    entry = (u8 *)this + 0x1028;
    for (i = 0; i < 96; i++, entry += 0x16f0)
    {
        if (*(i32 *)entry == 0)
        {
            continue;
        }
        AnmVm *vm = (AnmVm *)(entry + 0x1b8);
        vm->SetZRotation(*(f32 *)(entry + 0x10));
        vm->pos = *(Float3 *)(entry + 0x14);
        vm->pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
        vm->pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
        vm->pos.z = 0.0f;
        g_AnmManager->Draw2D(vm);
    }
}

// FUNCTION: th08 0x410ac0
#pragma var_order(i, entry)
void Player::FUN_00410ac0()
{
    u8 *entry;
    i32 i;

    this->FUN_0040bc60(0x80202080);
    entry = (u8 *)this + 0x1028;
    for (i = 0; i < 96; i++, entry += 0x16f0)
    {
        if (*(i32 *)entry == 0)
        {
            continue;
        }
        AnmVm *vm = (AnmVm *)(entry + 0x1b8);
        vm->SetZRotation(*(f32 *)(entry + 0x10));
        vm->pos = *(Float3 *)(entry + 0x14);
        vm->pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
        vm->pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
        vm->pos.z = 0.0f;
        g_AnmManager->Draw2D(vm);
    }
}

// FUNCTION: th08 0x40d310
#pragma var_order(color)
void Player::FUN_0040d310()
{
    i32 color;
    ZunTimer *timer = (ZunTimer *)((u8 *)this + 0xff4);

    if (*timer < 60)
    {
        this->FUN_0040bc60(0x80404040);
    }
    else
    {
        color = (timer->AsFrames() - 60) * 176 / 60 + 64;
        this->FUN_0040bc60(0x80000000 | color << 16 | color << 8 | color);
    }
}

// FUNCTION: th08 0x40d950
void Player::FUN_0040d950()
{
    this->FUN_0040bc60(0x80404040);
}

// FUNCTION: th08 0x40f550
void Player::FUN_0040f550()
{
    this->FUN_0040bc60(0x80d02020);
}

// FUNCTION: th08 0x40fcb0
void Player::FUN_0040fcb0()
{
    this->FUN_0040bc60(0x80f00000);
}

// FUNCTION: th08 0x412300
#pragma var_order(color, rect, bomb)
void Player::FUN_00412300()
{
    u8 *bomb = (u8 *)this + 0xfdc;
    ZunRect rect;
    D3DCOLOR color;
    ZunTimer *timer = (ZunTimer *)(bomb + 0x18);

    this->FUN_0040bc60(0x80404040);
    if (*timer >= 70)
    {
        this->FUN_0040bc60(0x80000030);
        rect.left = 32.0f;
        rect.top = 16.0f;
        rect.right = 416.0f;
        rect.bottom = 464.0f;
        color = 0x00ffffff;
        if (*timer < 100)
        {
            color |= 0xff000000;
        }
        else if (*timer < 160)
        {
            color |= (255 - (timer->AsFrames() - 100) * 255 / 60) << 24;
        }
        else
        {
            return;
        }
        ScreenEffect::DrawSquare(&rect, color);
    }
}

// FUNCTION: th08 0x412fa0
#pragma var_order(color, rect, bomb)
void Player::FUN_00412fa0()
{
    u8 *bomb = (u8 *)this + 0xfdc;
    ZunRect rect;
    D3DCOLOR color;
    ZunTimer *timer = (ZunTimer *)(bomb + 0x18);

    this->FUN_0040bc60(0x80404040);
    if (*timer >= 70)
    {
        this->FUN_0040bc60(0x80000030);
        rect.left = 32.0f;
        rect.top = 16.0f;
        rect.right = 416.0f;
        rect.bottom = 464.0f;
        color = 0x00ff0000;
        if (*timer < 100)
        {
            color |= 0xff000000;
        }
        else if (*timer < 160)
        {
            color |= (255 - (timer->AsFrames() - 100) * 255 / 60) << 24;
        }
        else
        {
            return;
        }
        ScreenEffect::DrawSquare(&rect, color);
    }
}

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

// FUNCTION: th08 0x44d180
#pragma var_order(factor)
void Player::FUN_0044d180()
{
    f32 factor;
    ZunTimer *timer = (ZunTimer *)((u8 *)this + 0xe2af4);

    *(i32 *)((u8 *)this + 0xe2a70) = 60;
    factor = 1.0f - (f32)*timer / 30.0f;
    this->playerSprite.scale.y = factor * 2.0f + 1.0f;
    this->playerSprite.scale.x = 1.0f - factor;
    this->playerSprite.FUN_0044e0f0();
    this->horizontalSpeedMultiplier = 1.0f;
    this->verticalSpeedMultiplier = 1.0f;
    this->playerSprite.color1.d3dColor = ((timer->AsFrames() * 255 / 30) << 24) | 0xffffff;
    *(i32 *)((u8 *)this + 0xe2a68) = 0;
    if (timer->AsFrames() >= 30)
    {
        this->playerState = 3;
        this->playerSprite.scale.x = 1.0f;
        this->playerSprite.scale.y = 1.0f;
        this->playerSprite.color1.d3dColor = 0xffffffff;
        this->playerSprite.FUN_0044e120();
        if ((*(u32 *)((u8 *)&g_GameManager + 0x3dbac) & 0x4000) == 0)
        {
            timer->SetCurrent(240);
        }
    }
}

// FUNCTION: th08 0x44d420
void Player::FUN_0044d420()
{
    *(Float3 *)((u8 *)this + 0xe2aa4) = Float3(-999.0f, -999.0f, 0.0f);
    *(Float3 *)((u8 *)this + 0xe2ab0) = Float3(-999.0f, -999.0f, 0.0f);
    *(i32 *)((u8 *)this + 0xe2ac0) = 0;
}

// FUNCTION: th08 0x44c5b0
#pragma var_order(i, collision)
void Player::FUN_0044c5b0()
{
    PlayerCollisionObject *collision;
    i32 i;

    collision = (PlayerCollisionObject *)((u8 *)this + 0xb8834);
    for (i = 0; i < 0x180; i++, collision++)
    {
        if (*(i32 *)((u8 *)collision + 0x24) < 0)
        {
            continue;
        }
        (*(i32 *)((u8 *)collision + 0x24))--;
        *(f32 *)((u8 *)collision + 0x8) += *(f32 *)((u8 *)collision + 0xc);
        *(f32 *)((u8 *)collision + 0x10) += *(f32 *)((u8 *)collision + 0x18);
        *(f32 *)((u8 *)collision + 0x14) += *(f32 *)((u8 *)collision + 0x1c);
        if (*(i32 *)((u8 *)collision + 0x24) <= 0)
        {
            ((PlayerUnkStruct0x40 *)collision)->FUN_0044e350();
        }
    }
}

// FUNCTION: th08 0x44de60
#pragma var_order(i, collision)
void *Player::FUN_0044de60(Float2 *position, f32 width, f32 height, i32 arg4, i32 arg5)
{
    PlayerCollisionObject *collision = (PlayerCollisionObject *)((u8 *)this + 0xbb834);
    i32 i;
    for (i = 0; i < 0xbf; i++, collision++)
    {
        if (!collision->isActive)
        {
            break;
        }
    }
    ::FUN_0044e370(collision);
    collision->isActive = 1;
    collision->position.x = position->x;
    collision->position.y = position->y;
    collision->width = width;
    collision->height = height;
    *(i32 *)((u8 *)collision + 0x24) = arg5;
    *(i32 *)((u8 *)collision + 0x28) = arg4;
    return collision;
}

// FUNCTION: th08 0x44df00
#pragma var_order(i, collision)
void *Player::FUN_0044df00(Float2 *position, f32 arg2, f32 arg3, i32 arg4, i32 arg5)
{
    PlayerCollisionObject *collision = (PlayerCollisionObject *)((u8 *)this + 0xbb834);
    i32 i;
    for (i = 0; i < 0xbf; i++, collision++)
    {
        if (!collision->isActive)
        {
            break;
        }
    }
    ::FUN_0044e370(collision);
    collision->isActive = 1;
    collision->position.x = position->x;
    collision->position.y = position->y;
    *(f32 *)((u8 *)collision + 0x8) = arg2;
    *(f32 *)((u8 *)collision + 0xc) = arg3;
    *(i32 *)((u8 *)collision + 0x24) = arg4;
    *(i32 *)((u8 *)collision + 0x28) = arg5;
    return collision;
}

// FUNCTION: th08 0x44dfa0
#pragma var_order(i, collision)
void *Player::FUN_0044dfa0(Float2 *position, f32 width, f32 height, i32 arg4, i32 arg5)
{
    PlayerCollisionObject *collision = (PlayerCollisionObject *)((u8 *)this + 0xb8834);
    i32 i;
    for (i = 0; i < 0xbf; i++, collision++)
    {
        if (!collision->isActive)
        {
            break;
        }
    }
    ::FUN_0044e370(collision);
    collision->isActive = 1;
    collision->position.x = position->x;
    collision->position.y = position->y;
    collision->width = width;
    collision->height = height;
    *(i32 *)((u8 *)collision + 0x24) = arg5;
    *(i32 *)((u8 *)collision + 0x2c) = arg4;
    return collision;
}

// FUNCTION: th08 0x44e040
#pragma var_order(i, collision)
void *Player::FUN_0044e040(Float2 *position, f32 arg2, f32 arg3, i32 arg4, i32 arg5)
{
    PlayerCollisionObject *collision = (PlayerCollisionObject *)((u8 *)this + 0xb8834);
    i32 i;
    for (i = 0; i < 0xbf; i++, collision++)
    {
        if (!collision->isActive)
        {
            break;
        }
    }
    ::FUN_0044e370(collision);
    collision->isActive = 1;
    collision->position.x = position->x;
    collision->position.y = position->y;
    *(f32 *)((u8 *)collision + 0x8) = arg2;
    *(f32 *)((u8 *)collision + 0xc) = arg3;
    *(i32 *)((u8 *)collision + 0x24) = arg5;
    *(i32 *)((u8 *)collision + 0x2c) = arg4;
    return collision;
}

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

// FUNCTION: th08 0x44a230
#pragma var_order(maxPosition, minPosition)
i32 Player::FUN_0044a230(Float3 *position, Float3 *hitbox)
{
    Float3 minPosition;
    Float3 maxPosition;

    *(i32 *)((u8 *)this + 0xe2a90) = 6;
    if (this->FUN_00449ff0(position, hitbox))
    {
        return 2;
    }
    minPosition = *position - *hitbox / 2.0f;
    maxPosition = *position + *hitbox / 2.0f;
    if (this->unk0x38C.x >= maxPosition.x || this->unk0x38C.y >= maxPosition.y ||
        this->unk0x398.x <= minPosition.x || this->unk0x398.y <= minPosition.y)
    {
        return 0;
    }
    if (this->playerState != PLAYER_STATE_ALIVE)
    {
        return 1;
    }
    g_GameManager.RandomizeAntiTamper();
    this->Die();
    return 1;
}

// FUNCTION: th08 0x44a360
#pragma var_order(maxPosition, minPosition)
i32 Player::FUN_0044a360(Float3 *position, Float3 *hitbox)
{
    Float3 minPosition;
    Float3 maxPosition;

    *(i32 *)((u8 *)this + 0xe2a90) = 6;
    minPosition = *position - *hitbox / 2.0f;
    maxPosition = *position + *hitbox / 2.0f;
    if (this->unk0x38C.x >= maxPosition.x || this->unk0x38C.y >= maxPosition.y ||
        this->unk0x398.x <= minPosition.x || this->unk0x398.y <= minPosition.y)
    {
        return 0;
    }
    if (this->playerState != PLAYER_STATE_ALIVE)
    {
        return 1;
    }
    g_GameManager.RandomizeAntiTamper();
    this->Die();
    return 1;
}

// FUNCTION: th08 0x44a470
#pragma var_order(maxPosition, minPosition)
i32 Player::FUN_0044a470(Float3 *position, Float3 *hitbox)
{
    Float3 minPosition;
    Float3 maxPosition;

    *(i32 *)((u8 *)this + 0xe2a90) = 6;
    if (this->FUN_00449ff0(position, hitbox))
    {
        return 2;
    }
    minPosition = *position - *hitbox / 2.0f;
    maxPosition = *position + *hitbox / 2.0f;
    minPosition.x -= 20.0f;
    minPosition.y -= 20.0f;
    maxPosition.x += 20.0f;
    maxPosition.y += 20.0f;
    if (this->playerState == PLAYER_STATE_DEAD || this->playerState == PLAYER_STATE_SPAWNING)
    {
        return 0;
    }
    if (this->unk0x3A4.x >= maxPosition.x || this->unk0x3A4.y >= maxPosition.y ||
        this->unk0x3B0.x <= minPosition.x || this->unk0x3B0.y <= minPosition.y)
    {
        return 0;
    }
    this->ScoreGraze(position, 0);
    return 1;
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
    g_GameManager.RandomizeAntiTamper();
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

// FUNCTION: th08 0x44c1b0
#pragma var_order(deltaY, deltaX)
f32 Player::AngleToPlayer(Float3 *position)
{
    f32 deltaX;
    f32 deltaY;

    deltaX = this->position.x - position->x;
    deltaY = this->position.y - position->y;
    if (deltaY == 0.0f && deltaX == 0.0f)
    {
        return 1.5707964f;
    }
    return atan2f(deltaY, deltaX);
}

// FUNCTION: th08 0x44e8d0
#pragma var_order(delta, targetPosition)
void __fastcall Player::FUN_0044e8d0(Effect *effect)
{
    Float3 targetPosition;
    Float3 delta;

    targetPosition = *(Float3 *)((u8 *)*(void **)((u8 *)this + 0xe2abc) + 0x2d88);
    targetPosition.y += 32.0f;
    if (targetPosition.y < 32.0f)
    {
        targetPosition.y = 32.0f;
    }
    delta = targetPosition - effect->position;
    f32 length = delta.FUN_0040b4c0();
    if (length != 0.0f)
    {
        delta *= 16.0f / length;
    }
    effect->velocity += (delta - effect->velocity) * 0.2f;
    effect->position += effect->velocity;
    if (fabsf(effect->velocity.x) <= 0.05f)
    {
        effect->velocity.x = 0.0f;
    }
}

// FUNCTION: th08 0x44ea40
#pragma var_order(state)
i32 __fastcall Player::FUN_0044ea40(PlayerUnkStruct0x2ec *data)
{
    i32 state = *(i32 *)((u8 *)data + 0x2c8);
    switch (state)
    {
    case 1:
        this->playerAnm->SetAndExecuteScriptIdx(&data->vm, 29);
        *(i32 *)((u8 *)data + 0x2c8) = 2;
    case 2:
        if (*(i32 *)((u8 *)this + 0xfdc) != 0)
        {
            break;
        }
        data->position0 = this->position;
        data->position0.y -= 32.0f;
        break;
    case 3:
        data->position0 = this->position;
        data->position0.y -= 32.0f;
        if (data->timer == 0)
        {
            data->vm.SetInterrupt(5);
        }
        if (data->timer > 16)
        {
            *(i32 *)((u8 *)data + 0x2c8) = 0;
            *(i32 *)((u8 *)data + 0x2ec) = 0;
            *(i32 *)((u8 *)data + 0x2f0) = 0;
        }
        break;
    }
    return 0;
}

struct PlayerShotData
{
    i16 interval;
    i16 phase;
    f32 offsetX;
    f32 offsetY;
    f32 hitboxX;
    f32 hitboxY;
    f32 angle;
    f32 speed;
    i16 damage;
    i16 unknown0x1e;
    i16 option;
    i16 unknown0x22;
    i16 anmScript;
    i16 sound;
};
C_ASSERT(sizeof(PlayerShotData) == 0x28);

// FUNCTION: th08 0x44fb70
#pragma var_order(positionComponent, positionComponent2)
void __fastcall Player::FUN_0044fb70(u8 *data, void *shotData)
{
    PlayerShotData *shot = (PlayerShotData *)shotData;
    Float3 *position = (Float3 *)(data + 0x2a4);

    if (shot->option == 0)
    {
        *position = this->position;
    }
    else
    {
        *position = *(Float3 *)((u8 *)this + 0x6b0 + (shot->option - 1) * 0x2f4);
    }
    position->x += shot->offsetX;
    position->y += shot->offsetY;
    position->z = 0.495f;
    *(f32 *)(data + 0x430) = shot->hitboxX;
    *(f32 *)(data + 0x434) = shot->hitboxY;
    *(f32 *)(data + 0x438) = 1.0f;
    *(f32 *)(data + 0x450) = shot->angle;
    *(f32 *)(data + 0x44c) = shot->speed;
    ((Float3 *)(data + 0x43c))->FromAngleMagnitude(shot->angle, shot->speed);
    ((ZunTimer *)(data + 0x454))->SetCurrent(0);
    *(u8 *)(data + 0x46c) = *(u8 *)((u8 *)this + 3);
    *(i16 *)(data + 0x464) = shot->unknown0x22;
    *(i16 *)(data + 0x460) = shot->damage;
    *(i16 *)(data + 0x46e) = shot->anmScript;
    if (shot->sound >= 0)
    {
        g_SoundPlayer.PlaySoundPositionedByIdx((SoundIdx)shot->sound, this->position.x);
    }
    this->playerAnm->SetAndExecuteScriptIdx((AnmVm *)data, shot->anmScript + 10);
    *(u8 *)(data + 0x470) = 0;
    if (g_GameManager.GaugeIsExtremelyYoukai() && shot->unknown0x1e > 0)
    {
        *(u8 *)(data + 0x470) = 1;
    }
}

// FUNCTION: th08 0x44fd80
i32 __fastcall Player::FUN_0044fd80(u8 *data, i32 frame, void *shotData)
{
    PlayerShotData *shot = (PlayerShotData *)shotData;
    if (frame % shot->interval == shot->phase)
    {
        this->FUN_0044fb70(data, shot);
        return 1;
    }
    return 0;
}

// FUNCTION: th08 0x44fdd0
i32 __fastcall Player::FUN_0044fdd0(u8 *data, i32 frame, void *shotData)
{
    PlayerShotData *shot = (PlayerShotData *)shotData;
    if (*(i32 *)((u8 *)this + 0xfdc) == 0 && frame % shot->interval == shot->phase)
    {
        this->FUN_0044fb70(data, shot);
        return 1;
    }
    return 0;
}

// FUNCTION: th08 0x44ffa0
#pragma var_order(speed, angle)
i32 __fastcall Player::FUN_0044ffa0(u8 *data, i32 frame, void *shotData)
{
    PlayerShotData *shot = (PlayerShotData *)shotData;
    f32 angle;
    f32 speed;

    if (frame % shot->interval != shot->phase)
    {
        return 0;
    }
    this->FUN_0044fb70(data, shot);
    if (*(f32 *)((u8 *)this + 0xe2ab0) >= -100.0f)
    {
        angle = AddNormalizeAngle(atan2f(*(f32 *)((u8 *)this + 0xe2ab4) - *(f32 *)(data + 0x2a8),
                                         *(f32 *)((u8 *)this + 0xe2ab0) - *(f32 *)(data + 0x2a4)),
                                  shot->angle + ZUN_PI / 2.0f);
        speed = shot->speed * 1.5f;
        ((Float3 *)(data + 0x43c))->FromAngleMagnitude(angle, speed);
        *(f32 *)(data + 0x450) = angle;
    }
    return 1;
}

// FUNCTION: th08 0x450080
#pragma var_order(speed, angle)
i32 __fastcall Player::FUN_00450080(u8 *data, i32 frame, void *shotData)
{
    PlayerShotData *shot = (PlayerShotData *)shotData;
    f32 angle;
    f32 speed;

    if (frame % shot->interval != shot->phase)
    {
        return 0;
    }
    this->FUN_0044fb70(data, shot);
    angle = AddNormalizeAngle(*(f32 *)((u8 *)this + 0xe2b0c), shot->angle + ZUN_PI / 2.0f);
    speed = shot->speed;
    ((Float3 *)(data + 0x43c))->FromAngleMagnitude(angle, speed);
    *(f32 *)(data + 0x450) = angle;
    return 1;
}

// FUNCTION: th08 0x450110
#pragma var_order(speed, angle)
i32 __fastcall Player::FUN_00450110(u8 *data, i32 frame, void *shotData)
{
    PlayerShotData *shot = (PlayerShotData *)shotData;
    f32 angle;
    f32 speed;

    if (*(i32 *)((u8 *)this + 0xfdc) != 0 || frame % shot->interval != shot->phase)
    {
        return 0;
    }
    this->FUN_0044fb70(data, shot);
    angle = AddNormalizeAngle(*(f32 *)((u8 *)this + 0xcd0), shot->angle);
    speed = shot->speed;
    ((Float3 *)(data + 0x43c))->FromAngleMagnitude(angle, speed);
    *(f32 *)(data + 0x450) = angle;
    return 1;
}

// FUNCTION: th08 0x4501b0
i32 __fastcall Player::FUN_004501b0(u8 *data, i32 frame, void *shotData)
{
    PlayerShotData *shot = (PlayerShotData *)shotData;
    if (frame % shot->interval != shot->phase)
    {
        return 0;
    }
    this->FUN_0044fb70(data, shot);
    *(f32 *)(data + 0x450) = g_Rng.GetRandomF32Signed() * ZUN_PI / 48.0f - ZUN_PI / 2.0f;
    ((Float3 *)(data + 0x43c))->FromAngleMagnitude(*(f32 *)(data + 0x450), shot->speed);
    return 1;
}

// FUNCTION: th08 0x450240
#pragma var_order(speed, angle)
i32 __fastcall Player::FUN_00450240(u8 *data, i32 frame, void *shotData)
{
    PlayerShotData *shot = (PlayerShotData *)shotData;
    f32 angle;
    f32 speed;

    if (frame % shot->interval != shot->phase)
    {
        return 0;
    }
    this->FUN_0044fb70(data, shot);
    if (*(void **)((u8 *)this + 0xe2abc) != NULL)
    {
        u8 *enemy = *(u8 **)((u8 *)this + 0xe2abc);
        angle = AddNormalizeAngle(atan2f(*(f32 *)(enemy + 0x2d38) - *(f32 *)(data + 0x2a8),
                                         *(f32 *)(enemy + 0x2d34) - *(f32 *)(data + 0x2a4)),
                                  shot->angle + ZUN_PI / 2.0f);
        speed = shot->speed * 1.5f;
        ((Float3 *)(data + 0x43c))->FromAngleMagnitude(angle, speed);
        *(f32 *)(data + 0x450) = angle;
    }
    return 1;
}

// FUNCTION: th08 0x450ee0
#pragma var_order(spawnPosition)
i32 __fastcall Player::FUN_00450ee0(Effect *effect, Float3 *position)
{
    Float3 spawnPosition;

    (*(u8 *)((u8 *)this + 0xe2a94))++;
    if ((*(u8 *)((u8 *)this + 0xe2a94) & 7) == 0)
    {
        spawnPosition = *position;
        spawnPosition.x = effect->position.x;
        g_EffectManager.SpawnEffect(5, &spawnPosition, 1, -1);
    }
    return 0;
}

// FUNCTION: th08 0x451ce0
void __fastcall FUN_00451ce0(Float3 *minimum, Float3 *maximum, Float3 *center, Float3 *size)
{
    minimum->x = center->x - size->x * 0.5f;
    minimum->y = center->y - size->y * 0.5f;
    maximum->x = center->x + size->x * 0.5f;
    maximum->y = center->y + size->y * 0.5f;
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

