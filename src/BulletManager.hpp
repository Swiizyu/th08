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
};
C_ASSERT(sizeof(BulletExState) == 0x20);

struct BulletTypeSprites
{
    BulletTypeSprites();

    AnmVm sprite0;
    AnmVm sprite1;
    AnmVm sprite2;
    AnmVm sprite3;
    AnmVm sprite4;
    Float3 position;
};
C_ASSERT(sizeof(BulletTypeSprites) == 0xd40);

struct Laser
{
    Laser();

    AnmVm sprite0;
    AnmVm sprite1;
    Float3 position;
    u8 unknown[0x34];
    ZunTimer timer;
};
C_ASSERT(sizeof(Laser) == 0x594);

struct Bullet
{
};

struct BulletManager
{
    void Initialize();

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
};

DIFFABLE_EXTERN(BulletManager, g_BulletManager);

} /* namespace th08 */
