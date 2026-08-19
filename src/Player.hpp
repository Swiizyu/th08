#pragma once

#include "AnmManager.hpp"
#include "Global.hpp"

#include "utils.hpp"

namespace th08
{

struct PlayerUnkStruct0x40
{
    void FUN_0044e350();

    u8 data[0x3c];
    u8 flag;
    u8 padding[3];
};
C_ASSERT(sizeof(PlayerUnkStruct0x40) == 0x40);

struct PlayerRawShtFile
{
    unknown_fields(0x0, 0x4); // 0x0
    float bombCount;          // 0x4
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
    ZunBool FUN_00451d50();
    void FUN_0044d150();
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
};

DIFFABLE_EXTERN(Player, g_Player);

} /* namespace th08 */
