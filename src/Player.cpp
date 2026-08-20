#include "th_pch.h"

#include "Player.hpp"
#include "AsciiManager.hpp"
#include "Background.hpp"
#include "BulletManager.hpp"
#include "EffectManager.hpp"
#include "ItemManager.hpp"
#include "SoundPlayer.hpp"
#include "ScreenEffect.hpp"
#include "Spellcard.hpp"

#include <stdio.h>

u32 FUN_004338b0();
u32 FUN_004338c0();
u32 FUN_0044e0e0();
void FUN_0044e370(void *data);

namespace th08 { i32 __fastcall FUN_00450580(void *, void *); }

namespace th08
{

DIFFABLE_STATIC(i32, g_BackgroundTintActive);

// FUNCTION: th08 0x40bc60
#pragma var_order(color2, result)
void __fastcall Player::FUN_0040bc60(D3DCOLOR color)
{
    D3DCOLOR result;
    D3DCOLOR color2 = color;

    if (*(ZunTimer *)((u8 *)this + 0xff4) < 60)
    {
        ((u8 *)&result)[2] =
            0x80 - ((0x80 - ((u8 *)&color2)[2]) * (i32)*(ZunTimer *)((u8 *)this + 0xff4) / 60);
        ((u8 *)&result)[1] =
            0x80 - ((0x80 - ((u8 *)&color2)[1]) * (i32)*(ZunTimer *)((u8 *)this + 0xff4) / 60);
        ((u8 *)&result)[0] =
            0x80 - ((0x80 - ((u8 *)&color2)[0]) * (i32)*(ZunTimer *)((u8 *)this + 0xff4) / 60);
    }
    else if (*(ZunTimer *)((u8 *)this + 0xff4) >= *(i32 *)((u8 *)this + 0xfe4) - 60)
    {
        ((u8 *)&result)[2] = 0x80 - ((0x80 - ((u8 *)&color2)[2]) *
                                     (*(i32 *)((u8 *)this + 0xfe4) - (i32)*(ZunTimer *)((u8 *)this + 0xff4)) / 60);
        ((u8 *)&result)[1] = 0x80 - ((0x80 - ((u8 *)&color2)[1]) *
                                     (*(i32 *)((u8 *)this + 0xfe4) - (i32)*(ZunTimer *)((u8 *)this + 0xff4)) / 60);
        ((u8 *)&result)[0] = 0x80 - ((0x80 - ((u8 *)&color2)[0]) *
                                     (*(i32 *)((u8 *)this + 0xfe4) - (i32)*(ZunTimer *)((u8 *)this + 0xff4)) / 60);
    }
    else
    {
        result = color2;
    }
    ((u8 *)&result)[3] = 0x80;
    g_AnmManager->SetMixColorDefault();
    g_Background.FUN_00409160(result);
    g_BackgroundTintActive = 1;
}

// FUNCTION: th08 0x40be30
#pragma var_order(i, bomb, entry)
void __fastcall Player::FUN_0040be30(i32 a0, const char *a1, i32 arg2, i32 duration, i32 a4)
{
    u8 *bomb;
    u8 *entry;
    u32 i;

    bomb = (u8 *)this + 0xfdc;
    g_Spellcard.FUN_00415d60(a0, a1, a4);
    *(i32 *)(bomb + 8) = arg2;
    *(ZunTimer *)((u8 *)this + 0xe2af4) = duration;
    this->playerState = 3;
    this->FUN_0040bf00();
    for (i = 0, entry = bomb + 0x4c; i < 0x80; i++, entry += 0x16f0)
    {
        *(i32 *)entry = 0;
    }
    g_ItemManager.AutoCollectAllItems();
    *(Float3 *)(bomb + 0xb784c) = this->position;
}

static void UpdateBombPattern(Player *player, i32 effectId, D3DCOLOR color, i32 duration, i32 interval)
{
    ZunTimer *timer = (ZunTimer *)((u8 *)player + 0xff4);
    if (timer->current == 0)
    {
        player->FUN_0040be30(0, (const char *)200, duration, duration, 0);
        player->FUN_0040bc60(color);
        g_EffectManager.SpawnEffect(effectId, &player->position, 1, color);
    }
    if (interval > 0 && timer->current % interval == 0)
    {
        Float3 position = player->position;
        position.x += g_Rng.GetRandomF32InRange(96.0f) - 48.0f;
        position.y += g_Rng.GetRandomF32InRange(96.0f) - 48.0f;
        g_EffectManager.SpawnEffect(effectId, &position, 1, color);
        g_BulletManager.RemoveAllBullets(0);
    }
    timer->Tick();
    if (timer->current >= duration)
    {
        player->FUN_0040bc60(0x80404040);
        player->playerState = PLAYER_STATE_ALIVE;
    }
}

// FUNCTION: th08 0x40c010
#pragma var_order(i, bomb, entry, angle, v2, dy, dx, t, target, gfx3)
void Player::FUN_0040c010()
{
    i32 i;
    u8 *bomb;
    u8 *entry;
    f32 angle;

    bomb = (u8 *)this + 0xfdc;

    if (((ZunTimer *)(bomb + 0x18))->FUN_0040d3d0() && *(ZunTimer *)(bomb + 0x18) == 0)
    {
        this->FUN_0040be30(0, "\x97\xec\x95\x84\x81\x75\x96\xb2\x91\x7a\x96\xad\x8e\xec\x81\x76", 0xc8, 0x104, 0);
        g_EffectManager.SpawnEffect(12, &this->position, 1, 0xff4040ff);

        angle = -ZUN_PI;
        for (entry = bomb + 0x4c, i = 0; i < 0x10; i++, entry += 0x16f0)
        {
            this->playerAnm->SetAndExecuteScriptIdx((AnmVm *)(bomb + i * 0x16f0 + 0x204), 0x13);
            *(f32 *)(bomb + i * 0x16f0 + 0x5c) = angle;
            angle += ZUN_PI / 8.0f;

            *(Float3 *)(entry + 0x14) = this->position;
            *(Float3 *)(entry + 0x20) = *(Float3 *)(entry + 0x14);
            *(i32 *)(entry + 0x8) = 0;
            *(i32 *)entry = 1;
            *(void **)(entry + 0x16ec) = this->FUN_0044df00((Float2 *)&this->position, 96.0f, 0.0f, 0xc8, 6);
            *(void **)(entry + 0x16e8) = this->FUN_0044e040((Float2 *)(entry + 0x14), 64.0f, 0.0f, 5, 0xc8);
            *(i32 *)((*(u8 **)(entry + 0x16e8)) + 0x38) = 2;
            *(i32 *)((*(u8 **)(entry + 0x16e8)) + 0x34) = 0xc8;
            *(u8 *)((*(u8 **)(entry + 0x16e8)) + 0x3d) = 1;
        }

        g_SoundPlayer.PlaySoundByIdx((SoundIdx)13, 0);
    }

    if (*(ZunTimer *)(bomb + 0x18) < 40)
    {
        Float3 v2;

        for (entry = bomb + 0x4c, i = 0; i < 0x10; i++, entry += 0x16f0)
        {
            *(f32 *)(entry + 0x10) =
                AddNormalizeAngle(*(f32 *)(entry + 0x10), (i & 1) ? (ZUN_PI / 60.0f) : -(ZUN_PI / 60.0f));
            v2 = *(Float3 *)(entry + 0x14);
            *(f32 *)(entry + 0x14) = sinf(*(f32 *)(entry + 0x10)) * *(f32 *)(entry + 8) + *(f32 *)(entry + 0x20);
            *(f32 *)(entry + 0x18) = cosf(*(f32 *)(entry + 0x10)) * *(f32 *)(entry + 8) + *(f32 *)(entry + 0x24);
            *(f32 *)(entry + 8) += 3.2f;
            *(Float3 *)(entry + 0x1a0) = *(Float3 *)(entry + 0x14) - v2;
        }
    }
    else
    {
        f32 dy;
        f32 dx;
        f32 t;
        Float3 target;

        if (*(ZunTimer *)(bomb + 0x18) == 40)
        {
            for (entry = bomb + 0x4c, i = 0; i < 0x10; i++, entry += 0x16f0)
            {
                *(f32 *)(entry + 0xc) = sqrtf(*(f32 *)(entry + 0x1a0) * *(f32 *)(entry + 0x1a0) +
                                              *(f32 *)(entry + 0x1a4) * *(f32 *)(entry + 0x1a4));
                *(f32 *)(entry + 0x10) = FUN_0040c7b0(*(f32 *)(entry + 0x1a0), *(f32 *)(entry + 0x1a4));
                *(i32 *)(entry + 4) = 0;
                *(f32 *)(entry + 8) = 8.0f;
            }
        }

        for (i = 0, entry = bomb + 0x4c; i < 0x10; i++, entry += 0x16f0)
        {
            if (*(i32 *)entry == 0)
            {
                continue;
            }

            if (*(i32 *)entry == 1 && ((ZunTimer *)(bomb + 0x18))->FUN_0040d3d0())
            {
                void *gfx3;

                if (*(f32 *)((u8 *)this + 0xe2aa4) > -100.0f)
                {
                    target = *(Float3 *)((u8 *)this + 0xe2aa4);
                }
                else
                {
                    target = this->position;
                }

                dx = ((f32 *)target)[0] - *(f32 *)(entry + 0x14);
                dy = ((f32 *)target)[1] - *(f32 *)(entry + 0x18);

                t = sqrtf(dx * dx + dy * dy) / (*(f32 *)(entry + 8) / 8.0f);
                if (t < 1.0f)
                {
                    t = 1.0f;
                }

                dx = dx / t + *(f32 *)(entry + 0x1a0);
                dy = dy / t + *(f32 *)(entry + 0x1a4);

                t = sqrtf(dx * dx + dy * dy);
                *(f32 *)(entry + 8) = (t > 10.0f) ? 10.0f : t;
                if (*(f32 *)(entry + 8) < 1.0f)
                {
                    *(f32 *)(entry + 8) = 1.0f;
                }

                *(f32 *)(entry + 0x1a0) = dx * *(f32 *)(entry + 8) / t;
                *(f32 *)(entry + 0x1a4) = dy * *(f32 *)(entry + 8) / t;

                this->FUN_0044df00((Float2 *)(entry + 0x14), 128.0f, 0.0f, 0, 6);

                if (*(i32 *)((*(u8 **)(entry + 0x16e8)) + 0x30) >= *(i32 *)((*(u8 **)(entry + 0x16e8)) + 0x34) ||
                    *(ZunTimer *)(bomb + 0x18) >= *(i32 *)(bomb + 8) - 30)
                {
                    *(u8 *)((*(u8 **)(entry + 0x16ec)) + 0x3c) = 0;
                    *(u8 *)((*(u8 **)(entry + 0x16e8)) + 0x3c) = 0;

                    this->FUN_0044df00((Float2 *)&this->position, 64.0f, 4.2666669f, 30, 6);
                    gfx3 = this->FUN_0044e040((Float2 *)(entry + 0x14), 64.0f, 12.8f, 0x1f4, 0xc);
                    *(i32 *)((u8 *)gfx3 + 0x38) = 4;
                    *(i32 *)((u8 *)gfx3 + 0x34) = 0;

                    g_EffectManager.SpawnEffect(6, (Float3 *)(entry + 0x14), 8, -1);

                    *(i32 *)entry = 2;
                    *(u16 *)(entry + 0x3b6) = 1;

                    *(Float3 *)(entry + 0x1a0) / 8.0f;

                    g_SoundPlayer.PlaySoundPositionedByIdx((SoundIdx)15, *(f32 *)(entry + 0x14));
                    ScreenEffect::RegisterChain((ScreenEffectType)1, 16, 8, 0, 0, 21);
                }
            }

            *(f32 *)(entry + 0x14) += g_Supervisor.framerateMultiplier * *(f32 *)(entry + 0x1a0);
            *(f32 *)(entry + 0x18) += g_Supervisor.framerateMultiplier * *(f32 *)(entry + 0x1a4);
        }
    }

    for (i = 0, entry = bomb + 0x4c; i < 0x10; i++, entry += 0x16f0)
    {
        if (*(i32 *)entry == 0)
        {
            continue;
        }

        if (*(i32 *)entry == 1)
        {
            *(f32 *)*(i32 *)(entry + 0x16ec) = *(f32 *)(entry + 0x14);
            *(f32 *)(*(i32 *)(entry + 0x16ec) + 4) = *(f32 *)(entry + 0x18);
            *(f32 *)*(i32 *)(entry + 0x16e8) = *(f32 *)(entry + 0x14);
            *(f32 *)(*(i32 *)(entry + 0x16e8) + 4) = *(f32 *)(entry + 0x18);
        }
        else if (*(i32 *)entry != 0)
        {
            if (((ZunTimer *)(bomb + 0x18))->FUN_0040d3d0())
            {
                *(i32 *)(entry + 4) += 1;
                if (*(i32 *)(entry + 4) >= 30)
                {
                    *(i32 *)entry = 0;
                }
            }
        }

        g_AnmManager->ExecuteScript((AnmVm *)(entry + 0x1b8));
    }
}

// FUNCTION: th08 0x40c910
void Player::FUN_0040c910()
{
    UpdateBombPattern(this, 12, 0xffff4040, 260, 16);
}

// FUNCTION: th08 0x40d430
#pragma var_order(bomb, entry, t, v, effect, bombGfx, effect1, effect2, effect3, effect4)
void Player::FUN_0040d430()
{
    u8 *bomb;
    u8 *entry;
    f32 t;

    bomb = (u8 *)this + 0xfdc;
    entry = bomb + 0x4c;

    if (((ZunTimer *)(bomb + 0x18))->FUN_0040d3d0() && *(ZunTimer *)(bomb + 0x18) == 0)
    {
        this->FUN_0040be30(1, "\x96\x82\x95\x84\x81\x75\x83\x41\x81\x5b\x83\x65\x83\x42\x83\x74\x83\x8b\x83\x54\x83\x4e\x83\x8a\x83\x74\x83\x40\x83\x43\x83\x58\x81\x76", 0xd2, 0xfa, 0);

        g_SoundPlayer.PlaySoundByIdx((SoundIdx)13, 0);
        *(Float3 *)(entry + 0x14) = *(Float3 *)((u8 *)this + 0x6b0);
        g_SoundPlayer.PlaySoundByIdx((SoundIdx)6, 0);
    }

    if (*(ZunTimer *)(bomb + 0x18) < 60)
    {
        Float3 v(192.0f, 224.0f, 0.0f);

        t = (f32)*(ZunTimer *)(bomb + 0x18) / 60.0f;
        t = t * t;

        *(Float3 *)((u8 *)this + 0x6b0) = (v - *(Float3 *)(entry + 0x14)) * t + *(Float3 *)(entry + 0x14);

        *(f32 *)((u8 *)this + 0x414) += (-ZUN_PI) / 10.0f;

        this->FUN_0044df00((Float2 *)((u8 *)this + 0x6b0), 32.0f, 0.0f, 0, 6);
        this->FUN_0044e040((Float2 *)((u8 *)this + 0x6b0), 32.0f, 0.0f, 40, 0);
        return;
    }

    *(f32 *)((u8 *)this + 0x414) = 0.0f;
    *(f32 *)((u8 *)this + 0x6b0) = 192.0f;
    *(f32 *)((u8 *)this + 0x6b4) = 224.0f;

    if (*(ZunTimer *)(bomb + 0x18) >= 150)
    {
        *(u8 *)((u8 *)this + 0x5ff) = 0;
    }

    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(60))
    {
        g_EffectManager.SpawnEffect(40, (Float3 *)((u8 *)this + 0x6b0), 1, -1);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(64))
    {
        g_EffectManager.SpawnEffect(40, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffffd0d0);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(68))
    {
        g_EffectManager.SpawnEffect(40, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffffb0b0);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(72))
    {
        g_EffectManager.SpawnEffect(40, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffff8080);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(76))
    {
        g_EffectManager.SpawnEffect(40, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffff4040);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(90))
    {
        Effect *effect;

        g_SoundPlayer.PlaySoundByIdx((SoundIdx)15, 0);
        effect = g_EffectManager.SpawnEffect(42, (Float3 *)((u8 *)this + 0x6b0), 1, -1);
        effect = g_EffectManager.SpawnEffect(43, (Float3 *)((u8 *)this + 0x6b0), 1, -1);
        effect = g_EffectManager.SpawnEffect(44, (Float3 *)((u8 *)this + 0x6b0), 1, -1);
        this->FUN_0044df00((Float2 *)((u8 *)this + 0x6b0), 1.0f, 5.0f, 110, 6);
        {
            void *bombGfx;

            bombGfx = this->FUN_0044e040((Float2 *)((u8 *)this + 0x6b0), 1.0f, 5.0f, 70, 110);
            *(i32 *)((u8 *)bombGfx + 0x38) = 5;
        }
        ScreenEffect::RegisterChain((ScreenEffectType)1, 24, 8, 0, 0, 21);
        ScreenEffect::RegisterChain((ScreenEffectType)3, 8, 1, 0x8fffffff, 0, 21);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(100))
    {
        Effect *effect1;

        effect1 = g_EffectManager.SpawnEffect(45, (Float3 *)((u8 *)this + 0x6b0), 1, -1);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(110))
    {
        Effect *effect2;

        effect2 = g_EffectManager.SpawnEffect(45, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffffd0d0);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(120))
    {
        Effect *effect3;

        effect3 = g_EffectManager.SpawnEffect(45, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffff8080);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(130))
    {
        Effect *effect4;

        effect4 = g_EffectManager.SpawnEffect(45, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffff0000);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(150))
    {
        ScreenEffect::RegisterChain((ScreenEffectType)3, 8, 1, 0x8fffffff, 0, 21);
        ScreenEffect::RegisterChain((ScreenEffectType)1, 24, 8, 0, 0, 21);
        g_SoundPlayer.PlaySoundByIdx((SoundIdx)0x19, 0);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(0xd1))
    {
        *(i32 *)((u8 *)this + 0x6d4) = 1;
        *(ZunTimer *)((u8 *)this + 0x6ec) = 0;
    }
}

