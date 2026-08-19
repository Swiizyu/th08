#include "th_pch.h"

#include "BulletManager.hpp"
#include "GameManager.hpp"
#include "ItemManager.hpp"
#include "Player.hpp"

namespace th08
{

DIFFABLE_STATIC(ItemManager, g_ItemManager);

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
        if (g_GameManager.globals->pointItemExtendsSoFar < 6)
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
        if (g_GameManager.globals->pointItemExtendsSoFar < 3)
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

// STUB: th08 0x440500
void ItemManager::OnUpdate()
{
    // TODO: NEEDS WORK ON Gui
}

// STUB: th08 0x440cf0
void Item::CollectPowerSmall()
{
}

// STUB: th08 0x440e40
void Item::CollectPoint()
{
}

// STUB: th08 0x441020
void Item::CollectPointSmall()
{
}

// STUB: th08 0x441170
void Item::CollectPowerBig()
{
}

// STUB: th08 0x4412b0
void Item::CollectTimeOrb()
{
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

// STUB: th08 0x441450
void ItemManager::ConvertAllPowerItemsToTimeOrbs(Item *item)
{
    // TODO: NEEDS WORK ON EffectManager
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
