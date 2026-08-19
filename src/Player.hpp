#pragma once

#include "AnmManager.hpp"
#include "Global.hpp"

#include "utils.hpp"

namespace th08
{

struct Effect;

struct PlayerUnkStruct0x40
{
    void FUN_0044e350();

    u8 data[0x3c];
    u8 flag;
    u8 padding[3];
};
C_ASSERT(sizeof(PlayerUnkStruct0x40) == 0x40);

struct PlayerUnkStruct0x2ec
{
    PlayerUnkStruct0x2ec();

    AnmVm vm;
    Float3 position0;
    Float3 position1;
    Float3 position2;
    u8 unknown0x2c8[0x18];
    ZunTimer timer;
};
C_ASSERT(sizeof(PlayerUnkStruct0x2ec) == 0x2ec);

struct PlayerUnkStruct0x460
{
    PlayerUnkStruct0x460();

    AnmVm vm;
    Float3 position;
    Float3 positions[0x20];
    Float3 finalPosition;
    u8 unknown0x43c[0x18];
    ZunTimer timer;
};
C_ASSERT(sizeof(PlayerUnkStruct0x460) == 0x460);

struct PlayerUnkStruct0x16f0
{
    PlayerUnkStruct0x16f0();

    u8 unknown0x0[0x14];
    Float3 position;
    Float3 positions[0x20];
    Float3 position2;
    Float3 position3;
    AnmVm vms[8];
    u8 unknown0x16d8[4];
    ZunTimer timer;
    u8 unknown0x16e8[8];
};
C_ASSERT(sizeof(PlayerUnkStruct0x16f0) == 0x16f0);

struct PlayerUnkStruct0xb7858
{
    PlayerUnkStruct0xb7858();

    u8 unknown0x0[0x18];
    ZunTimer timer;
    u8 unknown0x24[0x28];
    PlayerUnkStruct0x16f0 entries[0x80];
    Float3 position;
};
C_ASSERT(sizeof(PlayerUnkStruct0xb7858) == 0xb7858);

struct PlayerRawShtFile
{
    unknown_fields(0x0, 0x4); // 0x0
    float bombCount;          // 0x4
};

struct PlayerFlags
{
    u32 flag0 : 1;
    u32 flag1 : 1;
    u32 flag2 : 1;
    u32 flag3 : 1;
    u32 flag4 : 1;
    u32 flag5 : 1;
    u32 flag6 : 1;
    u32 flag7 : 1;
};

enum PlayerState
{
    PLAYER_STATE_ALIVE,
    PLAYER_STATE_SPAWNING,
    PLAYER_STATE_DEAD,
};

struct Player
{

    static ZunResult RegisterChain(u32 param);
    static ChainCallbackResult OnUpdate(Player *player);
    static ChainCallbackResult OnDrawHighPrio(Player *player);
    static ChainCallbackResult OnDrawLowPrio(Player *player);
    static ZunResult AddedCallback(Player *player);
    static ZunResult DeletedCallback(Player *player);
    static void CutChain();

    static ZunResult LoadShtFile(PlayerRawShtFile **header, const char *path);

