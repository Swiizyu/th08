#include "th_pch.h"

#include "AsciiManager.hpp"
#include "BulletManager.hpp"
#include "EffectManager.hpp"
#include "GameManager.hpp"
#include "Gui.hpp"
#include "ItemManager.hpp"
#include "Player.hpp"
#include "ReplayManager.hpp"
#include "SoundPlayer.hpp"
#include "Spellcard.hpp"

namespace th08
{

DIFFABLE_STATIC(ItemManager, g_ItemManager);
DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 6, g_PowerUpThresholds) = {8, 24, 48, 80, 128, 999};

// FUNCTION: th08 0x4337f0
void ItemManager::FUN_004337f0()
{
    memset(this, 0, sizeof(ItemManager));
    this->itemListTail = &this->itemListHead;
}

#pragma var_order(i, item)
Item *ItemManager::SpawnItem(Float3 *position, ItemType itemType, i32 state)
{
    i32 i;
    Item *item = &this->items[this->nextIndex];

    if (position->x < -64.0f || position->x > 448.0f)
    {
        return &this->items[MAX_ITEMS];
    }

    if (g_GameManager.GetPower() >= 128 && (itemType == ITEM_POWER_SMALL || itemType == ITEM_POWER_BIG))
    {
        itemType = ITEM_POINT_SMALL;
    }
    if (itemType == ITEM_TIME)
    {
        state = ITEM_STATE_UNK3;
    }
    else if (itemType == ITEM_TIME2)
    {
        state = ITEM_STATE_UNK5;
        itemType = ITEM_TIME;
    }

    for (i = 0; i < MAX_ITEMS; i++)
    {
        this->nextIndex++;

        if (item->isInUse)
        {
            if (this->nextIndex >= MAX_ITEMS)
            {
                this->nextIndex = 0;
                item = &this->items[0];
            }
            else
            {
                item++;
            }

            if (itemType == ITEM_TIME)
            {
                return &this->items[MAX_ITEMS];
            }

            continue;
        }

        if (this->nextIndex >= MAX_ITEMS)
        {
            this->nextIndex = 0;
        }

        item->isInUse = true;
        item->currentPosition = *position;
        item->startPositionOrVelocity.x = 0.0f;
        item->startPositionOrVelocity.y = -2.2f;
        item->startPositionOrVelocity.z = 0.0f;
        item->itemType = itemType;
        item->state = state;
        item->timer = 0;

        if (state == ITEM_STATE_UNK2)
        {
            item->targetPosition.x = g_Rng.GetRandomF32InRange(288.0f) + 48.0f;
            item->targetPosition.y = g_Rng.GetRandomF32InRange(192.0f) - 64.0f;
            item->targetPosition.z = 0.0f;
            item->startPositionOrVelocity = item->currentPosition;
        }
        else if (state == ITEM_STATE_UNK3)
        {
            item->startPositionOrVelocity.y = -2.0f - g_Rng.GetRandomF32InRange(0.2f);
            item->startPositionOrVelocity.x = g_Rng.GetRandomF32SignedInRange(0.6f);

            if (g_Player.playerState == PLAYER_STATE_DEAD)
            {
                item->state = ITEM_STATE_DEFAULT;
                item->startPositionOrVelocity.x = 0.0f;
                item->startPositionOrVelocity.y = -0.9f;
                item->startPositionOrVelocity.z = 0.0f;
            }
        }
        // ZUN bloat: This is just a duplicate of the above state!
        else if (state == ITEM_STATE_UNK5)
        {
            item->startPositionOrVelocity.y = -2.0f - g_Rng.GetRandomF32InRange(0.2f);
            item->startPositionOrVelocity.x = g_Rng.GetRandomF32SignedInRange(0.6f);

            if (g_Player.playerState == PLAYER_STATE_DEAD)
            {
                item->state = ITEM_STATE_DEFAULT;
                item->startPositionOrVelocity.x = 0.0f;
                item->startPositionOrVelocity.y = -0.9f;
                item->startPositionOrVelocity.z = 0.0f;
            }
        }

        // TODO: Uncomment this when BulletManager is actually done
        // g_BulletManager.bulletAnm->SetAndExecuteScriptIdx(&item->sprite, itemType + 61);

        item->sprite.color1.d3dColor = 0xFFFFFFFF;
        item->sprite.zWriteDisabled = true;
        item->isMaxValue = false;
        item->isOnscreen = true;
        this->itemListTail->next = item;
        item->prev = this->itemListTail;
        item->next = NULL;
        this->itemListTail = item;

        return i < MAX_ITEMS ? item : &this->items[MAX_ITEMS];
    }
}

DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 6, g_PointItemExtendThresholds) = {100, 250, 500, 800, 1100, 9999};
DIFFABLE_STATIC_ARRAY_ASSIGN(i32, 4, g_ExPointItemExtendThresholds) = {200, 666, 9999, 1};

void ItemManager::UpdatePointItemExtendThreshold()
{
    if (g_GameManager.difficulty < 4)
    {
        if ((u32)g_GameManager.globals->pointItemExtendsSoFar < 6)
        {
            g_GameManager.globals->nextPointItemExtendThreshold =
                g_PointItemExtendThresholds[g_GameManager.globals->pointItemExtendsSoFar];
        }
        else
        {
            g_GameManager.globals->nextPointItemExtendThreshold =
                (g_GameManager.globals->pointItemExtendsSoFar - 5) * 500 + g_PointItemExtendThresholds[5];
        }
    }
    else
    {
        if ((u32)g_GameManager.globals->pointItemExtendsSoFar < 3)
        {
            g_GameManager.globals->nextPointItemExtendThreshold =
                g_ExPointItemExtendThresholds[g_GameManager.globals->pointItemExtendsSoFar];
        }
        else
        {
            g_GameManager.globals->nextPointItemExtendThreshold = 99999;
        }
    }
}

// FUNCTION: th08 0x440500
#pragma var_order(itemType, next, angle, itemTimer, collectSpeed, item, hitbox, soundIdx)
void ItemManager::OnUpdate()
{
    i32 soundIdx;
    Float3 hitbox;
    Item *item;
    f32 collectSpeed;
    f32 itemTimer;
    f32 angle;
    Item *next;
    i32 itemType;

    soundIdx = 0;
    item = this->itemListHead.next;
    hitbox = Float3(16.0f, 16.0f, 16.0f);
    this->itemCount = 0;
    collectSpeed = 8.0f * g_Supervisor.framerateMultiplier;
    while (item != NULL)
    {
        this->itemCount++;
        next = item->next;
        if (item->state == ITEM_STATE_UNK2)
        {
            if (item->timer < 60)
            {
                itemTimer = (f32)item->timer / 60.0f;
                item->currentPosition = item->targetPosition * itemTimer +
                                        item->startPositionOrVelocity * (1.0f - itemTimer);
                goto check_collision;
            }
            if (item->timer >= 60)
            {
                item->startPositionOrVelocity = Float3(0.0f, 0.0f, 0.0f);
                item->state = ITEM_STATE_DEFAULT;
            }
        }
        else if (item->state == ITEM_STATE_UNK3 || item->state == ITEM_STATE_UNK5)
        {
            item->startPositionOrVelocity.y += 0.03f * g_Supervisor.framerateMultiplier;
            if (item->startPositionOrVelocity.y >= 3.0f || item->timer == 0)
            {
                item->state = ITEM_STATE_AUTOCOLLECT;
            }
            if (g_Player.playerState == PLAYER_STATE_DEAD)
            {
                item->state = ITEM_STATE_DEFAULT;
                item->startPositionOrVelocity = Float3(0.0f, -0.7f, 0.0f);
            }
        }
        else if (item->state == ITEM_STATE_AUTOCOLLECT)
        {
            if (g_Player.playerState != PLAYER_STATE_DEAD)
            {
                angle = atan2f(g_Player.position.y - item->currentPosition.y,
                               g_Player.position.x - item->currentPosition.x);
                item->startPositionOrVelocity.FromAngleMagnitude(angle, collectSpeed);
            }
            else
            {
                item->startPositionOrVelocity = Float3(0.0f, -0.7f, 0.0f);
                item->state = ITEM_STATE_DEFAULT;
            }
        }
        else
        {
            item->startPositionOrVelocity.x = 0.0f;
            item->startPositionOrVelocity.z = 0.0f;
            if (item->startPositionOrVelocity.y < -2.2f)
            {
                item->startPositionOrVelocity.y = -2.2f;
            }
        }

        item->currentPosition += item->startPositionOrVelocity * g_Supervisor.framerateMultiplier;
        if (item->state == ITEM_STATE_DEFAULT &&
            item->currentPosition.y >= g_GameManager.arcadeRegionSize.y + 16.0f)
        {
            g_GameManager.DecreaseSubrank(3);
            item->Delete();
            item = next;
            continue;
        }
        if (item->startPositionOrVelocity.y < 3.0f)
        {
            item->startPositionOrVelocity.y += 0.03f * g_Supervisor.framerateMultiplier;
        }
        else
        {
            item->startPositionOrVelocity.y = 3.0f;
        }

check_collision:
        if (item->state != ITEM_STATE_UNK3 && g_Player.CalcItemBoxCollision(&item->currentPosition, &hitbox))
        {
            if (g_ReplayManager != NULL)
            {
                g_ReplayManager->inputFlags |= 0x40;
            }
            itemType = item->itemType;
            switch (itemType)
            {
            case ITEM_POWER_SMALL:
                item->CollectPowerSmall();
                break;
            case ITEM_POINT:
                item->CollectPoint();
                break;
            case ITEM_POWER_BIG:
                item->CollectPowerBig();
                break;
            case ITEM_BOMB:
                if (g_GameManager.GetBombsRemaining() < 8)
                {
                    g_GameManager.AddToBombCount(1);
                }
                g_GameManager.IncreaseSubrank(5);
                break;
            case ITEM_POWER_FULL:
                if (g_GameManager.GetPower() < 128)
                {
                    g_BulletManager.FUN_00415c60();
                    g_Gui.ShowPopupText(0, 1);
                    g_ItemManager.ConvertAllPowerItemsToTimeOrbs(item);
                }
                g_GameManager.SetPower(128);
                g_GameManager.AddScore(1000);
                break;
            case ITEM_EXTEND:
                g_GameManager.CollectExtend();
                break;
            case ITEM_POINT_STAR:
                g_GameManager.AddScore(100);
                break;
            case ITEM_TIME:
                item->CollectTimeOrb();
                break;
            case ITEM_POINT_SMALL:
                item->CollectPointSmall();
                break;
            }
            if (soundIdx <= 0x15)
            {
                soundIdx = item->isMaxValue ? 0x2c : 0x15;
            }
            item->Delete();
            item = next;
            continue;
        }
        item->timer++;
        if (item->sprite.currentInstruction != NULL)
        {
            g_AnmManager->ExecuteScript(&item->sprite);
        }
        item = next;
    }
    if (soundIdx != 0)
    {
        g_SoundPlayer.PlaySoundByIdx((SoundIdx)soundIdx, 0);
    }
}

