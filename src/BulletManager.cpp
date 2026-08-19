#include "th_pch.h"

#include "BulletManager.hpp"
#include "EffectManager.hpp"
#include "ItemManager.hpp"
#include "ZunMath.hpp"

u32 FUN_004338c0();

namespace th08
{

DIFFABLE_STATIC(BulletManager, g_BulletManager);
DIFFABLE_STATIC(ChainElem, g_BulletManagerCalcChain);
DIFFABLE_STATIC(ChainElem, g_BulletManagerDrawChain);

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

// STUB: th08 0x433070
ZunResult BulletManager::AddedCallback(BulletManager *bulletManager)
{
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