    ZunBool IsHuman();
    ZunBool IsYoukai();
    void __fastcall FUN_0040bc60(D3DCOLOR color);
    void __fastcall FUN_0040be30(i32 arg0, i32 arg1, i32 arg2, i32 duration, i32 arg4);
    void FUN_0040bf00();
    void FUN_0040c820();
    void FUN_00410300();
    void FUN_00410ac0();
    void FUN_0040d310();
    void FUN_0040d950();
    void FUN_0040f550();
    void FUN_0040fcb0();
    void FUN_00412300();
    void FUN_00412fa0();
    ZunBool FUN_00451d50();
    i32 FUN_00449ff0(Float3 *position, Float3 *hitbox);
    i32 FUN_0044a230(Float3 *position, Float3 *hitbox);
    i32 FUN_0044a360(Float3 *position, Float3 *hitbox);
    i32 FUN_0044a470(Float3 *position, Float3 *hitbox);
    i32 CalcLaserHitbox(Float3 *center, Float3 *size, Float3 *origin, f32 rotation, i32 canGraze);
    void ScoreGraze(Float3 *position, i32 suppressTimeOrbEffects);
    void Die();
    f32 AngleToPlayer(Float3 *position);
    i32 CalcItemBoxCollision(Float3 *position, Float3 *hitbox);
    void FUN_0044cba0();
    void FUN_0044c5b0();
    void FUN_0044d150();
    void FUN_0044d180();
    void FUN_0044d2c0();
    void FUN_0044d420();
    void *FUN_0044de60(Float2 *position, f32 width, f32 height, i32 arg4, i32 arg5);
    void *FUN_0044df00(Float2 *position, f32 arg2, f32 arg3, i32 arg4, i32 arg5);
    void *FUN_0044dfa0(Float2 *position, f32 width, f32 height, i32 arg4, i32 arg5);
    void *FUN_0044e040(Float2 *position, f32 arg2, f32 arg3, i32 arg4, i32 arg5);
    void __fastcall FUN_0044e770(Effect *effect);
    void __fastcall FUN_0044e8d0(Effect *effect);
    i32 __fastcall FUN_0044ea40(PlayerUnkStruct0x2ec *data);
    void __fastcall FUN_0044fb70(u8 *data, void *shotData);
    i32 __fastcall FUN_0044fd80(u8 *data, i32 frame, void *shotData);
    i32 __fastcall FUN_0044fdd0(u8 *data, i32 frame, void *shotData);
    i32 __fastcall FUN_0044ffa0(u8 *data, i32 frame, void *shotData);
    i32 __fastcall FUN_00450080(u8 *data, i32 frame, void *shotData);
    i32 __fastcall FUN_00450110(u8 *data, i32 frame, void *shotData);
    i32 __fastcall FUN_004501b0(u8 *data, i32 frame, void *shotData);
    i32 __fastcall FUN_00450240(u8 *data, i32 frame, void *shotData);
    i32 __fastcall FUN_00450320(u8 *data);
    i32 __fastcall FUN_004505d0(u8 *data);
    i32 __fastcall FUN_00450840(u8 *data);
    i32 __fastcall FUN_00450ad0(u8 *data);
    i32 __fastcall FUN_00450c50(u8 *data, Float3 *position);
    i32 __fastcall FUN_00450ee0(Effect *effect, Float3 *position);
    void FUN_004512f0();
    void FUN_00451400();
    i32 FUN_00451500();
    void FUN_00451640();

    i8 playerState;           // 0x0
    unknown_fields(0x1, 0x2); // 0x1
    u8 isFocus;               // 0x3
    unknown_fields(0x4, 0x1); // 0x4
    u8 isYoukai;              // 0x5
    unknown_fields(0x6, 0x2); // 0x6
    i32 timeFocused;          // 0x8
    AnmLoaded *playerAnm;     // 0xC
    AnmVm playerSprite;       // 0x10

    Float3 position;     // 0x2B4
    Float3 unk0x2C0;     // 0x2C0
    Float3 unk0x2CC[16]; // 0x2CC
    Float3 unk0x38C;     // 0x38C
    Float3 unk0x398;     // 0x398
    Float3 unk0x3A4;     // 0x3A4
    Float3 unk0x3B0;     // 0x3B0
    Float3 unk0x3BC;     // 0x3BC
    Float3 unk0x3C8;     // 0x3C8
    Float3 unk0x3D4;     // 0x3D4
    Float3 unk0x3E0;     // 0x3E0
    Float3 unk0x3EC;     // 0x3EC

    Float3 velocity;                 // 0x3F8
    float horizontalSpeedMultiplier; // 0x404
    float verticalSpeedMultiplier;   // 0x408

    unknown_fields(0x40C, 0xE2667); // 0x40C

    PlayerRawShtFile *player1ShtFile; // 0xE2A74
    PlayerRawShtFile *player2ShtFile; // 0xE2A78
    u8 unknown0xe2a7c[0x78];
    ZunTimer timer0xe2af4;
    u8 unknown0xe2b00[0x10];
    ChainElem *calcChain;
    ChainElem *drawChainHighPrio;
    ChainElem *drawChainLowPrio;
    u8 unknown0xe2b1c[0x14];
};
C_ASSERT(sizeof(Player) == 0xe2b30);

DIFFABLE_EXTERN(Player, g_Player);

} /* namespace th08 */