// FUNCTION: th08 0x440cf0
#pragma var_order(i, oldPowerLevel)
void Item::CollectPowerSmall()
{
    i32 i;
    i32 oldPowerLevel;

    if (128 > g_GameManager.GetPower())
    {
        i = 0;
        while (g_GameManager.GetPower() >= g_PowerUpThresholds[i])
        {
            i++;
        }
        oldPowerLevel = i;
        *(u8 *)((u8 *)&g_GameManager + 0x3dba8) = 0;
        g_GameManager.AddPower(1);
        if (g_GameManager.GetPower() >= 128)
        {
            g_GameManager.SetPower(128);
            if (!g_Spellcard.spellcard_fun_004178a0())
            {
                g_BulletManager.FUN_00415c60();
            }
            g_Gui.ShowPopupText(0, 1);
            g_ItemManager.ConvertAllPowerItemsToTimeOrbs(this);
        }
        g_GameManager.AddScore(10);
        g_Gui.flags.powerDisplayUpdateFrames = 2;
        while (g_GameManager.GetPower() >= g_PowerUpThresholds[i])
        {
            i++;
        }
        if (i != oldPowerLevel)
        {
            g_AsciiManager.CreateScorePopup(&this->currentPosition, -1, 0xffffc0a0);
            g_SoundPlayer.PlaySoundByIdx(SOUND_POWERUP, 0);
        }
        else
        {
            g_AsciiManager.CreateScorePopup(&this->currentPosition, 10, 0xffffffff);
        }
    }
    g_GameManager.IncreaseSubrank(1);
}