// FUNCTION: th08 0x40d970
#pragma var_order(bomb, entry, t, v, effect, bombGfx, pos, effect1, effect2, effect3, effect4)
void Player::FUN_0040d970()
{
    u8 *bomb;
    u8 *entry;
    f32 t;

    bomb = (u8 *)this + 0xfdc;
    entry = bomb + 0x4c;

    if (((ZunTimer *)(bomb + 0x18))->FUN_0040d3d0() && *(ZunTimer *)(bomb + 0x18) == 0)
    {
        this->FUN_0040be30(1, "\x96\x82\x91\x80\x81\x75\x83\x8a\x83\x5e\x81\x5b\x83\x93\x83\x43\x83\x69\x83\x6a\x83\x81\x83\x67\x83\x6c\x83\x58\x81\x76", 0xe6, 0x118, 1);

        g_SoundPlayer.PlaySoundByIdx((SoundIdx)13, 0);
        *(Float3 *)(entry + 0x14) = this->position;
    }

    if (*(ZunTimer *)(bomb + 0x18) < 60)
    {
        Float3 v(192.0f, 224.0f, 0.0f);

        t = (f32)*(ZunTimer *)(bomb + 0x18) / 60.0f;
        t = t * t;

        *(Float3 *)((u8 *)this + 0x6b0) = (v - *(Float3 *)(entry + 0x14)) * t + *(Float3 *)(entry + 0x14);

        *(f32 *)((u8 *)this + 0x414) += (-ZUN_PI) / 10.0f;

        this->FUN_0044df00((Float2 *)((u8 *)this + 0x6b0), 32.0f, 0.0f, 0, 6);
        this->FUN_0044e040((Float2 *)((u8 *)this + 0x6b0), 32.0f, 0.0f, 40, 0);
        return;
    }

    *(f32 *)((u8 *)this + 0x414) = 0.0f;
    *(f32 *)((u8 *)this + 0x6b0) = 192.0f;
    *(f32 *)((u8 *)this + 0x6b4) = 224.0f;

    if (*(ZunTimer *)(bomb + 0x18) >= 128)
    {
        *(u8 *)((u8 *)this + 0x5ff) = 0;
    }

    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(60))
    {
        g_EffectManager.SpawnEffect(40, (Float3 *)((u8 *)this + 0x6b0), 1, -1);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(64))
    {
        g_EffectManager.SpawnEffect(40, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffffd0d0);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(68))
    {
        g_EffectManager.SpawnEffect(40, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffffb0b0);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(72))
    {
        g_EffectManager.SpawnEffect(40, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffff8080);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(76))
    {
        g_EffectManager.SpawnEffect(40, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffff4040);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(120))
    {
        Effect *effect;

        g_SoundPlayer.PlaySoundByIdx((SoundIdx)15, 0);
        effect = g_EffectManager.SpawnEffect(42, (Float3 *)((u8 *)this + 0x6b0), 1, -1);
        effect = g_EffectManager.SpawnEffect(43, (Float3 *)((u8 *)this + 0x6b0), 1, -1);
        effect = g_EffectManager.SpawnEffect(44, (Float3 *)((u8 *)this + 0x6b0), 1, -1);

        {
            void *bombGfx;
            Float3 pos(64.0f, 96.0f, 0.0f);

            effect = g_EffectManager.SpawnEffect(45, &pos, 1, 0xff0000f0);
            pos.y = 352.0f;
            effect = g_EffectManager.SpawnEffect(45, &pos, 1, 0xfff00000);
            pos.x = 320.0f;
            effect = g_EffectManager.SpawnEffect(45, &pos, 1, 0xff00f000);
            pos.y = 96.0f;
            effect = g_EffectManager.SpawnEffect(45, &pos, 1, 0xff00f0f0);

            this->FUN_0044df00((Float2 *)((u8 *)this + 0x6b0), 1.0f, 5.0f, 110, 6);
            bombGfx = this->FUN_0044e040((Float2 *)((u8 *)this + 0x6b0), 1.0f, 5.0f, 70, 110);
            *(i32 *)((u8 *)bombGfx + 0x38) = 5;
        }
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(0x82))
    {
        Effect *effect1;

        effect1 = g_EffectManager.SpawnEffect(45, (Float3 *)((u8 *)this + 0x6b0), 1, -1);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(0x8c))
    {
        Effect *effect2;

        effect2 = g_EffectManager.SpawnEffect(45, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffffd0d0);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(0x96))
    {
        Effect *effect3;

        effect3 = g_EffectManager.SpawnEffect(45, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffff8080);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(0xa0))
    {
        Effect *effect4;

        effect4 = g_EffectManager.SpawnEffect(45, (Float3 *)((u8 *)this + 0x6b0), 1, 0xffff0000);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(0xb4))
    {
        ScreenEffect::RegisterChain((ScreenEffectType)3, 8, 1, -1, 0, 21);
        ScreenEffect::RegisterChain((ScreenEffectType)1, 24, 8, 0, 0, 21);
        g_SoundPlayer.PlaySoundByIdx((SoundIdx)0x19, 0);
        return;
    }
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(0xe5))
    {
        *(i32 *)((u8 *)this + 0x6d4) = 1;
        *(ZunTimer *)((u8 *)this + 0x6ec) = 0;
    }
}

