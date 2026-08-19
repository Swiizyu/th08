#pragma once

#include "AnmManager.hpp"
#include "GameManager.hpp"
#include "Global.hpp"
#include "ZunMath.hpp"

namespace th08
{

struct BulletEx
{
    f32 unk0x0;
    f32 unk0x4;
    i32 unk0x8;
    i32 unk0xc;
    i32 unk0x10;
    i32 unk0x14;
};
C_ASSERT(sizeof(BulletEx) == 0x18);

struct BulletExState
{
    BulletExState();

    ZunTimer timer;
    f32 unk0xc;
    f32 unk0x10;
    Float3 position;
    i32 unk0x20;
    i32 unk0x24;
    i32 unk0x28;
};
C_ASSERT(sizeof(BulletExState) == 0x2c);

struct BulletTypeSprites
{
    BulletTypeSprites();

    AnmVm spriteBullet;
    AnmVm spriteSpawnEffectFast;
    AnmVm spriteSpawnEffectNormal;
    AnmVm spriteSpawnEffectSlow;
    AnmVm spriteDespawnEffect;
    Float3 hitboxSize;
    u8 unk0xd40;
    u8 bulletHeight;
    u8 padding[2];
};
C_ASSERT(sizeof(BulletTypeSprites) == 0xd44);

struct Laser
{
    Laser();

    AnmVm vm0;
    AnmVm vm1;
    Float3 position;
    f32 angle;
    f32 startOffset;
    f32 endOffset;
    f32 length;
    f32 width;
    f32 width2;
    f32 speed;
    i32 startTime;
    i32 startHitboxTime;
    i32 duration;
    i32 stopTime;
    i32 stopHitboxTime;
    ZunBool isInUse;
    ZunTimer timer;
    u16 flags;
    i16 color;
    u8 state;
    u8 unk0x599;
    u8 padding[2];
};
C_ASSERT(sizeof(Laser) == 0x59c);

struct Bullet
{
    Bullet();
    void Draw();
    void FUN_00432170();

    BulletTypeSprites sprites;
    Float3 position;
    Float3 velocity;
    Float3 acceleration;
    f32 speed;
    f32 ex5Float0;
    f32 directionChangeSpeed;
    f32 angle;
    f32 ex5Float1;
    f32 directionChangeRotation;
    ZunTimer timeSinceBulletFired;
    ZunTimer timeActive;
    i32 ex5Int0;
    i32 directionChangeInterval;
    i32 directionChangeNumTimes;
    i32 directionChangeMaxTimes;
    i32 despawnProtectionFrames;
    u32 exFlags;
    u32 flags;
    i16 spriteOffset;
    u16 unk0xdb6;
    u16 state;
    u16 despawnCounter;
    u8 unk0xdbc;
    u8 isGrazed;
    u8 unk0xdbe;
    u8 padding0xdbf;
    Bullet *nextInLayer;
    i32 reimuBarrierCooldownFrames;
    i32 transformSfx;
    i32 currentExIndex;
    BulletEx ex[18];
    BulletExState exStates[7];
    i8 isUnderReisenIllusion;
    u8 padding0x10b5[3];
};
C_ASSERT(sizeof(Bullet) == 0x10b8);

#define MAX_BULLETS 1536

struct BulletManager
{
    BulletManager();
    void Initialize();
    void FUN_004321b0();

    // STUB: th08 0x415c60
    void FUN_00415c60()
    {
    }

    static ZunResult RegisterChain(char *bulletAnmFilename);
    static ChainCallbackResult OnUpdate(BulletManager *bulletManager);
    static ChainCallbackResult OnDraw(BulletManager *bulletManager);
    static ZunResult AddedCallback(BulletManager *bulletManager);
    static ZunResult DeletedCallback(BulletManager *bulletManager);
    static void CutChain();

    BulletTypeSprites bulletTypeTemplates[0x20];
    Bullet bullets[MAX_BULLETS + 1];
    Laser lasers[0x100];
    i32 numActiveBullets;
    i32 cancelFramesRemaining;
    ZunTimer timer;
    i32 unkCounter;
    char *bulletAnmFilename;
    Bullet *layerListHeads[6];
    Bullet *nextBulletSlot;
    i32 bonusItemType;
    AnmLoaded *bonusAnm;
};
C_ASSERT(sizeof(BulletManager) == 0x6ba578);

DIFFABLE_EXTERN(BulletManager, g_BulletManager);

} /* namespace th08 */
