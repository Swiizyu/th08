#include "th_pch.h"

#include "BulletManager.hpp"
#include "EffectManager.hpp"
#include "ItemManager.hpp"
#include "Player.hpp"
#include "ZunMath.hpp"

u32 FUN_004338b0();
u32 FUN_004338c0();

namespace th08
{

DIFFABLE_STATIC(BulletManager, g_BulletManager);
DIFFABLE_STATIC(ChainElem, g_BulletManagerCalcChain);
DIFFABLE_STATIC(ChainElem, g_BulletManagerDrawChain);

struct BulletTypeInfo
{
    i32 bulletScript;
    i32 spawnFastScript;
    i32 spawnNormalScript;
    i32 spawnSlowScript;
    i32 despawnScript;
};
DIFFABLE_STATIC_ARRAY_ASSIGN(BulletTypeInfo, 21, g_BulletTypeInfos) = {
    {0, 18, 19, 20, 15},     {1, 21, 22, 23, 16},     {2, 21, 22, 23, 16},
    {3, 21, 22, 23, 16},     {4, 21, 22, 23, 16},     {5, 21, 22, 23, 16},
    {6, 21, 22, 23, 16},     {7, 24, 24, 24, 17},     {8, 24, 24, 24, 17},
    {9, 24, 24, 24, 17},     {25, 27, 27, 27, 26},    {106, 21, 22, 23, 16},
    {107, 21, 22, 23, 16},   {108, 21, 22, 23, 16},   {109, 24, 24, 24, 17},
    {110, 24, 24, 24, 17},   {111, 21, 22, 23, 16},   {112, 21, 22, 23, 16},
    {113, 24, 24, 24, 17},   {114, 24, 24, 24, 17},   {115, 24, 24, 24, 17},
};

// FUNCTION: th08 0x42ff70
Float3 *Float3::FUN_0042ff70(const Float3 &other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return this;
}

// FUNCTION: th08 0x42f420
BulletManager::BulletManager()
{
    this->Initialize();
}

// FUNCTION: th08 0x42f500
Bullet::Bullet()
{
}

// FUNCTION: th08 0x415c60
void BulletManager::FUN_00415c60()
{
    this->RemoveAllBullets(1);
}

// FUNCTION: th08 0x430d30
#pragma var_order(i, bullet, delta)
void BulletManager::FUN_00430d30(Float3 *position, f32 radius)
{
    Float3 delta;
    Bullet *bullet;
    i32 i;

    radius *= radius;
    bullet = &this->bullets[0];
    for (i = 0; i < MAX_BULLETS; i++, bullet++)
    {
        if (bullet->state == 0 || bullet->state == 5)
        {
            continue;
        }
        delta = bullet->position - *position;
        if (delta.FUN_0040b500() >= radius)
        {
            continue;
        }
        g_ItemManager.SpawnItem(&bullet->position, ITEM_POINT_STAR, 1);
        memset(bullet, 0, sizeof(Bullet));
    }
}

// FUNCTION: th08 0x430830
#pragma var_order(position, collisionResult, i, sine, bullet, laser, cosine, offset)
void BulletManager::RemoveAllBullets(i32 itemState)
{
    f32 offset;
    f32 cosine;
    Laser *laser;
    Bullet *bullet;
    f32 sine;
    i32 i;
    i32 collisionResult;

    bullet = g_BulletManager.bullets;
    for (i = 0; i < MAX_BULLETS; i++, bullet++)
    {
        if (bullet->state == 0 || bullet->state == 5)
        {
            continue;
        }
        collisionResult = g_Player.FUN_00449ff0(&bullet->position, &bullet->sprites.hitboxSize);
        if (g_Player.FUN_00449ff0(&bullet->position, &bullet->sprites.hitboxSize) == 2)
        {
            g_ItemManager.SpawnItem(&bullet->position, (ItemType)*(i32 *)((u8 *)&g_Player + 0xe2a90), 1);
            memset(bullet, 0, sizeof(Bullet));
        }
        else if (itemState != 4)
        {
            g_ItemManager.SpawnItem(&bullet->position, (ItemType)this->bonusItemType, itemState);
            memset(bullet, 0, sizeof(Bullet));
        }
        else
        {
            bullet->state = 5;
        }
    }

    laser = this->lasers;
    Float3 position;
    for (i = 0; i < ARRAY_SIZE_SIGNED(this->lasers); i++, laser++)
    {
        if (!laser->isInUse)
        {
            continue;
        }
        if ((laser->flags & 4) && itemState != 4)
        {
            continue;
        }
        if (laser->state < 2)
        {
            laser->state = 2;
            laser->timer = 0;
            laser->width = laser->width2;
            if (itemState != 4)
            {
                offset = laser->startOffset;
                fsincos(&sine, &cosine, laser->angle);
                while (laser->endOffset > offset)
                {
                    position.x = cosine * offset + laser->position.x;
                    position.y = sine * offset + laser->position.y;
                    position.z = 0.0f;
                    g_ItemManager.SpawnItem(&position, (ItemType)this->bonusItemType, itemState);
                    offset += 32.0f;
                }
            }
        }
        laser->stopHitboxTime = 0;
    }
    this->cancelFramesRemaining = 10;
}

// FUNCTION: th08 0x42f4a0
BulletTypeSprites::BulletTypeSprites()
{
}

// FUNCTION: th08 0x42f580
Laser::Laser()
{
}

// FUNCTION: th08 0x42f5c0
BulletExState::BulletExState()
{
}

// FUNCTION: th08 0x432f20
void Bullet::Draw()
{
    AnmVm *vmToDraw;

    switch (this->state)
    {
    case 2:
        vmToDraw = &this->sprites.spriteSpawnEffectFast;
        break;
    case 3:
        vmToDraw = &this->sprites.spriteSpawnEffectNormal;
        break;
    case 4:
        vmToDraw = &this->sprites.spriteSpawnEffectSlow;
        break;
    case 5:
        vmToDraw = &this->sprites.spriteDespawnEffect;
        break;
    default:
        vmToDraw = &this->sprites.spriteBullet;
        break;
    }

    vmToDraw->pos[0] = g_GameManager.arcadeRegionTopLeftPos.x + this->position[0];
    vmToDraw->pos[1] = g_GameManager.arcadeRegionTopLeftPos.y + this->position[1];
    vmToDraw->pos[2] = 0.05f;
    vmToDraw->color1.d3dColor = (vmToDraw->color1.d3dColor & 0xff000000) | 0xffffff;
    if (vmToDraw->type != 0)
    {
        vmToDraw->SetZRotation(AddNormalizeAngle((ZUN_PI / 2.0f) + this->angle, 0.0f));
    }
    g_AnmManager->Draw2D(vmToDraw);
}

// FUNCTION: th08 0x432170
void Bullet::FUN_00432170()
{
    this->state = 0;
    this->timeSinceBulletFired = 0;
    this->timeActive = 0;
}

// FUNCTION: th08 0x4321b0
void BulletManager::FUN_004321b0()
{
    this->layerListHeads[5] = NULL;
    this->layerListHeads[4] = NULL;
    this->layerListHeads[3] = NULL;
    this->layerListHeads[2] = NULL;
    this->layerListHeads[1] = NULL;
    this->layerListHeads[0] = NULL;
}

// FUNCTION: th08 0x42f360
void BulletManager::Initialize()
{
    i32 i;
    Bullet *bullet;

    memset(this, 0, sizeof(BulletManager));
    this->nextBulletSlot = this->bullets;
    this->bullets[MAX_BULLETS].state = 6;
    this->bonusItemType = 6;

    bullet = g_BulletManager.bullets;
    for (i = 0; i < MAX_BULLETS; i++, bullet++)
    {
        bullet->sprites.spriteBullet.scriptIndex = -1;
        bullet->sprites.spriteDespawnEffect.scriptIndex = -1;
        bullet->sprites.spriteSpawnEffectFast.scriptIndex = -1;
        bullet->sprites.spriteSpawnEffectNormal.scriptIndex = -1;
        bullet->sprites.spriteSpawnEffectSlow.scriptIndex = -1;
    }
}

// FUNCTION: th08 0x4311a0
ZunResult BulletManager::RegisterChain(char *bulletAnmFilename)
{
    BulletManager *bulletManager = &g_BulletManager;

    bulletManager->Initialize();
    bulletManager->bulletAnmFilename = bulletAnmFilename;
    g_BulletManagerCalcChain.SetCallback((ChainCallback)BulletManager::OnUpdate);
    g_BulletManagerCalcChain.addedCallback = (ChainLifetimeCallback)BulletManager::AddedCallback;
    g_BulletManagerCalcChain.deletedCallback = (ChainLifetimeCallback)BulletManager::DeletedCallback;
    g_BulletManagerCalcChain.arg = bulletManager;
    if (g_Chain.AddToCalcChain(&g_BulletManagerCalcChain, 14) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    g_BulletManagerDrawChain.SetCallback((ChainCallback)BulletManager::OnDraw);
    g_BulletManagerDrawChain.arg = bulletManager;
    g_Chain.AddToDrawChain(&g_BulletManagerDrawChain, 13);

    return ZUN_SUCCESS;
}

// STUB: th08 0x431240
ChainCallbackResult BulletManager::OnUpdate(BulletManager *bulletManager)
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x432b50
#pragma var_order(i, laserSine, laser, laserCenter, laserCosine, bullet)
ChainCallbackResult BulletManager::OnDraw(BulletManager *bulletManager)
{
    i32 i;
    f32 laserSine;
    Laser *laser;
    f32 laserCenter;
    f32 laserCosine;
    Bullet *bullet;

    if (g_GameManager.flags.unk10)
    {
        g_AnmManager->SetMixColor(0xfff01010);
    }

    laser = bulletManager->lasers;
    g_ItemManager.OnDraw();
    for (i = 0; i < ARRAY_SIZE_SIGNED(bulletManager->lasers); i++, laser++)
    {
        if (!laser->isInUse)
        {
            continue;
        }

        fsincos(&laserSine, &laserCosine, laser->angle);
        laserCenter = (laser->endOffset - laser->startOffset) / 2.0f + laser->startOffset;

        laser->vm0.pos[0] = laser->position[0] + laserCosine * laserCenter;
        laser->vm0.pos[1] = laser->position[1] + laserSine * laserCenter;
        laser->vm0.pos[2] = 0.06f;
        laser->color = COLOR_COMBINE_ALPHA(COLOR_WHITE, laser->color);
        laser->vm0.pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
        laser->vm0.pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
        g_AnmManager->Draw2D(&laser->vm0);

        if (laser->startOffset < 16.0f || laser->speed == 0.0f)
        {
            if (laser->unk0x599 == 0 || laser->state != 0)
            {
                laser->vm1.pos[0] = laser->position[0] + laserCosine * laser->startOffset;
                laser->vm1.pos[1] = laser->position[1] + laserSine * laser->startOffset;
                laser->vm1.pos[2] = 0.05f;
                laser->vm1.color1.d3dColor = laser->vm0.color1.d3dColor;
                laser->vm1.flag6 = 1;
                laser->vm1.color1.d3dColor = COLOR_SET_ALPHA2(laser->vm1.color1.d3dColor, 0xff);
                laser->vm1.scale.y = laser->vm1.scale.x =
                    (laser->width / 10.0f) * ((16.0f - laser->startOffset) / 16.0f);
                if (laser->vm1.scale.y <= 0.0f)
                {
                    laser->vm1.scale.y = laser->vm1.scale.x = laser->width / 10.0f;
                }
                laser->vm1.pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
                laser->vm1.pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
                g_AnmManager->Draw2D(&laser->vm1);
            }
        }
    }

    for (i = 0; i < ARRAY_SIZE_SIGNED(bulletManager->layerListHeads); i++)
    {
        bullet = bulletManager->layerListHeads[i];
        while (bullet)
        {
            bullet->Draw();
            bullet = bullet->nextInLayer;
        }
    }

    g_EffectManager.DrawUnkTypeEffects();

    if (g_GameManager.flags.unk10)
    {
        g_AnmManager->SetMixColorDefault();
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x433070
#pragma var_order(i)
ZunResult BulletManager::AddedCallback(BulletManager *bulletManager)
{
    u32 i;

    if (FUN_004338b0())
    {
        bulletManager->bonusAnm = g_AnmManager->PreloadAnm(6, "etama.anm");
        if (bulletManager->bonusAnm == NULL)
        {
            return ZUN_ERROR;
        }
    }
    else
    {
        bulletManager->bonusAnm = g_AnmManager->GetAnm(6);
    }

    for (i = 0; i < 21; i++)
    {
        bulletManager->bonusAnm->SetAndExecuteScriptIdx(&bulletManager->bulletTypeTemplates[i].spriteBullet,
                                                         g_BulletTypeInfos[i].bulletScript);
        bulletManager->bonusAnm->SetAndExecuteScriptIdx(&bulletManager->bulletTypeTemplates[i].spriteSpawnEffectFast,
                                                         g_BulletTypeInfos[i].spawnFastScript);
        bulletManager->bonusAnm->SetAndExecuteScriptIdx(&bulletManager->bulletTypeTemplates[i].spriteSpawnEffectNormal,
                                                         g_BulletTypeInfos[i].spawnNormalScript);
        bulletManager->bonusAnm->SetAndExecuteScriptIdx(&bulletManager->bulletTypeTemplates[i].spriteSpawnEffectSlow,
                                                         g_BulletTypeInfos[i].spawnSlowScript);
        bulletManager->bonusAnm->SetAndExecuteScriptIdx(&bulletManager->bulletTypeTemplates[i].spriteDespawnEffect,
                                                         g_BulletTypeInfos[i].despawnScript);
        bulletManager->bulletTypeTemplates[i].spriteBullet.zWriteDisabled = true;
        bulletManager->bulletTypeTemplates[i].spriteSpawnEffectFast.zWriteDisabled = true;
        bulletManager->bulletTypeTemplates[i].spriteSpawnEffectNormal.zWriteDisabled = true;
        bulletManager->bulletTypeTemplates[i].spriteSpawnEffectSlow.zWriteDisabled = true;
        bulletManager->bulletTypeTemplates[i].spriteDespawnEffect.zWriteDisabled = true;
        bulletManager->bulletTypeTemplates[i].spriteBullet.baseSpriteIndex = bulletManager->bulletTypeTemplates[i].spriteBullet.activeSpriteIndex;
        bulletManager->bulletTypeTemplates[i].bulletHeight =
            (u8)bulletManager->bulletTypeTemplates[i].spriteBullet.loadedSprite->heightPx;
        if (bulletManager->bulletTypeTemplates[i].spriteBullet.loadedSprite->heightPx <= 8.0f)
        {
            bulletManager->bulletTypeTemplates[i].hitboxSize.x = 4.0f;
            bulletManager->bulletTypeTemplates[i].hitboxSize.y = 4.0f;
            bulletManager->bulletTypeTemplates[i].collisionType = 5;
        }
        else if (bulletManager->bulletTypeTemplates[i].spriteBullet.loadedSprite->heightPx <= 16.0f)
        {
            i32 script = g_BulletTypeInfos[i].bulletScript;
            if (script <= 8 || script >= 106)
            {
                bulletManager->bulletTypeTemplates[i].hitboxSize.x = 4.0f;
                bulletManager->bulletTypeTemplates[i].hitboxSize.y = 4.0f;
                bulletManager->bulletTypeTemplates[i].collisionType = 4;
            }
            else
            {
                bulletManager->bulletTypeTemplates[i].hitboxSize.x = 6.0f;
                bulletManager->bulletTypeTemplates[i].hitboxSize.y = 6.0f;
                bulletManager->bulletTypeTemplates[i].collisionType = 3;
            }
        }
        else if (bulletManager->bulletTypeTemplates[i].spriteBullet.loadedSprite->heightPx <= 32.0f)
        {
            i32 script = g_BulletTypeInfos[i].bulletScript;
            if (script == 8 || script == 9 || script == 113 || script == 114 || script == 115)
            {
                bulletManager->bulletTypeTemplates[i].hitboxSize.x = 5.0f;
                bulletManager->bulletTypeTemplates[i].hitboxSize.y = 5.0f;
                bulletManager->bulletTypeTemplates[i].collisionType = 2;
            }
            else
            {
                bulletManager->bulletTypeTemplates[i].hitboxSize.x = 10.0f;
                bulletManager->bulletTypeTemplates[i].hitboxSize.y = 10.0f;
                bulletManager->bulletTypeTemplates[i].collisionType = 1;
            }
        }
        else
        {
            bulletManager->bulletTypeTemplates[i].collisionType = 0;
            bulletManager->bulletTypeTemplates[i].hitboxSize.x = 24.0f;
            bulletManager->bulletTypeTemplates[i].hitboxSize.y = 24.0f;
        }
    }
    g_ItemManager.FUN_004337f0();
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x433820
ZunResult BulletManager::DeletedCallback(BulletManager *bulletManager)
{
    if (::FUN_004338c0())
    {
        g_AnmManager->ReleaseAnm(6);
    }
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x433850
void BulletManager::CutChain()
{
    g_Chain.Cut(&g_BulletManagerCalcChain);
    g_Chain.Cut(&g_BulletManagerDrawChain);
}

} /* namespace th08 */