// FUNCTION: th08 0x40e3b0
void Player::FUN_0040e3b0()
{
    UpdateBombPattern(this, 30, 0xffd0d0ff, 350, 21);
}

// FUNCTION: th08 0x40e780
void Player::FUN_0040e780()
{
    UpdateBombPattern(this, 35, 0xffffd0d0, 380, 10);
}

// FUNCTION: th08 0x40ee10
#pragma var_order(bomb, entry, t, pos, pos2, effect, v2, v1, b, a)
void Player::FUN_0040ee10()
{
    u8 *bomb;
    u8 *entry;
    f32 t;

    bomb = (u8 *)this + 0xfdc;
    entry = bomb + 0x4c;

    if (((ZunTimer *)(bomb + 0x18))->FUN_0040d3d0() && *(ZunTimer *)(bomb + 0x18) == 0)
    {
        this->FUN_0040be30(1, "\x8d\x67\x95\x84\x81\x75\x95\x73\x96\xe9\x8f\xe9\x83\x8c\x83\x62\x83\x68\x81\x76", 0xf0, 0x122, 0);

        g_SoundPlayer.PlaySoundByIdx((SoundIdx)13, 0);
        *(Float3 *)(entry + 0x14) = *(Float3 *)((u8 *)this + 0x6bc);
        *(Float3 *)(entry + 0x1704) = *(Float3 *)((u8 *)this + 0x9b0);
        *(Float3 *)(entry + 0x2df4) = *(Float3 *)((u8 *)this + 0xca4);
        *(Float3 *)(entry + 0x44e4) = *(Float3 *)((u8 *)this + 0xf98);
        g_SoundPlayer.PlaySoundByIdx((SoundIdx)6, 0);

        *(i32 *)(bomb + 0x14) = 0;
        *(f32 *)((u8 *)this + 0x408) = 0.0f;
        *(f32 *)((u8 *)this + 0x404) = 0.0f;
    }

    if (*(ZunTimer *)(bomb + 0x18) < 60)
    {
        t = (f32)*(ZunTimer *)(bomb + 0x18) / 60.0f;
        t = t * t;

        Float3 pos;

        pos = this->position;
        pos.x -= 32.0f;
        *(Float3 *)((u8 *)this + 0x6bc) = (pos - *(Float3 *)(entry + 0x14)) * t + *(Float3 *)(entry + 0x14);
        pos.x += 32.0f;
        pos.y -= 32.0f;
        *(Float3 *)((u8 *)this + 0x9b0) = (pos - *(Float3 *)(entry + 0x1704)) * t + *(Float3 *)(entry + 0x1704);
        pos.y += 64.0f;
        *(Float3 *)((u8 *)this + 0xca4) = (pos - *(Float3 *)(entry + 0x2df4)) * t + *(Float3 *)(entry + 0x2df4);
        pos.x += 32.0f;
        pos.y -= 32.0f;
        *(Float3 *)((u8 *)this + 0xf98) = (pos - *(Float3 *)(entry + 0x44e4)) * t + *(Float3 *)(entry + 0x44e4);

        return;
    }
    else if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(60))
    {
        *(f32 *)((u8 *)this + 0x408) = 2.0f;
        *(f32 *)((u8 *)this + 0x404) = 2.0f;
        ((AnmVm *)((u8 *)this + 0x40c))->SetInterrupt(2);
        ((AnmVm *)((u8 *)this + 0x700))->SetInterrupt(2);
        ((AnmVm *)((u8 *)this + 0x9f4))->SetInterrupt(2);
        ((AnmVm *)((u8 *)this + 0xce8))->SetInterrupt(2);
    }

    {
        Float3 pos2;

        pos2 = this->position;
        pos2.x -= 32.0f;
        *(Float3 *)((u8 *)this + 0x6bc) = pos2;
        pos2.x += 32.0f;
        pos2.y -= 32.0f;
        *(Float3 *)((u8 *)this + 0x9b0) = pos2;
        pos2.y += 64.0f;
        *(Float3 *)((u8 *)this + 0xca4) = pos2;
        pos2.x += 32.0f;
        pos2.y -= 32.0f;
        *(Float3 *)((u8 *)this + 0xf98) = pos2;
    }

    *(void **)(entry + 0x16ec) = this->FUN_0044df00((Float2 *)&this->position, 96.0f, 0.0f, 0, 6);

    if (((ZunTimer *)(bomb + 0x18))->FUN_0040ebc0(10))
    {
        Effect *effect;

        effect = g_EffectManager.SpawnSpecialEffect(53, &this->position, (*(i32 *)(bomb + 0x14) % 4) + 4, 1, -1);
        *(i32 *)((u8 *)effect + 0x324) = 0x20;
        *(f32 *)((u8 *)effect + 0x334) = 4.0f;

        Float3 v2;
        Float3 v1;

        v2.x = 192.0f;
        v2.y = g_Rng.GetRandomF32InRange(128.0f);
        v2.z = 0.0f;
        v1.x = 0.0f;
        v1.y = 0.0f;
        v1.z = 0.0f;
        ((AnmVm *)effect)->FUN_0040ec30(30, 4, &v1, &v2);

        {
            Float2 b;
            {
                Float2 a;

                a.x = 64.0f;
                a.y = 0.0f;
                b.x = 64.0f;
                b.y = 0.0f;
                ((AnmVm *)effect)->FUN_0040eda0(30, 1, &a, &b);
                ((AnmVm *)effect)->FUN_0040ed50(30, 3, 0xff, 0);
                ((AnmVm *)effect)->FUN_0040eca0(30, 0, -1, 0xffff0000);
            }
        }

        *(i32 *)(bomb + 0x14) += 1;
        g_SoundPlayer.PlaySoundPositionedByIdx((SoundIdx)0x11, this->position.x);
        g_AnmManager->ExecuteScript((AnmVm *)effect);
    }

    if (*(ZunTimer *)((u8 *)this + 0xe2ac4) >= 5)
    {
        *(void **)(entry + 0x16ec) = this->FUN_0044de60((Float2 *)&this->position, 96.0f, 800.0f, 6, 0);
        *(void **)(entry + 0x16ec) = this->FUN_0044de60((Float2 *)&this->position, 800.0f, 96.0f, 6, 0);
    }

    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(0xef))
    {
        *(i32 *)((u8 *)this + 0x6d4) = 1;
        *(ZunTimer *)((u8 *)this + 0x6ec) = 0;
        *(i32 *)((u8 *)this + 0x9c8) = 1;
        *(ZunTimer *)((u8 *)this + 0x9e0) = 0;
        *(i32 *)((u8 *)this + 0xcbc) = 1;
        *(ZunTimer *)((u8 *)this + 0xcd4) = 0;
        *(i32 *)((u8 *)this + 0xfb0) = 1;
        *(ZunTimer *)((u8 *)this + 0xfc8) = 0;
    }
}

