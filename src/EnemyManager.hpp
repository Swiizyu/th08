#pragma once
#include "Global.hpp"
#include "EclManager.hpp"
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
    u8 trailingPadding[0xc];
};
C_ASSERT(sizeof(EnemyUnkStruct2) == 0x228);

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
    Enemy();

    i32 GetFamiliarCount();
    void FUN_00415c80();
    Enemy *FUN_0041efc0();
    Enemy *__fastcall FUN_0041f110(void *instruction);
    Enemy *__fastcall FUN_0041f280(void *instruction);
    i32 __fastcall FUN_0041f420(i32 value);
    i32 *__fastcall FUN_0041fe10(void *operand, i32 flags, i32 operandIndex);
    f32 FUN_00420120(f32 value);
    f32 *__fastcall FUN_00420950(void *operand, i32 flags, i32 operandIndex);
    void __fastcall FUN_00420d10(void *instruction);
    void __fastcall FUN_00420f40(void *instruction);
    void __fastcall FUN_00421120(void *instruction, f32 interpolation);
    void __fastcall FUN_00421180(void *instruction, f32 interpolation);
    void __fastcall FUN_00421300(void *instruction);
    void __fastcall FUN_004213f0(void *instruction);
    void *__fastcall FUN_004215f0(void *instruction);
    void __fastcall FUN_00422020(void *instruction);
    void __fastcall FUN_004222b0(void *instruction, f32 angle);
    void __fastcall FUN_004224a0(void *instruction);
    void __fastcall FUN_00422720(void *instruction);
    void FUN_00422c40();
    void FUN_00423150();
    void __fastcall FUN_00423a60(void *instruction);
    void __fastcall FUN_00423e20(void *instruction);
    void __fastcall FUN_004241e0(void *instruction);
    void __fastcall FUN_00421e50(void *instruction);
    void __fastcall FUN_00424a20(void *instruction);
    void __fastcall FUN_00424c40(void *instruction);
    ZunBool FUN_0041f000();
    void FUN_0041f040(i32 x, i32 y, i32 z);
    void FUN_0041f0b0(i32 value);
    void FUN_0041f0e0(i32 value);
    void FUN_00421de0(i16 a, i16 b, i16 c, i16 d, i16 e, i16 f);
    void __fastcall FUN_00421bd0(void *instruction, i16 timelineIndex);
    i32 __fastcall FUN_00421cb0(void *instruction);
    ZunBool FUN_0041fd20();
    ZunBool FUN_0041fd90();
    void FUN_0042adb0(i32 spawnDrops);
    void FUN_0042b2f0();
    void FUN_0042b370(i32 damage);
    i32 FUN_0042b490();
    i32 FUN_0042b930();
    void FUN_0042bc50();
    void FUN_0042bc90();
    void FUN_0042c180();
    void FUN_004244f0(void *instruction);
    void FUN_00424e50(void *instruction);
    void FUN_004250d0(void *instruction);
    void FUN_0042bcf0();
    void FUN_0042bea0(i32 itemState);
    void FUN_0042c290(Float3 *position, Float3 *hitbox);
    void FUN_0042c420();
    void FUN_0042e010();
    void FUN_0042a820();

    u8 unknown0x0[0xc];
    AnmVm vm;
    AnmVm vms[2];
    EnemyUnkStruct2 unk0x7f8;
    EnemyUnkStruct2 unk0xa20[0x10];
    void *enemyData;
    u8 unknown0x2ca4[0x90];
    Float3 position0x2d34;
    Float3 position0x2d40;
    Float3 position0x2d4c;
    Float3 position0x2d58;
    Float3 position0x2d64;
    Float3 position0x2d70;
    Float3 position0x2d7c;
    Float3 position0x2d88;
    u8 unknown0x2d94[0x24];
    Float3 position0x2db8;
    Float3 position0x2dc4;
    Float3 position0x2dd0;
    ZunTimer timer0x2ddc;
    u8 unknown0x2de8[0x2c];
    ZunTimer timer0x2e14;
    u8 unknown0x2e20[4];
    EnemyUnkStruct4 unk0x2e24;
    u8 unknown0x3034[0x30];
    ZunTimer timer0x3064;
    EnemyUnkStruct4 unk0x3070;
    u8 unknown0x3280[0x98];
    ZunTimer timer0x3318;
    u8 unknown0x3324[0x5c];
    i32 field98_0x3380;
    u8 unknown0x3384[0x10];
    EnemyUnkStruct0x1c unk0x3394[0x60];
    VertexTex1DiffuseXyzrhw unk0x3e14[0xc2];
    u8 unknown0x534c[8];
    ZunTimer timer0x5354;
    u8 unknown0x5360[0x70];
};
C_ASSERT(sizeof(Enemy) == 0x53d0);

struct EnemyManager
{
    EnemyManager();
    void Initialize();
    static ZunResult RegisterChain();
    static ChainCallbackResult OnUpdate();
    static ChainCallbackResult OnDrawHighPrio(EnemyManager *enemyManager);
    ChainCallbackResult __fastcall OnDrawImpl(i32 firstLayer, i32 lastLayer);
    static ChainCallbackResult OnDrawLowPrio(EnemyManager *enemyManager);
    static ZunResult AddedCallback(EnemyManager *enemyManager);
    static ZunResult DeletedCallback(EnemyManager *enemyManager);
    static void CutChain();

    i32 DespawnAllEnemies(i32 param_1, i32 param_2);
    Enemy *FUN_0042a4e0(i16 timeline, Float3 *position, i32 health, i8 arg4, i32 field2e08, i32 flag);
    Enemy *FUN_0042a680(i16 timeline, Float3 *position, i32 health, i8 arg4, i32 field2e08, void *eclData);
    void FUN_0042c3b0();

    Enemy enemyTemplate;
    Enemy enemies[481];
    Enemy *bossIds[8];
    u8 unknown0x9dcdc0[0x10];
    EclTimeline timelines[0x10];
    ZunTimer timer;
    u8 unknown0x9dcedc[0x34];
};
C_ASSERT(sizeof(EnemyManager) == 0x9dcf10);

DIFFABLE_EXTERN(EnemyManager, g_EnemyManager);

} /* namespace th08 */
