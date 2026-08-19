#pragma once

#include "AnmManager.hpp"
#include "Global.hpp"

namespace th08
{

struct BulletExState
{
    BulletExState();

    ZunTimer timer;
    u8 unknown[8];
    Float3 position;
    u8 padding[0xc];
};
C_ASSERT(sizeof(BulletExState) == 0x2c);

struct BulletTypeSprites
{
    BulletTypeSprites();

    AnmVm sprite0;
    AnmVm sprite1;
    AnmVm sprite2;
    AnmVm sprite3;
    AnmVm sprite4;
    Float3 position;
    u32 padding;
};
C_ASSERT(sizeof(BulletTypeSprites) == 0xd44);

struct Laser
{
    Laser();

    AnmVm sprite0;
    AnmVm sprite1;
    Float3 position;
    u8 unknown[0x34];
    ZunTimer timer;
    u8 padding[8];
};
C_ASSERT(sizeof(Laser) == 0x59c);

struct Bullet
{
    Bullet();
    void FUN_00432170();

    BulletTypeSprites typeSprites;
    Float3 position;
    Float3 velocity;
    Float3 acceleration;
    u8 unknown0xd68[0x18];
    ZunTimer timer1;
    ZunTimer timer2;
    u8 unknown0xd98[0x20];
    i16 unk0xdb8;
    u8 unknown0xdba[0x1c6];
    BulletExState exStates[7];
    u32 padding;
};
C_ASSERT(sizeof(Bullet) == 0x10b8);

struct BulletManager
{
    BulletManager();
    void Initialize();
    void FUN_004321b0();

    // STUB: th08 0x415c60
    void FUN_00415c60()
    {
    }

    static ZunResult RegisterChain();
    static ChainCallbackResult OnUpdate(BulletManager *bulletManager);
    static ChainCallbackResult OnDraw(BulletManager *bulletManager);
    static ZunResult AddedCallback(BulletManager *bulletManager);
    static ZunResult DeletedCallback(BulletManager *bulletManager);
    static void CutChain();

    BulletTypeSprites bulletTypes[0x20];
    Bullet bullets[0x601];
    Laser lasers[0x100];
    u8 padding0x6ba538[8];
    ZunTimer timer;
    u8 padding0x6ba54c[8];
    i32 unknown[6];
};
C_ASSERT(sizeof(BulletManager) == 0x6ba56c);

DIFFABLE_EXTERN(BulletManager, g_BulletManager);

} /* namespace th08 */
