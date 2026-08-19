#pragma once
#include "Global.hpp"
#include "ZunResult.hpp"
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "utils.hpp"
#include <windows.h>

namespace th08
{

struct StdRawHeader
{
    i16 nbObjects;
    i16 nbQuads;
    u32 quadsOffset;
    u32 scriptOffset;
    i32 unk_c;
    char stageName[128];
    char songNames[4][128];
    char songPaths[4][128];
};
C_ASSERT(sizeof(StdRawHeader) == 0x490);

struct StdRawQuadBasic
{
    i16 type;
    i16 byteSize;
    i16 anmScript;
    i16 vmIndex;
    Float3 position;
    Float2 size;
};

struct StdRawObject
{
    u16 id;
    i8 zLevel;
    i8 flags;
    Float3 position;
    Float3 size;
    StdRawQuadBasic firstQuad;
};

struct StdRawInstance
{
    i16 id;
    i16 unknown;
    Float3 position;
};

struct StdRawInstr
{
    i32 frame;
    i16 opcode;
    i16 size;
    i32 args[3];
};

struct BackgroundUnkVectors
{
    BackgroundUnkVectors();

    Float3 vector0;
    Float3 vector1;
    Float3 vector2;
    Float3 vector3;
    Float3 vector4;
    Float3 vector5;
    f32 angle;
};
C_ASSERT(sizeof(BackgroundUnkVectors) == 0x4c);

struct Background
{
    Background();

    static ChainCallbackResult OnUpdate(Background *background);
    static ChainCallbackResult OnDrawHighPrio(Background *background);
    static ChainCallbackResult OnDrawLowPrio(Background *background);
    static ZunResult AddedCallback(Background *background);
    static ZunResult RegisterChain(i32 stage);
    static ZunResult DeletedCallback(Background *background);
    static void CutChain();
    ZunResult LoadStageData(const char *path);
    u32 FUN_00409f40();
    u8 FUN_0040a1b0(u32 zLevel);
    void background_fun_00416ad0();
    void background_fun_00415ce0();
    void FUN_00409160(u32 color);

    void SetCamera1();
    void SetCamera2();

    AnmVm *quadVms;
    AnmVm vm0x4;
    AnmVm vm0x2a8;
    AnmVm vm0x54c;
    AnmLoaded *anm0x7f0;
    StdRawHeader *stdData;
    i32 quadCount;
    i32 objectsCount;
    StdRawObject **objects;
    StdRawInstance *objectInstances;
    StdRawInstr *beginningOfScript;
    ZunTimer timer0x80c;
    i32 instructionIndex;
    i32 stageFrameCounter;
    i32 stage;
    Float3 position0x824;
    u8 unknown0x830[8];
    ZunTimer timer0x838;
    AnmVm vm0x844;
    u8 unknown0xae8[0x2c];
    ZunTimer timer0xb14;
    u8 unknown0xb20[4];
    u8 skyFogNeedsSetup; // Leftover from earlier games. Never checked in IN
    u8 unknown0xb25[0xf];
    i32 unk_b34;
    AnmVm vms0xb38[0x20];
    AnmVm vm0x5fb8;
    void (*callback0x625c)();
    i32 unknown0x6260;
    BackgroundUnkVectors vectors0x6264;
    BackgroundUnkVectors vectors0x62b0;
    BackgroundUnkVectors vectors0x62fc;
    BackgroundUnkVectors vectors0x6348;
    BackgroundUnkVectors vectors0x6394;
    u8 unknown0x63e0[0x14];
    ZunTimer timers0x63f4[5];
    u8 unknown0x6430[0x14];
    Float3 position0x6444;
    u8 unknown0x6450[4];
    Float3 position0x6454;
    u8 unknown0x6460[0x20];
    Float3 positions0x6480[0x20];
};
C_ASSERT(sizeof(Background) == 0x6600);

DIFFABLE_EXTERN(Background, g_Background);
}; // Namespace th08
