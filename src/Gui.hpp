#pragma once

#include "AnmManager.hpp"
#include "Global.hpp"
#include "utils.hpp"

#define TRANSITION_SPRITE_ROWS 14
#define TRANSITION_SPRITE_COLS 12

namespace th08
{

union MsgRawInstrArgs {
    // TODO: probably fill this in for starters
};

struct MsgRawInstr
{
    u16 time;
    u8 opcode;
    u8 instructionSize;
    MsgRawInstrArgs args;
};

struct MsgRawHeader
{
    i32 numInstrs;
    MsgRawInstr *instrs;
};

struct GuiMsgVm
{
    MsgRawHeader *msgFile;
    MsgRawInstr *currentInstr;
    i32 currentMsgIdx;
    ZunTimer timer;
    i32 framesElapsedDuringPause;
    i32 unk_1c;
    AnmVm portraits[4];
    AnmVm dialogueLines[2];
    AnmVm introLines[2];
    COLORREF textColorsA[4];
    COLORREF textColorsB[4];
    u32 fontSize;
    u32 ignoreWaitCounter;
    bool dialogueSkippable;
    u8 textColorIdx;
    bool unk_156a;
    u8 unk_156b;
    u8 unk_156c;
    bool isTextBoxVisible;
    u8 selectedOption;
    u8 unk_156f;
};
C_ASSERT(sizeof(GuiMsgVm) == 0x1570);

typedef enum GuiDisplayArg
{
    GUI_DISPLAY_HIDDEN = 0,
    GUI_DISPLAY_SHOWN = 1,
    GUI_DISPLAY_FULL_POWER = 1,
    GUI_DISPLAY_BORDER = 2,
    GUI_DISPLAY_CHERRY_MAX = 3,
    GUI_DISPLAY_BORDER_BONUS = 4,
    GUI_DISPLAY_SPELL_BONUS_FAILED = 5,
    GUI_DISPLAY_LAST_SPELL_FAILED = 6,
} GuiDisplayArg;

struct GuiFormattedText
{
    Float3 pos;
    i32 fmtArg;
    i32 displayArg;
    ZunTimer timer;
};

struct GuiFlags
{
    u32 lifeDisplayUpdateFrames : 2;
    u32 bombDisplayUpdateFrames : 2;
    u32 powerDisplayUpdateFrames : 2;
    u32 grazeDisplayUpdateFrames : 2;
    u32 pointDisplayUpdateFrames : 2;
    u32 timeDisplayUpdateFrames : 2;
};

#define MAX_BOSS_LIFEBAR_SEGMENTS 8

struct Gui
{
    static ChainCallbackResult OnUpdate(Gui *gui);
    static ChainCallbackResult OnDraw(Gui *gui);

    static ZunResult AddedCallback(Gui *gui);
    static ZunResult DeletedCallback(Gui *gui);

    static ZunResult RegisterChain();
    static void CutChain();

    i32 MsgWait();
    i32 IsDialoguePresent();
    void UpdateStageElements();
    void DrawGameScene();
    void DrawStageElements();
    ZunBool IsStageFinished();
    ZunBool IsDialogueSkippable();
    void ShowBonusScore(i32 score);
    void ShowPopupText(i32 fmtArg, i32 popupType);
    void ShowSpellcardBonus(i32 bonus);
    static void CopyEnemyNameTexture(i32 param_1);
    static void FUN_00438046();
    void DrawStageClearScreen();
    void DrawAsciiText();
    void CaptureArcade();
    static ZunBool IsInitialStageLoad();
    static ZunBool IsResourceReleaseDisabled();
    static ZunBool IsResourceReloadDisabled();
    i32 ShowClockTime();
    i32 FlashClockTimeSlow();
    i32 FlashClockTimeFast();
    i32 HideClockTime();
    ZunResult ActualAddedCallback();
    void InitStageClearScreen();
    ZunResult LoadMsg(const char *path);
    void FreeMsgFile();
    void MsgRead(i32 msgIdx);

    u32 frameCounter;
    GuiFlags flags;
    struct GuiImpl *impl;
    AnmLoaded *frontAnm;
    AnmLoaded *stageTextAnm;
    AnmLoaded *timesAnm;
    AnmLoaded *loadingPortraitAnm;
    u32 bossUIOpacity;
    i32 eclSetLives;
    i32 spellcardSecondsRemaining;
    i32 previousSpellcardSecondsRemaining;
    bool bossPresent;
    f32 bossLifeBarMaxSize;
    f32 bossLifeBarSize;
    unknown_fields(0x38, 0x4);
    f32 bossLifeBarSegmentStop[MAX_BOSS_LIFEBAR_SEGMENTS];
    f32 bossLifeBarSegmentStart[MAX_BOSS_LIFEBAR_SEGMENTS];
    i32 bossLifeBarSegmentColor[MAX_BOSS_LIFEBAR_SEGMENTS];
};
C_ASSERT(sizeof(Gui) == 0x9c);

DIFFABLE_EXTERN(Gui, g_Gui);

} /* namespace th08 */