// FUNCTION: th08 0x440e40
#pragma var_order(maximumValue, value)
void Item::CollectPoint()
{
    i32 maximumValue;
    i32 value;

    maximumValue = g_GameManager.globals->pointItemValue;
    value = this->currentPosition.y < *(f32 *)((u8 *)g_Player.player1ShtFile + 0x1c)
                ? maximumValue
                : maximumValue / 2 -
                      (i32)(this->currentPosition.y - *(f32 *)((u8 *)g_Player.player1ShtFile + 0x1c)) *
                          (g_GameManager.globals->pointItemValue / 1000);
    if (this->isMaxValue == 1)
    {
        value = maximumValue;
    }
    value -= value % 10;
    if (g_GameManager.GaugeIsExtremelyHuman())
    {
        value += value;
    }
    g_AsciiManager.CreateScorePopup(&this->currentPosition, value,
                                    value >= maximumValue ? 0xffffff00 : 0xffffffff);
    if (value >= maximumValue)
    {
        this->isMaxValue = true;
    }
    g_GameManager.AddScore(value);
    g_GameManager.globals->pointItemsCollectedInStage++;
    g_GameManager.globals->pointItemsCollected++;
    g_Gui.flags.pointDisplayUpdateFrames = 2;
    if (value >= maximumValue)
    {
        g_GameManager.IncreaseSubrank(10);
    }
    else
    {
        g_GameManager.IncreaseSubrank(3);
    }
    if (g_GameManager.globals->pointItemExtendsSoFar >= 0)
    {
        do
        {
            ItemManager::UpdatePointItemExtendThreshold();
            if (g_GameManager.globals->pointItemsCollected <
                g_GameManager.globals->nextPointItemExtendThreshold)
            {
                break;
            }
            g_GameManager.CollectExtend();
            g_GameManager.globals->pointItemExtendsSoFar++;
        } while (g_GameManager.globals->pointItemsCollected >=
                 g_GameManager.globals->nextPointItemExtendThreshold);
    }
    (*(i32 *)((u8 *)&g_GameManager + 0x3da94))++;
    g_GameManager.UpdateAntiTamper();
}

// FUNCTION: th08 0x441020
#pragma var_order(maximumValue, value)
void Item::CollectPointSmall()
{
    i32 maximumValue;
    i32 value;

    maximumValue = g_GameManager.globals->pointItemValue;
    value = this->currentPosition.y < *(f32 *)((u8 *)g_Player.player1ShtFile + 0x1c)
                ? maximumValue
                : maximumValue / 2 -
                      (i32)(this->currentPosition.y - *(f32 *)((u8 *)g_Player.player1ShtFile + 0x1c)) *
                          (g_GameManager.globals->pointItemValue / 1000);
    if (this->isMaxValue == 1)
    {
        value = maximumValue;
    }
    maximumValue /= 10;
    maximumValue -= maximumValue % 10;
    value /= 10;
    value -= value % 10;
    if (g_GameManager.GaugeIsExtremelyHuman())
    {
        value += value;
    }
    g_AsciiManager.CreateScorePopup(&this->currentPosition, value,
                                    value >= maximumValue ? 0xffffff00 : 0xffffffff);
    g_GameManager.AddScore(value);
    if (value >= maximumValue)
    {
        this->isMaxValue = true;
    }
}

// FUNCTION: th08 0x441170
#pragma var_order(i, oldPowerLevel)
void Item::CollectPowerBig()
{
    i32 i;
    i32 oldPowerLevel;

    if (g_GameManager.GetPower() >= 128)
    {
        return;
    }
    i = 0;
    while (g_GameManager.GetPower() >= g_PowerUpThresholds[i])
    {
        i++;
    }
    oldPowerLevel = i;
    g_GameManager.AddPower(8);
    if (g_GameManager.GetPower() >= 128)
    {
        g_GameManager.SetPower(128);
        if (!g_Spellcard.spellcard_fun_004178a0())
        {
            g_BulletManager.FUN_00415c60();
        }
        g_Gui.ShowPopupText(0, 1);
        g_ItemManager.ConvertAllPowerItemsToTimeOrbs(this);
    }
    g_Gui.flags.powerDisplayUpdateFrames = 2;
    g_GameManager.AddScore(10);
    while (g_GameManager.GetPower() >= g_PowerUpThresholds[i])
    {
        i++;
    }
    if (i != oldPowerLevel)
    {
        g_AsciiManager.CreateScorePopup(&this->currentPosition, -1, 0xffffc0a0);
        g_SoundPlayer.PlaySoundByIdx(SOUND_POWERUP, 0);
    }
    else
    {
        g_AsciiManager.CreateScorePopup(&this->currentPosition, 10, 0xffffffff);
    }
}

// FUNCTION: th08 0x4412b0
void Item::CollectTimeOrb()
{
    i32 score;
    i32 gaugeAmount;

    if (*(i32 *)((u8 *)&g_Player + 0xe2a7c) == 0)
    {
        if (g_GameManager.globals->pointItemsCollectedInStage >= 2000)
        {
            score = 10000;
        }
        else
        {
            score = g_GameManager.globals->pointItemsCollected / 2 * 10;
            if (score < 100)
            {
                score = 100;
            }
        }
    }
    else
    {
        score = 100;
    }

    if (this != NULL)
    {
        g_AsciiManager.CreatePlayerPointPopup(
            &this->currentPosition, score,
            g_GameManager.GetTimeOrbs() < g_GameManager.GetLastSpellTimeOrbThreshold() ? 0xdfffffff : 0xdfffef80);
    }
    g_Gui.flags.timeDisplayUpdateFrames = 2;
    g_GameManager.AddScore(score);
    g_GameManager.AddTimeOrbs(1);
    g_Spellcard.FUN_00416b10(8000);
    if (*(ZunTimer *)((u8 *)&g_Player + 0xe2adc) == 0)
    {
        score = 111;
        if (g_Player.isFocus)
        {
            gaugeAmount = score;
        }
        else
        {
            gaugeAmount = -score;
        }
        g_GameManager.AddToYoukaiGauge(gaugeAmount, 0);
    }
}