// FUNCTION: th08 0x40f570
#pragma var_order(bomb, entry, t, pos, pos2, effect, v2, v1, b, a)
void Player::FUN_0040f570()
{
    u8 *bomb;
    u8 *entry;
    f32 t;

    bomb = (u8 *)this + 0xfdc;
    entry = bomb + 0x4c;

    if (((ZunTimer *)(bomb + 0x18))->FUN_0040d3d0() && *(ZunTimer *)(bomb + 0x18) == 0)
    {
        this->FUN_0040be30(1, "\x8d\x67\x96\x82\x81\x75\x83\x58\x83\x4a\x81\x5b\x83\x8c\x83\x62\x83\x67\x83\x66\x83\x72\x83\x8b\x81\x76", 0x118, 0x140, 1);

        g_SoundPlayer.PlaySoundByIdx((SoundIdx)13, 0);
        *(Float3 *)(entry + 0x14) = *(Float3 *)((u8 *)this + 0x6bc);
        *(Float3 *)(entry + 0x1704) = *(Float3 *)((u8 *)this + 0x9b0);
        *(Float3 *)(entry + 0x2df4) = *(Float3 *)((u8 *)this + 0xca4);
        *(Float3 *)(entry + 0x44e4) = *(Float3 *)((u8 *)this + 0xf98);
        g_SoundPlayer.PlaySoundByIdx((SoundIdx)6, 0);

        *(i32 *)(bomb + 0x14) = 0;
        *(f32 *)((u8 *)this + 0x408) = 0.0f;
        *(f32 *)((u8 *)this + 0x404) = 0.0f;
    }

    if (*(ZunTimer *)(bomb + 0x18) < 60)
    {
        t = (f32)*(ZunTimer *)(bomb + 0x18) / 60.0f;
        t = t * t;

        Float3 pos;

        pos = this->position;
        pos.x -= 32.0f;
        *(Float3 *)((u8 *)this + 0x6bc) = (pos - *(Float3 *)(entry + 0x14)) * t + *(Float3 *)(entry + 0x14);
        pos.x += 32.0f;
        pos.y -= 32.0f;
        *(Float3 *)((u8 *)this + 0x9b0) = (pos - *(Float3 *)(entry + 0x1704)) * t + *(Float3 *)(entry + 0x1704);
        pos.y += 64.0f;
        *(Float3 *)((u8 *)this + 0xca4) = (pos - *(Float3 *)(entry + 0x2df4)) * t + *(Float3 *)(entry + 0x2df4);
        pos.x += 32.0f;
        pos.y -= 32.0f;
        *(Float3 *)((u8 *)this + 0xf98) = (pos - *(Float3 *)(entry + 0x44e4)) * t + *(Float3 *)(entry + 0x44e4);

        return;
    }
    else if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(60))
    {
        *(f32 *)((u8 *)this + 0x408) = 3.0f;
        *(f32 *)((u8 *)this + 0x404) = 3.0f;
        ((AnmVm *)((u8 *)this + 0x40c))->SetInterrupt(2);
        ((AnmVm *)((u8 *)this + 0x700))->SetInterrupt(2);
        ((AnmVm *)((u8 *)this + 0x9f4))->SetInterrupt(2);
        ((AnmVm *)((u8 *)this + 0xce8))->SetInterrupt(2);
    }

    {
        Float3 pos2;

        *(void **)(entry + 0x16ec) = this->FUN_0044df00((Float2 *)&this->position, 96.0f, 0.0f, 0, 6);

        pos2 = this->position;
        pos2.x -= 32.0f;
        *(Float3 *)((u8 *)this + 0x6bc) = pos2;
        pos2.x += 32.0f;
        pos2.y -= 32.0f;
        *(Float3 *)((u8 *)this + 0x9b0) = pos2;
        pos2.y += 64.0f;
        *(Float3 *)((u8 *)this + 0xca4) = pos2;
        pos2.x += 32.0f;
        pos2.y -= 32.0f;
        *(Float3 *)((u8 *)this + 0xf98) = pos2;
    }

    if (((ZunTimer *)(bomb + 0x18))->FUN_0040ebc0(10))
    {
        Effect *effect;

        effect = g_EffectManager.SpawnSpecialEffect(53, &this->position, (*(i32 *)(bomb + 0x14) % 4) + 4, 1, -1);
        *(i32 *)((u8 *)effect + 0x324) = 0x20;
        *(f32 *)((u8 *)effect + 0x334) = 4.0f;

        Float3 v2;
        Float3 v1;

        v2.x = 192.0f;
        v2.y = g_Rng.GetRandomF32InRange(128.0f);
        v2.z = 0.0f;
        v1.x = 0.0f;
        v1.y = 0.0f;
        v1.z = 0.0f;
        ((AnmVm *)effect)->FUN_0040ec30(30, 4, &v1, &v2);

        {
            Float2 b;
            {
                Float2 a;

                a.x = 64.0f;
                a.y = 0.0f;
                b.x = 128.0f;
                b.y = 0.0f;
                ((AnmVm *)effect)->FUN_0040eda0(30, 1, &a, &b);
                ((AnmVm *)effect)->FUN_0040ed50(30, 3, 0xff, 0);
                ((AnmVm *)effect)->FUN_0040eca0(30, 0, -1, 0xffff0000);
            }
        }

        *(i32 *)(bomb + 0x14) += 1;
        g_SoundPlayer.PlaySoundPositionedByIdx((SoundIdx)0x11, this->position.x);
        g_AnmManager->ExecuteScript((AnmVm *)effect);
    }

    if (*(ZunTimer *)((u8 *)this + 0xe2ac4) >= 5)
    {
        *(void **)(entry + 0x16ec) = this->FUN_0044de60((Float2 *)&this->position, 96.0f, 800.0f, 6, 0);
        *(void **)(entry + 0x16ec) = this->FUN_0044de60((Float2 *)&this->position, 800.0f, 96.0f, 6, 0);
    }

    if (((ZunTimer *)(bomb + 0x18))->FUN_0040e350(0x117))
    {
        *(i32 *)((u8 *)this + 0x6d4) = 1;
        *(ZunTimer *)((u8 *)this + 0x6ec) = 0;
        *(i32 *)((u8 *)this + 0x9c8) = 1;
        *(ZunTimer *)((u8 *)this + 0x9e0) = 0;
        *(i32 *)((u8 *)this + 0xcbc) = 1;
        *(ZunTimer *)((u8 *)this + 0xcd4) = 0;
        *(i32 *)((u8 *)this + 0xfb0) = 1;
        *(ZunTimer *)((u8 *)this + 0xfc8) = 0;
    }
}

// FUNCTION: th08 0x40fcd0
void Player::FUN_0040fcd0()
{
    UpdateBombPattern(this, 6, 0xff80ffff, 300, 10);
}

// FUNCTION: th08 0x4103f0
void Player::FUN_004103f0()
{
    UpdateBombPattern(this, 7, 0xffffff80, 300, 10);
}

// FUNCTION: th08 0x410c40
void Player::FUN_00410c40()
{
    UpdateBombPattern(this, 8, 0xff80c0ff, 260, 8);
}

// FUNCTION: th08 0x410fe0
void Player::FUN_00410fe0()
{
    UpdateBombPattern(this, 9, 0xffffc080, 260, 8);
}

// FUNCTION: th08 0x411b10
void Player::FUN_00411b10()
{
    UpdateBombPattern(this, 10, 0xffc0c0ff, 360, 12);
}

// FUNCTION: th08 0x4123d0
void Player::FUN_004123d0()
{
    UpdateBombPattern(this, 11, 0xffffc0c0, 360, 12);
}

// FUNCTION: th08 0x413140
void Player::FUN_00413140()
{
    UpdateBombPattern(this, 14, 0xffa0a0ff, 420, 9);
}

// FUNCTION: th08 0x413990
void Player::FUN_00413990()
{
    UpdateBombPattern(this, 15, 0xffffa0a0, 480, 9);
}

