#pragma once
#include "Global.hpp"
#include "ZunResult.hpp"
#include "diffbuild.hpp"
#include "utils.hpp"

namespace th08
{

struct EnemyFlags
{
    u32 flag0 : 1;
    u32 flag1 : 1;
    u32 flag2 : 1;
    u32 flag3 : 1;
    u32 flag4 : 1;
    u32 flag5 : 1;
    u32 flag6 : 1;
    u32 flag7 : 1;
    u32 flag8 : 1;
    u32 flag9 : 1;
    u32 flag10 : 1;
    u32 flag11 : 1;
};

struct EnemyFloat3Holder
{
    EnemyFloat3Holder();

    u32 unknown;
    Float3 position;
};
C_ASSERT(sizeof(EnemyFloat3Holder) == 0x10);

struct EnemyUnkStruct0x1c
{
    EnemyUnkStruct0x1c();

    Float3 first;
    Float3 second;
    u32 unknown;
};
C_ASSERT(sizeof(EnemyUnkStruct0x1c) == 0x1c);

struct EnemyUnkStruct3
{
    EnemyUnkStruct3();

    u32 unknown;
    ZunTimer timer;
    u8 padding[0x20];
};
C_ASSERT(sizeof(EnemyUnkStruct3) == 0x30);

struct EnemyUnkStruct2
{
    EnemyUnkStruct2();

    u32 unknown;
    ZunTimer firstTimer;
    u8 padding[0x80];
    ZunTimer secondTimer;
    EnemyUnkStruct3 entries[8];
};
C_ASSERT(sizeof(EnemyUnkStruct2) == 0x21c);

struct EnemyUnkStruct4
{
    EnemyUnkStruct4();

    u32 unknown0x0;
    Float3 position;
    u8 unknown0x10[0x1f4];
    i32 unk0x204;
    u8 unknown0x208[8];
};
C_ASSERT(sizeof(EnemyUnkStruct4) == 0x210);

struct Enemy
{
    i32 GetFamiliarCount();
    void FUN_00415c80();
    Enemy *FUN_0041efc0();
    ZunBool FUN_0041f000();
    void FUN_0041f040(i32 x, i32 y, i32 z);
    void FUN_0041f0b0(i32 value);
    void FUN_0041f0e0(i32 value);
    ZunBool FUN_0041fd20();
    void FUN_0042bc50();
    void FUN_0042bc90();
    void FUN_0042a820();

    unknown_fields(0x0, 0x3380);
    i32 field98_0x3380;
    unknown_fields(0x3384, 0x204c);
};
C_ASSERT(sizeof(Enemy) == 0x53d0);

struct EnemyManager
{
    void Initialize();
    static ZunResult RegisterChain();
    static ChainCallbackResult OnUpdate();
    static ChainCallbackResult OnDrawHighPrio(EnemyManager *enemyManager);
    ChainCallbackResult OnDrawImpl();
    static ChainCallbackResult OnDrawLowPrio(EnemyManager *enemyManager);
    static ZunResult AddedCallback(EnemyManager *enemyManager);
    static ZunResult DeletedCallback(EnemyManager *enemyManager);
    static void CutChain();

    i32 DespawnAllEnemies(i32 param_1, i32 param_2);

    Enemy enemyTemplate;
    Enemy enemies[481];
    Enemy *bossIds[8];
    unknown_fields(0x9dcdc0, 0x150);
};
C_ASSERT(sizeof(EnemyManager) == 0x9dcf10);

DIFFABLE_EXTERN(EnemyManager, g_EnemyManager);

} /* namespace th08 */