// FUNCTION: th08 0x4413e0
#pragma var_order(item)
void ItemManager::AutoCollectAllItems()
{
    Item *item;

    item = this->itemListHead.next;
    while (item != NULL)
    {
        item->state = ITEM_STATE_AUTOCOLLECT;
        item->startPositionOrVelocity = Float3(0.0f, -0.5f, 0.0f);
        item = item->next;
    }
}

// FUNCTION: th08 0x441450
void ItemManager::ConvertAllPowerItemsToTimeOrbs(Item *excludedItem)
{
    Item *item;

    item = this->itemListHead.next;
    while (item != NULL)
    {
        if (item != excludedItem && (item->itemType == ITEM_POWER_SMALL || item->itemType == ITEM_POWER_BIG))
        {
            if (item->startPositionOrVelocity.y > -0.5f)
            {
                item->startPositionOrVelocity.x = 0.0f;
                item->startPositionOrVelocity.y = -0.5f;
                item->startPositionOrVelocity.z = 0.0f;
            }
            g_EffectManager.SpawnEffect(0, &item->currentPosition, 1, 0xffffffff);
            item->itemType = ITEM_POINT_SMALL;
            g_BulletManager.bonusAnm->SetAndExecuteScriptIdx(&item->sprite, 69);
        }
        item = item->next;
    }
}

void ItemManager::CancelAutoCollect()
{
    Item *item = this->itemListHead.next;
    while (item != NULL)
    {
        if (item->state == ITEM_STATE_AUTOCOLLECT)
        {
            item->state = ITEM_STATE_DEFAULT;
            item->startPositionOrVelocity.x = 0.0f;
            item->startPositionOrVelocity.y = -0.9f;
            item->startPositionOrVelocity.z = 0.0f;
        }
        item = item->next;
    }
}

// FUNCTION: th08 0x4415a0
#pragma var_order(alpha, item)
void ItemManager::OnDraw()
{
    Item *item;
    i32 alpha;

    item = this->itemListHead.next;
    while (item != NULL)
    {
        item->sprite.pos.x = g_GameManager.arcadeRegionTopLeftPos.x + item->currentPosition.x;
        item->sprite.pos.y = g_GameManager.arcadeRegionTopLeftPos.y + item->currentPosition.y;
        item->sprite.pos.z = 0.15f;
        if (item->currentPosition[1] < -8.0f)
        {
            item->sprite.pos.y = 8.0f + g_GameManager.arcadeRegionTopLeftPos.y;
            if (item->isOnscreen)
            {
                g_BulletManager.bonusAnm->SetSprite(&item->sprite, item->itemType + 0xb6);
                item->isOnscreen = false;
                item->sprite.zWriteDisabled = true;
            }
            alpha = 255 - (i32)((8.0f - item->currentPosition[1]) * 255.0f / 128.0f);
            if (alpha < 64)
            {
                alpha = 64;
            }
            item->sprite.color1.d3dColor = (item->sprite.color1.d3dColor & 0xffffff) | alpha << 24;
        }
        else if (!item->isOnscreen)
        {
            g_BulletManager.bonusAnm->SetSprite(&item->sprite, item->itemType + 0xac);
            item->isOnscreen = true;
            item->sprite.color1.d3dColor = 0xffffffff;
            item->sprite.zWriteDisabled = true;
        }
        g_AnmManager->Draw2D(&item->sprite);
        item = item->next;
    }
}

void Item::Delete()
{
    this->isInUse = false;
    this->prev->next = this->next;
    if (this->next != NULL)
    {
        this->next->prev = this->prev;
    }
    if (g_ItemManager.itemListTail == this)
    {
        g_ItemManager.itemListTail = this->prev;
    }
}

i32 ItemManager::GetTimeOrbCount()
{
    Item *next = this->itemListHead.next;
    i32 count = 0;

    while (next != NULL)
    {
        if (next->itemType == ITEM_TIME)
        {
            count++;
        }
        next = next->next;
    }

    return count;
}

} /* namespace th08 */