// FUNCTION: th08 0x40bf00
#pragma var_order(effect)
void Player::FUN_0040bf00()
{
    Effect *effect;

    if (*(Effect **)((u8 *)this + 0xe2b1c) != NULL)
    {
        (*(Effect **)((u8 *)this + 0xe2b1c))->active = 0;
    }
    effect = g_EffectManager.SpawnSpecialEffect(23, &this->position, 0, 1, -1);
    *(ZunTimer *)((u8 *)&effect->vm + 0x80) = 0;
    *(ZunTimer *)((u8 *)&effect->vm + 0xd4) = *(ZunTimer *)((u8 *)this + 0xe2af4);
    *(u8 *)((u8 *)&effect->vm + 0xfc) = 0;
    *(Float2 *)((u8 *)&effect->vm + 0x268) = *(Float2 *)((u8 *)&effect->vm + 0x18);
    *(f32 *)((u8 *)&effect->vm + 0x270) = 0.0625f;
    *(f32 *)((u8 *)&effect->vm + 0x274) = 0.0625f;
    *(i32 *)((u8 *)&effect->vm + 0x100) = ((ZunTimer *)((u8 *)this + 0xe2af4))->AsFrames();
    *(f32 *)((u8 *)&effect->vm + 0x14) = *(f32 *)((u8 *)&effect->vm + 0x14) * -1.0f;
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
    for (i = 0, entry = (u8 *)this + 0x1028; i < 16; i++, entry += 0x16f0)
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

// FUNCTION: th08 0x40d010
#pragma var_order(vm, i, entry)
void Player::FUN_0040d010()
{
    u8 *entry;
    u32 i;
    AnmVm *vm;

    this->FUN_0040bc60(0x802020d0);
    for (i = 0, entry = (u8 *)this + 0x1028; i < 128; i++, entry += 0x16f0)
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

// FUNCTION: th08 0x40d100
#pragma var_order(bomb, effect, i)
void Player::FUN_0040d100()
{
    u8 *bomb;
    Effect *effect;
    i32 i;

    bomb = (u8 *)this + 0xfdc;
    if (((ZunTimer *)(bomb + 0x18))->FUN_0040d3d0() && *(ZunTimer *)(bomb + 0x18) == 0)
    {
        if (g_GameManager.IsSpellPractice() == 0)
        {
            this->FUN_0040be30(-1, "\x81\x75\x83\x66\x83\x42\x83\x5d\x83\x8b\x83\x75\x83\x58\x83\x79\x83\x8b\x81\x76", 120, 200, 0);
        }
        else
        {
            this->FUN_0040be30(-1, "\x81\x75\x83\x66\x83\x42\x83\x5d\x83\x8b\x83\x75\x83\x58\x83\x79\x83\x8b\x81\x76", 40, 200, 0);
        }
        g_EffectManager.SpawnEffect(12, &this->position, 1, 0xff4040ff);
        effect = g_EffectManager.SpawnSpecialEffect(0x32, &this->position, 4, 1, 0xff4040ff);
        *(ZunTimer *)((u8 *)effect + 0x50) = 0;
        if (g_GameManager.IsSpellPractice() == 0)
        {
            *(ZunTimer *)((u8 *)effect + 0xa4) = 0x5a;
        }
        else
        {
            *(ZunTimer *)((u8 *)effect + 0xa4) = 0x1e;
        }
        *(u8 *)((u8 *)effect + 0xf8) = 5;
        *(f32 *)((u8 *)effect + 0x238) = 8.0f;
        *(f32 *)((u8 *)effect + 0x244) = 128.0f;
        *(f32 *)((u8 *)effect + 0x23c) = 64.0f;
        *(f32 *)((u8 *)effect + 0x248) = 0.0f;
        *(f32 *)((u8 *)effect + 0x208) = 8.0f;
        *(f32 *)((u8 *)effect + 0x20c) = 64.0f;
        *(i32 *)((u8 *)effect + 0x324) = 0x40;
        *(i32 *)((u8 *)effect + 0x318) = 0;
        *(f32 *)((u8 *)effect + 0x314) = 8.0f;
        *(f32 *)((u8 *)effect + 0x320) = 15.0f;
        *(f32 *)((u8 *)effect + 0x334) = 6.0f;
        g_SoundPlayer.PlaySoundByIdx((SoundIdx)13, 0);
        *(i32 *)((u8 *)this + 0x408) = 0;
        *(i32 *)((u8 *)this + 0x404) = 0;
        for (i = 0; i < 8; i++)
        {
            if (*(u8 **)(0xf54cc0 + i * 4) != NULL)
            {
                *(u32 *)(*(u8 **)(0xf54cc0 + i * 4) + 0x3324) &= ~0x40;
            }
        }
    }
}

// FUNCTION: th08 0x40e610
#pragma var_order(angle, i, vm)
void Player::FUN_0040e610()
{
    f32 angleStep = 0.20943952f;
    AnmVm *vm = (AnmVm *)((u8 *)this + 0x11e0);

    this->FUN_0040bc60(0x80404040);
    for (i32 i = 0; i < 5; i++, vm++)
    {
        f32 angle = i * angleStep - ZUN_PI / 2.0f - angleStep * 2.0f;
        if (angle < -ZUN_PI)
        {
            angle += ZUN_2PI;
        }
        vm->pos = this->position;
        f32 radius = vm->loadedSprite->widthPx * vm->scale.y / 2.0f;
        vm->pos.x += cosf(angle) * radius;
        vm->pos.y += sinf(angle) * radius;
        vm->SetZRotation(angle);
        vm->pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
        vm->pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
        vm->pos.z = 0.0f;
        g_AnmManager->Draw2D(vm);
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

// FUNCTION: th08 0x4113a0
#pragma var_order(vm, entry)
void Player::FUN_004113a0()
{
    u8 *entry = (u8 *)this + 0x1028;
    AnmVm *vm = (AnmVm *)(entry + 0x1b8);

    this->FUN_0040bc60(0x802020d0);
    vm->pos = *(Float3 *)(entry + 0x14) + vm->pos2;
    vm->pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
    vm->pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
    vm->pos.z = 0.01f;
    g_AnmManager->Draw2D(vm);
    vm++;
    vm->pos = *(Float3 *)(entry + 0x14) + vm->pos2;
    vm->pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
    vm->pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
    vm->pos.z = 0.0f;
    g_AnmManager->Draw2D(vm);
}

// FUNCTION: th08 0x413890
#pragma var_order(i, entry)
void Player::FUN_00413890()
{
    u8 *entry = (u8 *)this + 0x1028;
    this->FUN_0040bc60(0x80404040);
    for (i32 i = 0; i < 96; i++, entry += 0x16f0)
    {
        if (*(i32 *)entry == 0)
        {
            continue;
        }
        AnmVm *vm = (AnmVm *)(entry + 0x1b8);
        vm->SetZRotation(atan2f(*(f32 *)(entry + 0x30), *(f32 *)(entry + 0x2c)));
        vm->pos = *(Float3 *)(entry + 0x14);
        vm->pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
        vm->pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
        vm->pos.z = 0.0f;
        g_AnmManager->Draw2D(vm);
    }
}

// FUNCTION: th08 0x4142c0
#pragma var_order(i, entry)
void Player::FUN_004142c0()
{
    u8 *entry = (u8 *)this + 0x1028;
    this->FUN_0040bc60(0x80802020);
    for (i32 i = 0; i < 128; i++, entry += 0x16f0)
    {
        if (*(i32 *)entry == 0)
        {
            continue;
        }
        AnmVm *vm = (AnmVm *)(entry + 0x1b8);
        vm->SetZRotation(atan2f(*(f32 *)(entry + 0x30), *(f32 *)(entry + 0x2c)));
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

// FUNCTION: th08 0x40dee0
void Player::FUN_0040dee0()
{
    ZunTimer *timer = (ZunTimer *)((u8 *)this + 0xff4);
    ZunRect rect;
    D3DCOLOR color;

    if (timer->AsFrames() < 90)
    {
        this->FUN_0040bc60(0x802020d0);
        return;
    }
    rect.left = 32.0f;
    rect.top = 16.0f;
    rect.right = 416.0f;
    rect.bottom = 464.0f;
    if (timer->AsFrames() < 120)
    {
        i32 component = (timer->AsFrames() - 90) * 208 / 30;
        this->FUN_0040bc60(0x80000000 | ((component / 5 + 208) << 16) |
                           ((component + 32) << 8) | component + 32);
        color = ((timer->AsFrames() - 90) * 255 / 30) << 24 | 0xffffff;
        ScreenEffect::DrawSquare(&rect, color);
    }
    else if (timer->AsFrames() < 220)
    {
        ScreenEffect::DrawSquare(&rect, 0x70ffffff);
    }
    else
    {
        this->FUN_0040bc60(0x802020d0);
    }
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

// FUNCTION: th08 0x44d2c0
void Player::FUN_0044d2c0()
{
    ZunTimer *timer = (ZunTimer *)((u8 *)this + 0xe2af4);

    if (*(i32 *)((u8 *)this + 0xe2a70) != 0)
    {
        (*(i32 *)((u8 *)this + 0xe2a70))--;
        this->FUN_0044de60((Float2 *)&this->position, 768.0f, 896.0f, -1, 0);
    }
    if (this->playerState == 3)
    {
        *(u8 *)((u8 *)this + 4) = 0;
        Effect *effect = *(Effect **)((u8 *)this + 0xe2b1c);
        if (effect != NULL)
        {
            effect->position = this->position;
        }
        (*timer)--;
        if (timer->AsFrames() <= 0)
        {
            if (effect != NULL)
            {
                effect->active = 0;
                *(Effect **)((u8 *)this + 0xe2b1c) = NULL;
            }
            this->playerState = PLAYER_STATE_ALIVE;
            timer->SetCurrent(0);
            *(i32 *)((u8 *)this + 0x200) = -1;
        }
        else if (timer->AsFrames() % 8 < 2)
        {
            *(D3DCOLOR *)((u8 *)this + 0x200) = 0xfff02020;
        }
        else
        {
            *(D3DCOLOR *)((u8 *)this + 0x200) = 0xffffffff;
        }
    }
    else
    {
        timer->Tick();
    }
}

// FUNCTION: th08 0x44d420
void Player::FUN_0044d420()
{
    *(Float3 *)((u8 *)this + 0xe2aa4) = Float3(-999.0f, -999.0f, 0.0f);
    *(Float3 *)((u8 *)this + 0xe2ab0) = Float3(-999.0f, -999.0f, 0.0f);
    *(i32 *)((u8 *)this + 0xe2ac0) = 0;
}

// FUNCTION: th08 0x44aec0
void Player::FUN_0044aec0()
{
    u16 input = g_CurFrameInput;
    i32 direction = 0;
    if ((input & 0x50) == 0x50) direction = 5;
    else if ((input & 0x60) == 0x60) direction = 7;
    else if ((input & 0x90) == 0x90) direction = 6;
    else if ((input & 0xa0) == 0xa0) direction = 8;
    else if (input & 0x20) direction = 2;
    else if (input & 0x10) direction = 1;
    else if (input & 0x40) direction = 3;
    else if (input & 0x80) direction = 4;
    *(i32 *)((u8 *)this + 0xe2a98) = direction;

    this->isFocus = (*(i32 *)((u8 *)this + 0xfdc) != 0)
                        ? (*(i32 *)((u8 *)this + 0xfe0) & 1) != 0
                        : (input & 4) != 0;
    if (this->playerState != PLAYER_STATE_ALIVE)
    {
        return;
    }
    f32 speed = this->isFocus ? 2.0f : 4.0f;
    f32 diagonal = speed * 0.70710677f;
    switch (direction)
    {
    case 1: this->position.y -= speed * this->verticalSpeedMultiplier; break;
    case 2: this->position.y += speed * this->verticalSpeedMultiplier; break;
    case 3: this->position.x -= speed * this->horizontalSpeedMultiplier; break;
    case 4: this->position.x += speed * this->horizontalSpeedMultiplier; break;
    case 5:
        this->position.x -= diagonal * this->horizontalSpeedMultiplier;
        this->position.y -= diagonal * this->verticalSpeedMultiplier;
        break;
    case 6:
        this->position.x += diagonal * this->horizontalSpeedMultiplier;
        this->position.y -= diagonal * this->verticalSpeedMultiplier;
        break;
    case 7:
        this->position.x -= diagonal * this->horizontalSpeedMultiplier;
        this->position.y += diagonal * this->verticalSpeedMultiplier;
        break;
    case 8:
        this->position.x += diagonal * this->horizontalSpeedMultiplier;
        this->position.y += diagonal * this->verticalSpeedMultiplier;
        break;
    }
    if (this->position.x < 8.0f) this->position.x = 8.0f;
    if (this->position.x > 376.0f) this->position.x = 376.0f;
    if (this->position.y < 16.0f) this->position.y = 16.0f;
    if (this->position.y > 432.0f) this->position.y = 432.0f;
}

// FUNCTION: th08 0x44c650
void Player::FUN_0044c650()
{
    if (*(i32 *)((u8 *)this + 0xe2a6c) > 0)
    {
        (*(i32 *)((u8 *)this + 0xe2a6c))--;
    }
    if (this->playerState == PLAYER_STATE_DEAD)
    {
        this->FUN_0044cbf0();
        return;
    }
    if (this->playerState == PLAYER_STATE_SPAWNING)
    {
        this->FUN_0044d180();
        return;
    }
    this->FUN_0044c5b0();
    if (*(i32 *)((u8 *)this + 0xe2a68) > 0)
    {
        this->FUN_0044cbf0();
    }
}

// FUNCTION: th08 0x44cbf0
void Player::FUN_0044cbf0()
{
    i32 *deathCounter = (i32 *)((u8 *)this + 0xe2a68);
    ZunTimer *timer = (ZunTimer *)((u8 *)this + 0xe2af4);

    if (*deathCounter > 0)
    {
        (*deathCounter)--;
        *(u8 *)((u8 *)this + 4) = 1;
        if (*deathCounter == 0)
        {
            Effect *effect = *(Effect **)((u8 *)this + 0xe2b28);
            if (effect != NULL)
            {
                effect->active = 0;
                *(Effect **)((u8 *)this + 0xe2b28) = NULL;
            }
            g_EffectManager.SpawnEffect(12, &this->position, 3, 0xff4040ff);
            g_EffectManager.SpawnEffect(6, &this->position, 16, -1);
            g_SoundPlayer.PlaySoundPositionedByIdx((SoundIdx)15, this->position.x);
            this->playerState = PLAYER_STATE_DEAD;
            timer->SetCurrent(0);
            this->FUN_0044d150();
        }
        return;
    }
    timer->Tick();
    f32 factor = (f32)timer->AsFrames() / 30.0f;
    this->playerSprite.scale.x = 1.0f - factor;
    this->playerSprite.scale.y = factor * 2.0f + 1.0f;
    i32 alpha = 255 - timer->AsFrames() * 255 / 30;
    if (alpha < 0) alpha = 0;
    this->playerSprite.color1.d3dColor = (alpha << 24) | 0xffffff;
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

static void ResetPlayerEffectState(u8 *data)
{
    ZunTimer *timer = (ZunTimer *)(data + 0x2e0);
    if (timer->AsFrames() == 0)
    {
        ((AnmVm *)data)->SetInterrupt(5);
    }
    if (timer->AsFrames() > 16)
    {
        *(i32 *)(data + 0x2c8) = 0;
        *(i32 *)(data + 0x2ec) = 0;
        *(i32 *)(data + 0x2f0) = 0;
    }
}

// FUNCTION: th08 0x44e3a0
i32 __fastcall Player::FUN_0044e3a0(u8 *data)
{
    i32 state = *(i32 *)(data + 0x2c8);
    if (state == 1)
    {
        this->playerAnm->SetAndExecuteScriptIdx((AnmVm *)data, 18);
        *(Float3 *)(data + 0x2a4) = this->position;
        *(f32 *)(data + 0x2a8) -= 96.0f;
        if (*(f32 *)(data + 0x2a8) < 32.0f)
        {
            *(f32 *)(data + 0x2a8) = 32.0f;
        }
        *(i32 *)(data + 0x2c8) = 2;
        *(void **)((u8 *)this + 0xe2abc) = NULL;
    }
    else if (state == 2)
    {
        this->FUN_0044e770((Effect *)data);
        f32 velocityX = *(f32 *)(data + 0x2bc);
        i32 phase = *(i32 *)(data + 0x2cc);
        if ((velocityX <= 0.0f && phase == 0) || (velocityX >= 0.0f && phase == 1))
        {
            ((AnmVm *)data)->SetInterrupt(2);
            *(i32 *)(data + 0x2cc) = phase == 0 ? 1 : 2;
            ((AnmVm *)data)->scale.y = -((AnmVm *)data)->scale.y;
        }
    }
    else if (state == 3)
    {
        ResetPlayerEffectState(data);
    }
    return 0;
}

static void InitializePlayerOrb(Player *player, u8 *data, i32 script)
{
    player->playerAnm->SetAndExecuteScriptIdx((AnmVm *)data, script);
    *(i32 *)(data + 0x2c8) = 2;
    *(Float3 *)(data + 0x2b0) = player->position;
    i32 mode = *(i32 *)(data + 0x2d0) & 3;
    f32 xOffsets[4] = {-30.0f, -10.0f, 10.0f, 30.0f};
    *(f32 *)(data + 0x2b0) += xOffsets[mode];
    *(f32 *)(data + 0x2b4) -= mode == 0 || mode == 3 ? 96.0f : 32.0f;
    *(f32 *)(data + 0x2d8) = mode == 1 || mode == 3 ? ZUN_PI : 0.0f;
}

static void UpdatePlayerOrb(u8 *data)
{
    i32 mode = *(i32 *)(data + 0x2d0) & 3;
    if (((ZunTimer *)(data + 0x2e0))->AsFrames() >= 12)
    {
        f32 angularVelocity[4] = {0.02617994f, -0.03490659f, 0.03490659f, -0.02617994f};
        *(f32 *)(data + 0x2d8) = AddNormalizeAngle(*(f32 *)(data + 0x2d8), angularVelocity[mode]);
    }
    ((Float3 *)(data + 0x2a4))->FromAngleMagnitude(*(f32 *)(data + 0x2d8), 8.0f);
    *(Float3 *)(data + 0x2a4) += *(Float3 *)(data + 0x2b0);
}

// FUNCTION: th08 0x44eb70
i32 __fastcall Player::FUN_0044eb70(u8 *data)
{
    i32 state = *(i32 *)(data + 0x2c8);
    if (state == 1)
    {
        InitializePlayerOrb(this, data, 24);
    }
    else if (state == 2)
    {
        UpdatePlayerOrb(data);
    }
    else if (state == 3)
    {
        ResetPlayerEffectState(data);
    }
    return 0;
}

// FUNCTION: th08 0x44ee70
i32 __fastcall Player::FUN_0044ee70(u8 *data)
{
    i32 state = *(i32 *)(data + 0x2c8);
    if (state == 1)
    {
        InitializePlayerOrb(this, data, 24);
    }
    else if (state == 2)
    {
        UpdatePlayerOrb(data);
        *(D3DCOLOR *)(data + 0x1f0) = this->isFocus ? 0xffff8080 : 0xff80ffff;
    }
    else if (state == 3)
    {
        ResetPlayerEffectState(data);
    }
    return 0;
}

static void InitializePlayerSpiral(Player *player, u8 *data)
{
    player->playerAnm->SetAndExecuteScriptIdx((AnmVm *)data, 21);
    *(i32 *)(data + 0x2c8) = 2;
    *(Float3 *)(data + 0x2b0) = *(Float3 *)((u8 *)player + 0x380);
    *(f32 *)(data + 0x2d8) = 0.0f;
    *(f32 *)(data + 0x2dc) = -ZUN_PI / 2.0f;
}

static void UpdatePlayerSpiral(Player *player, u8 *data, D3DCOLOR color)
{
    *(f32 *)(data + 0x2d8) = AddNormalizeAngle(*(f32 *)(data + 0x2d8), 0.05235988f);
    ((Float3 *)(data + 0x2a4))->FromAngleMagnitude(*(f32 *)(data + 0x2d8), 8.0f);
    Float3 target = *(Float3 *)((u8 *)player + 0x380);
    Float3 delta = target - *(Float3 *)(data + 0x2b0);
    f32 length = delta.FUN_0040b4c0();
    if (length != 0.0f)
    {
        delta *= 0.05f / length;
    }
    *(Float3 *)(data + 0x2b0) += delta;
    *(Float3 *)(data + 0x2a4) += *(Float3 *)(data + 0x2b0);
    *(f32 *)(data + 0x2ac) = 0.0f;
    g_EffectManager.SpawnEffect(47, (Float3 *)(data + 0x2a4), 1, color);
}

// FUNCTION: th08 0x44f2d0
i32 __fastcall Player::FUN_0044f2d0(u8 *data)
{
    i32 state = *(i32 *)(data + 0x2c8);
    if (state == 1)
    {
        InitializePlayerSpiral(this, data);
    }
    else if (state == 2)
    {
        UpdatePlayerSpiral(this, data, 0x80405080);
    }
    else if (state == 3)
    {
        ResetPlayerEffectState(data);
    }
    return 0;
}

// FUNCTION: th08 0x44f5e0
i32 __fastcall Player::FUN_0044f5e0(u8 *data)
{
    i32 state = *(i32 *)(data + 0x2c8);
    if (state == 1)
    {
        InitializePlayerSpiral(this, data);
    }
    else if (state == 2)
    {
        UpdatePlayerSpiral(this, data, 0xfff05080);
    }
    else if (state == 3)
    {
        ResetPlayerEffectState(data);
    }
    return 0;
}

// FUNCTION: th08 0x44e770
#pragma var_order(delta, targetPosition)
void __fastcall Player::FUN_0044e770(Effect *effect)
{
    Float3 targetPosition;
    Float3 delta;

    targetPosition = this->position;
    targetPosition.y -= 96.0f;
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
    if (((ZunTimer *)((u8 *)this + 0xe2ac4))->AsFrames() >= 0 &&
        *(void **)((u8 *)this + 0xe2abc) != NULL &&
        ((ZunTimer *)((u8 *)effect + 0x2e0))->AsFrames() >= 10)
    {
        effect->vm.SetInterrupt(3);
        *(i32 *)((u8 *)effect + 0x2cc) = 3;
    }
    else
    {
        *(void **)((u8 *)this + 0xe2abc) = NULL;
    }
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

// FUNCTION: th08 0x44f930
#pragma var_order(state, targetPosition)
i32 __fastcall Player::FUN_0044f930(u8 *data)
{
    Float3 targetPosition = this->position;
    i32 state = *(i32 *)(data + 0x2c8);

    if (state == 1)
    {
        this->playerAnm->SetAndExecuteScriptIdx((AnmVm *)data, 21);
        *(i32 *)(data + 0x2c8) = 2;
        *(Float3 *)(data + 0x2b0) = targetPosition;
        *(f32 *)(data + 0x2d8) = *(i32 *)(data + 0x2d0) == 0 ? 0.0f : -2.3561945f;
        state = 2;
    }
    if (state == 2)
    {
        if (*(i32 *)(data + 0x2d0) == 0)
        {
            targetPosition.x -= 32.0f;
            *(f32 *)(data + 0x2d8) = AddNormalizeAngle(*(f32 *)(data + 0x2d8), 0.05235988f);
        }
        else
        {
            targetPosition.x += 32.0f;
            *(f32 *)(data + 0x2d8) = AddNormalizeAngle(*(f32 *)(data + 0x2d8), -0.05235988f);
        }
        ((Float3 *)(data + 0x2a4))->FromAngleMagnitude(*(f32 *)(data + 0x2d8), 6.0f);
        Float3 delta = targetPosition - *(Float3 *)(data + 0x2b0);
        f32 length = delta.FUN_0040b4c0();
        if (length != 0.0f)
        {
            delta *= 0.09f / length;
        }
        *(Float3 *)(data + 0x2b0) += delta;
        *(Float3 *)(data + 0x2a4) += *(Float3 *)(data + 0x2b0);
        *(f32 *)(data + 0x2ac) = 0.0f;
        g_EffectManager.SpawnEffect(47, (Float3 *)(data + 0x2a4), 1, 0x80602050);
    }
    else if (state == 3)
    {
        ZunTimer *timer = (ZunTimer *)(data + 0x2e0);
        if (timer->AsFrames() == 0)
        {
            ((AnmVm *)data)->SetInterrupt(5);
        }
        if (timer->AsFrames() > 16)
        {
            *(i32 *)(data + 0x2c8) = 0;
            *(i32 *)(data + 0x2ec) = 0;
            *(i32 *)(data + 0x2f0) = 0;
        }
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

// FUNCTION: th08 0x44fe20
#pragma var_order(i, index)
i32 __fastcall Player::FUN_0044fe20(u8 *data, i32, void *shotData)
{
    PlayerShotData *shot = (PlayerShotData *)shotData;
    i32 index = shot->phase;
    ZunTimer *slotTimer = (ZunTimer *)((u8 *)this + 0xe2a38 + index * 16);
    void **owner = (void **)((u8 *)this + 0xe2a44 + index * 16);

    if (*(i32 *)((u8 *)this + 0xfdc) != 0 ||
        (*(u32 *)((u8 *)&g_GameManager + 0x3dbac) & 0x2000) != 0)
    {
        return 0;
    }
    if (*owner != NULL)
    {
        if (*(void **)((u8 *)this + 0xe2a80 + index * 4) != shotData)
        {
            *(i16 *)((u8 *)*owner + 0x1fe) = 1;
            *owner = NULL;
        }
        return 0;
    }
    slotTimer->SetCurrent(999);
    *owner = data;
    *(i16 *)(data + 0x466) = (i16)index;
    *(i16 *)(data + 0x468) = shot->option;
    *(f32 *)(data + 0x444) = shot->offsetX;
    *(f32 *)(data + 0x448) = shot->offsetY;
    *(i16 *)(data + 0x46a) = shot->interval;
    this->FUN_0044fb70(data, shot);
    for (i32 i = 31; i >= 0; i--)
    {
        *(f32 *)(data + 0x2b0 + i * 12) = -999.0f;
    }
    *(f32 *)(data + 0x2a4) = -999.0f;
    *(void **)((u8 *)this + 0xe2a80 + index * 4) = shotData;
    return 1;
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

// FUNCTION: th08 0x450320
#pragma var_order(speed, deltaY, deltaX)
i32 __fastcall Player::FUN_00450320(u8 *data)
{
    f32 deltaX;
    f32 deltaY;
    f32 speed;

    if (*(i16 *)(data + 0x462) == 1)
    {
        if (*(f32 *)((u8 *)this + 0xe2aa4) >= -100.0f && ((ZunTimer *)(data + 0x454))->AsFrames() < 40)
        {
            deltaX = *(f32 *)((u8 *)this + 0xe2aa4) - *(f32 *)(data + 0x2a4);
            deltaY = *(f32 *)((u8 *)this + 0xe2aa8) - *(f32 *)(data + 0x2a8);
            speed = sqrtf(deltaX * deltaX + deltaY * deltaY) / (*(f32 *)(data + 0x44c) / 4.0f);
            if (speed < 1.0f)
            {
                speed = 1.0f;
            }
            deltaX = deltaX / speed + *(f32 *)(data + 0x43c);
            deltaY = deltaY / speed + *(f32 *)(data + 0x440);
            speed = sqrtf(deltaX * deltaX + deltaY * deltaY);
            *(f32 *)(data + 0x44c) = speed > 10.0f ? 10.0f : speed;
            if (*(f32 *)(data + 0x44c) < 1.0f)
            {
                *(f32 *)(data + 0x44c) = 1.0f;
            }
            *(f32 *)(data + 0x43c) = deltaX * *(f32 *)(data + 0x44c) / speed;
            *(f32 *)(data + 0x440) = deltaY * *(f32 *)(data + 0x44c) / speed;
        }
        else if (*(f32 *)(data + 0x44c) < 10.0f)
        {
            *(f32 *)(data + 0x44c) += 1.0f / 3.0f;
            deltaX = *(f32 *)(data + 0x43c);
            deltaY = *(f32 *)(data + 0x440);
            speed = sqrtf(deltaX * deltaX + deltaY * deltaY);
            *(f32 *)(data + 0x43c) = deltaX * *(f32 *)(data + 0x44c) / speed;
            *(f32 *)(data + 0x440) = deltaY * *(f32 *)(data + 0x44c) / speed;
        }
    }
    *(f32 *)(data + 0x450) = atan2f(*(f32 *)(data + 0x440), *(f32 *)(data + 0x43c));
    return 0;
}

// FUNCTION: th08 0x4505d0
i32 __fastcall Player::FUN_004505d0(u8 *data)
{
    i32 index = *(i16 *)(data + 0x466);
    ZunTimer *slotTimer = (ZunTimer *)((u8 *)this + 0xe2a38 + index * 16);
    void **owner = (void **)((u8 *)this + 0xe2a44 + index * 16);

    if (*owner != data)
    {
        *(i16 *)(data + 0x1fe) = 1;
    }
    if (this->playerState == PLAYER_STATE_DEAD || *(i32 *)((u8 *)this + 0xfdc) != 0)
    {
        if (slotTimer->AsFrames() > 20)
        {
            slotTimer->SetCurrent(20);
        }
    }
    if (slotTimer->AsFrames() <= 0)
    {
        slotTimer->SetCurrent(0);
        *owner = NULL;
        *(i16 *)(data + 0x462) = 0;
        return 1;
    }
    *(f32 *)(data + 0x2a4) += *(f32 *)(data + 0x444);
    *(f32 *)(data + 0x2ac) = 0.44f;
    if (this->playerState == PLAYER_STATE_DEAD)
    {
        return 1;
    }
    *(f32 *)(data + 0x1c) = *(f32 *)(data + 0x2a8) / 14.0f;
    *(f32 *)(data + 0x434) = *(f32 *)(data + 0x2a8);
    *(f32 *)(data + 0x2a8) /= 2.0f;
    if (g_GameManager.GaugeIsExtremelyYoukai())
    {
        *(u8 *)(data + 0x1f2) = 0xff;
        *(u8 *)(data + 0x1f1) = 0xd0;
        *(u8 *)(data + 0x1f0) = 0xb0;
    }
    else
    {
        *(D3DCOLOR *)(data + 0x1f0) = 0xffffffff;
    }
    return 0;
}

// FUNCTION: th08 0x450840
#pragma var_order(i)
i32 __fastcall Player::FUN_00450840(u8 *data)
{
    i32 i;
    i32 index = *(i16 *)(data + 0x466);
    void **owner = (void **)((u8 *)this + 0xe2a44 + index * 16);

    if (*owner != data || ((ZunTimer *)((u8 *)this + 0xe2ac4))->AsFrames() < 0 ||
        this->playerState == PLAYER_STATE_DEAD || *(i32 *)((u8 *)this + 0xfdc) != 0)
    {
        *(i16 *)(data + 0x1fe) = 1;
        *owner = NULL;
        *(void **)(data + 0x474) = NULL;
    }
    if (*(i32 *)((u8 *)this + 0x6d4) == 0)
    {
        *owner = NULL;
        return 1;
    }
    for (i = 0; i < *(i16 *)(data + 0x46a); i++)
    {
        Float3 *trailPosition = (Float3 *)(data + 0x2b0 + i * 24);
        if (trailPosition->x >= -900.0f)
        {
            u8 *hitbox = (u8 *)this->FUN_0044dfa0((Float2 *)trailPosition, 16.0f, 448.0f, 1, 0);
            *(u8 *)(hitbox + 0x3d) = 1;
        }
    }
    for (i = 31; i > 0; i--)
    {
        *(Float3 *)(data + 0x2b0 + i * 12) = *(Float3 *)(data + 0x2b0 + (i - 1) * 12);
        *(f32 *)(data + 0x2b4 + i * 12) -= 1.0f;
    }
    *(Float3 *)(data + 0x2b0) = *(Float3 *)(data + 0x2a4);
    *(Float3 *)(data + 0x2a4) = *(Float3 *)((u8 *)this + 0x6b0);
    *(f32 *)(data + 0x2ac) = 0.44f;
    *(f32 *)(data + 0x434) = 448.0f;
    *(f32 *)(data + 0x2a8) -= 208.0f;
    if (g_GameManager.GaugeIsExtremelyYoukai())
    {
        *(u8 *)(data + 0x1f2) = 0xff;
        *(u8 *)(data + 0x1f1) = 0xd0;
        *(u8 *)(data + 0x1f0) = 0xb0;
    }
    else
    {
        *(D3DCOLOR *)(data + 0x1f0) = 0xffffffff;
    }
    return 0;
}

// FUNCTION: th08 0x450ad0
#pragma var_order(i, alpha, originalAlpha)
i32 __fastcall Player::FUN_00450ad0(u8 *data)
{
    i32 originalAlpha = *(u8 *)(data + 0x1f3);
    i32 alpha = originalAlpha * 3 / 4;

    for (i32 i = 0; i < *(i16 *)(data + 0x46a) * 2; i += 2)
    {
        Float3 *position = (Float3 *)(data + 0x2b0 + i * 12);
        if (position->x == -999.0f)
        {
            break;
        }
        ((AnmVm *)data)->pos = *position;
        if (i != 0)
        {
            *(u8 *)(data + 0x1f3) = (u8)(alpha - (alpha / 2) * i / *(i16 *)(data + 0x46a));
        }
        ((AnmVm *)data)->pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
        ((AnmVm *)data)->pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
        if (g_GameManager.GaugeIsExtremelyYoukai())
        {
            *(u8 *)(data + 0x1f2) = 0xff;
            *(u8 *)(data + 0x1f1) = 0x40;
            *(u8 *)(data + 0x1f0) = 0x40;
        }
        g_AnmManager->Draw2D((AnmVm *)data);
    }
    *(u8 *)(data + 0x1f3) = (u8)originalAlpha;
    return 0;
}

// FUNCTION: th08 0x450c50
i32 __fastcall Player::FUN_00450c50(u8 *data, Float3 *position)
{
    if (*(i16 *)(data + 0x462) == 2)
    {
        if (((ZunTimer *)(data + 0x454))->AsFrames() % 2 != 0)
        {
            return 1;
        }
        if (g_Spellcard.IsActive() && ((ZunTimer *)(data + 0x454))->AsFrames() % 4 != 0)
        {
            return 1;
        }
        *(i16 *)(data + 0x460) /= 3;
        if (*(i16 *)(data + 0x460) == 0)
        {
            *(i16 *)(data + 0x460) = 1;
        }
        *(f32 *)(data + 0x43c) *= 0.88f;
        *(f32 *)(data + 0x440) *= 0.88f;
    }
    else
    {
        f32 angle = g_Rng.GetRandomF32InRange(3.1415927f / 2.0f) - 2.3561945f;
        i32 script = *(i16 *)(data + 0x21a);
        f32 size = 0.0f;
        if (script == 12) size = 48.0f;
        if (script == 14) size = 64.0f;
        if (script == 16) size = 80.0f;
        if (script == 18) size = 96.0f;
        if (script == 20) size = 128.0f;
        if (size != 0.0f)
        {
            *(f32 *)(data + 0x430) = size;
            *(f32 *)(data + 0x434) = size;
            ((Float3 *)(data + 0x43c))->FromAngleMagnitude(angle, 6.0f);
        }
    }
    if (((ZunTimer *)(data + 0x454))->AsFrames() % 6 == 0)
    {
        g_EffectManager.SpawnEffect(5, position, 1, -1);
    }
    return 0;
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

// FUNCTION: th08 0x450f60
#pragma var_order(result, i, data, shot)
void __fastcall Player::FUN_00450f60(i32 frame)
{
    u8 *group;
    u8 *shot;
    u8 *data;
    i32 i;
    i32 result;

    group = *(u8 **)((u8 *)this + (this->isFocus ? 0xe2a78 : 0xe2a74)) + 0x38;
    while (g_GameManager.GetPower() >= *(i32 *)(group + 4))
    {
        group += 8;
    }
    shot = *(u8 **)group;
    data = (u8 *)this + 0xbe838;
    for (i = 0; i < 128; i++, data += 0x484)
    {
        if (*(i16 *)(data + 0x462) != 0)
        {
            continue;
        }
        while (*(i16 *)shot >= 0)
        {
            if (*(void **)(shot + 0x28) != NULL)
            {
                result = ((i32(__fastcall *)(Player *, u8 *, i32, void *))*(void **)(shot + 0x28))(
                    this, data, frame, shot);
            }
            else
            {
                result = this->FUN_0044fd80(data, frame, shot);
            }
            if (result == 1)
            {
                *(u32 *)(data + 0x1f8) |= 0x2000;
                *(i16 *)(data + 0x462) = 1;
                *(void **)(data + 0x480) = shot;
                *(void **)(data + 0x474) = *(void **)(shot + 0x2c);
                *(void **)(data + 0x478) = *(void **)(shot + 0x30);
                *(void **)(data + 0x47c) = *(void **)(shot + 0x34);
                shot += 0x38;
                break;
            }
            shot += 0x38;
        }
        if (*(i16 *)shot < 0)
        {
            break;
        }
    }
}

// FUNCTION: th08 0x451150
#pragma var_order(i, data)
void Player::FUN_00451150()
{
    u8 *data = (u8 *)this + 0xbe838;
    for (i32 i = 0; i < 128; i++, data += 0x484)
    {
        if (*(i16 *)(data + 0x462) == 0)
        {
            continue;
        }
        if (*(void **)(data + 0x474) != NULL &&
            ((i32(__fastcall *)(Player *, u8 *))*(void **)(data + 0x474))(this, data) != 0)
        {
            *(i16 *)(data + 0x462) = 0;
            continue;
        }
        *(f32 *)(data + 0x2a4) += g_Supervisor.framerateMultiplier * *(f32 *)(data + 0x43c);
        *(f32 *)(data + 0x2a8) += g_Supervisor.framerateMultiplier * *(f32 *)(data + 0x440);
        if (*(i16 *)(data + 0x464) != 4 && *(i16 *)(data + 0x464) != 5)
        {
            AnmLoadedSprite *sprite = ((AnmVm *)data)->loadedSprite;
            if (!g_GameManager.IsWithinPlayfield(*(f32 *)(data + 0x2a4), *(f32 *)(data + 0x2a8),
                                                  sprite->widthPx, sprite->heightPx))
            {
                *(i16 *)(data + 0x462) = 0;
            }
        }
        if (g_AnmManager->ExecuteScript((AnmVm *)data))
        {
            *(i16 *)(data + 0x462) = 0;
        }
        ((ZunTimer *)(data + 0x454))->Tick();
    }
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

// FUNCTION: th08 0x451500
i32 Player::FUN_00451500()
{
    ZunTimer *timer = (ZunTimer *)((u8 *)this + 0xe2ac4);

    if (*(i32 *)((u8 *)&g_GameManager + 0x3ddc0) < 20 || timer->AsFrames() < 0 || this->FUN_00451d50())
    {
        return 0;
    }
    timer->Tick();
    if (timer->AsFrames() >= 20)
    {
        timer->SetCurrent(-1);
    }
    if ((g_CurFrameInput & 1) != 0 && timer->AsFrames() < 0)
    {
        timer->SetCurrent(0);
    }
    if (this->playerState == PLAYER_STATE_DEAD || this->playerState == PLAYER_STATE_SPAWNING)
    {
        timer->SetCurrent(-1);
    }
    return 0;
}

// FUNCTION: th08 0x451670
#pragma var_order(i, data, damage)
i32 Player::FUN_00451670(Float3 *center, Float3 *size, i32 hitType, i32 *hitCount)
{
    if (this->playerState != PLAYER_STATE_ALIVE)
    {
        return 0;
    }
    if (hitCount != NULL)
    {
        *hitCount = 0;
    }
    Float3 targetMin;
    Float3 targetMax;
    FUN_00451ce0(&targetMin, &targetMax, center, size);
    i32 damage = 0;
    u8 *data = (u8 *)this + 0xbe838;
    for (i32 i = 0; i < 128; i++, data += 0x484)
    {
        if (*(i16 *)(data + 0x462) == 0 ||
            (*(i16 *)(data + 0x462) != 1 && *(i16 *)(data + 0x464) != 3))
        {
            continue;
        }
        Float3 shotMin;
        Float3 shotMax;
        FUN_00451ce0(&shotMin, &shotMax, (Float3 *)(data + 0x2a4), (Float3 *)(data + 0x430));
        if (shotMax.x < targetMin.x || shotMin.x > targetMax.x ||
            shotMax.y < targetMin.y || shotMin.y > targetMax.y)
        {
            continue;
        }
        damage += *(i16 *)(data + 0x460);
        if (hitCount != NULL)
        {
            (*hitCount)++;
        }
        if (*(void **)(data + 0x47c) != NULL)
        {
            ((void(__fastcall *)(Player *, u8 *, i32))*(void **)(data + 0x47c))(this, data, hitType);
        }
        if (*(i16 *)(data + 0x464) != 4 && *(i16 *)(data + 0x464) != 5)
        {
            *(i16 *)(data + 0x462) = 0;
        }
    }
    return damage;
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

// FUNCTION: th08 0x44c390
ChainCallbackResult Player::OnUpdate(Player *player)
{
    player->FUN_0044c5b0();
    player->FUN_0044c650();
    if (player->playerState == PLAYER_STATE_DEAD)
    {
        player->FUN_0044cbf0();
        player->FUN_0044d180();
    }
    else if (player->playerState == PLAYER_STATE_SPAWNING)
    {
        player->FUN_0044d180();
    }
    player->FUN_0044d2c0();
    if (player->playerState == PLAYER_STATE_ALIVE)
        player->FUN_0044aec0();
    if (player->playerAnm != NULL)
        g_AnmManager->ExecuteScript(&player->playerSprite);
    player->FUN_00451150();
    player->FUN_00451500();
    player->FUN_0044d420();
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

// FUNCTION: th08 0x44d650
ZunResult Player::AddedCallback(Player *player)
{
    static const u8 characters[] = {0, 1, 2, 3, 0, 0, 1, 1, 2, 2, 3, 3};
    static const char *sht1[] = {"ply00a.sht", "ply01a.sht", "ply02a.sht", "ply03a.sht",
        "ply00a.sht", "ply00as.sht", "ply01a.sht", "ply01as.sht",
        "ply02a.sht", "ply02as.sht", "ply03a.sht", "ply03as.sht"};
    static const char *sht2[] = {"ply00as.sht", "ply01as.sht", "ply02as.sht", "ply03as.sht",
        "ply00a.sht", "ply00as.sht", "ply01a.sht", "ply01as.sht",
        "ply02a.sht", "ply02as.sht", "ply03a.sht", "ply03as.sht"};
    i32 shot = g_GameManager.character;
    if (shot < 0 || shot >= ARRAY_SIZE_SIGNED(characters)) shot = 0;
    if (FUN_004338b0())
    {
        if (Player::LoadShtFile(&player->player1ShtFile, sht1[shot]) != ZUN_SUCCESS) return ZUN_ERROR;
        if (Player::LoadShtFile(&player->player2ShtFile, sht2[shot]) != ZUN_SUCCESS) return ZUN_ERROR;
        char filename[32];
        sprintf(filename, "player%02d.anm", characters[shot]);
        player->playerAnm = g_AnmManager->LoadAnm(5, filename);
        if (player->playerAnm == NULL) return ZUN_ERROR;
    }
    else
    {
        player->playerAnm = g_AnmManager->GetAnm(5);
    }
    player->playerAnm->SetAndExecuteScriptIdx(&player->playerSprite, shot >= 4 && (shot & 1) ? 5 : 0);
    player->position = Float3(192.0f, 384.0f, 0.49f);
    player->velocity = Float3(0.0f, 0.0f, 0.0f);
    player->horizontalSpeedMultiplier = 1.0f;
    player->verticalSpeedMultiplier = 1.0f;
    player->playerState = PLAYER_STATE_SPAWNING;
    *(u8 *)((u8 *)player + 2) = 1;
    player->isFocus = 2;
    for (i32 i = 0; i < ARRAY_SIZE(player->unk0x2CC); i++) player->unk0x2CC[i] = player->position;
    for (i32 i = 0; i < 0x180; i++) FUN_0044e370((u8 *)player + 0xb8834 + i * 0x40);
    for (i32 i = 0; i < 0x80; i++) *(i16 *)((u8 *)player + 0xbe838 + i * 0x484 + 0x462) = 0;
    *(i32 *)((u8 *)player + 0xe2b2c) = 40;
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

static i32 __fastcall ShtFdd0(Player *p, u8 *d, i32 f, void *s) { return p->FUN_0044fdd0(d, f, s); }
static i32 __fastcall ShtFe20(Player *p, u8 *d, i32 f, void *s) { return p->FUN_0044fe20(d, f, s); }
static i32 __fastcall ShtFfa0(Player *p, u8 *d, i32 f, void *s) { return p->FUN_0044ffa0(d, f, s); }
static i32 __fastcall Sht0080(Player *p, u8 *d, i32 f, void *s) { return p->FUN_00450080(d, f, s); }
static i32 __fastcall Sht0110(Player *p, u8 *d, i32 f, void *s) { return p->FUN_00450110(d, f, s); }
static i32 __fastcall Sht01b0(Player *p, u8 *d, i32 f, void *s) { return p->FUN_004501b0(d, f, s); }
static i32 __fastcall Sht0240(Player *p, u8 *d, i32 f, void *s) { return p->FUN_00450240(d, f, s); }
static i32 __fastcall Sht0320(Player *p, u8 *d) { return p->FUN_00450320(d); }
static i32 __fastcall Sht0580(Player *p, u8 *d) { return FUN_00450580(p, d); }
static i32 __fastcall Sht05d0(Player *p, u8 *d) { return p->FUN_004505d0(d); }
static i32 __fastcall Sht0840(Player *p, u8 *d) { return p->FUN_00450840(d); }
static i32 __fastcall Sht0ad0(Player *p, u8 *d) { return p->FUN_00450ad0(d); }
static i32 __fastcall Sht0c50(Player *p, u8 *d, Float3 *pos) { return p->FUN_00450c50(d, pos); }
static i32 __fastcall Sht0ee0(Player *p, Effect *e, Float3 *pos) { return p->FUN_00450ee0(e, pos); }

DIFFABLE_STATIC_ARRAY_ASSIGN(void *, 9, g_ShtFileFireCallbacks) = {NULL,     Sht0240, ShtFdd0, ShtFdd0, ShtFe20,
                                                                   ShtFfa0, Sht0080, Sht01b0, Sht0110};
DIFFABLE_STATIC_ARRAY_ASSIGN(void *, 6, g_ShtFileUpdateCallbacks) = {NULL, Sht0320, NULL, Sht0580, Sht05d0, Sht0840};
DIFFABLE_STATIC_ARRAY_ASSIGN(void *, 2, g_ShtFileDrawCallbacks) = {NULL, Sht0ad0};
DIFFABLE_STATIC_ARRAY_ASSIGN(void *, 3, g_ShtFileHitCallbacks) = {NULL, Sht0c50, Sht0ee0};

#pragma var_order(i, entry)
// FUNCTION: th08 0x44dd70
ZunResult Player::LoadShtFile(PlayerRawShtFile **header, const char *path)
{
    i32 i;
    u8 *entry;

    *header = (PlayerRawShtFile *)FileSystem::OpenFile(path, NULL, false);
    if (*header == NULL)
    {
        return ZUN_ERROR;
    }

    for (i = 0; i < *(u16 *)((u8 *)*header + 2); i++)
    {
        *(u32 *)((u8 *)*header + 0x38 + i * 8) += (u32)*header;
        entry = *(u8 **)((u8 *)*header + 0x38 + i * 8);
        while (*(i16 *)entry >= 0)
        {
            *(void **)(entry + 0x28) = g_ShtFileFireCallbacks[*(u32 *)(entry + 0x28)];
            *(void **)(entry + 0x2c) = g_ShtFileUpdateCallbacks[*(u32 *)(entry + 0x2c)];
            *(void **)(entry + 0x30) = g_ShtFileDrawCallbacks[*(u32 *)(entry + 0x30)];
            *(void **)(entry + 0x34) = g_ShtFileHitCallbacks[*(u32 *)(entry + 0x34)];
            entry += 0x38;
        }
    }
    return ZUN_SUCCESS;
}

} /* namespace th08 */

// FUNCTION: th08 0x44e370
void FUN_0044e370(void *data)
{
    memset(data, 0, 0x40);
    *(i32 *)((u8 *)data + 0x38) = 1;
}

