#pragma once
#include "Global.hpp"
#include "ZunResult.hpp"
#include "diffbuild.hpp"
#include "utils.hpp"

namespace th08
{

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
};
C_ASSERT(sizeof(EnemyUnkStruct3) == 0x10);

struct Enemy
{
    i32 GetFamiliarCount();
    ZunBool FUN_0041fd20();

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
