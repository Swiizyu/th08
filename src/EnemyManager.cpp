#include "th_pch.h"

#include "EnemyManager.hpp"
#include "AsciiManager.hpp"
#include "Background.hpp"
#include "BulletManager.hpp"
#include "EffectManager.hpp"
#include "EclManager.hpp"
#include "Spellcard.hpp"
#include "ScreenEffect.hpp"
#include "SoundPlayer.hpp"
#include "ItemManager.hpp"
#include "Gui.hpp"
#include "Player.hpp"
#include "ReplayManager.hpp"

u32 FUN_004338b0();
u32 FUN_004338c0();

namespace th08
{

u32 IsDisableResourceReload();

DIFFABLE_STATIC(EnemyManager, g_EnemyManager);
DIFFABLE_STATIC(EclManager, g_EclManager);
DIFFABLE_STATIC(i32, g_EnemyManagerUnknown);
DIFFABLE_STATIC(EffectManager, g_EffectManager);
DIFFABLE_STATIC(i32, g_EffectManagerState);
void FUN_004235a0();

DIFFABLE_STATIC(AnmVm, g_EclEffectVm0);
DIFFABLE_STATIC(AnmVm, g_EclEffectVm1);
DIFFABLE_STATIC_ARRAY_ASSIGN(const char *, 9, g_StageEnemyAnms) = {
    "stg1enm.anm", "stg2enm.anm", "stg3enm.anm", "stg4aenm.anm", "stg4benm.anm",
    "stg5enm.anm", "stg6enm.anm", "stg7enm.anm", "stg8enm.anm",
};
DIFFABLE_STATIC_ARRAY_ASSIGN(const char *, 9, g_StageEclFiles) = {
    "ecldata1.ecl", "ecldata2.ecl", "ecldata3.ecl", "ecldata4a.ecl", "ecldata4b.ecl",
    "ecldata5.ecl", "ecldata6.ecl", "ecldata7.ecl", "ecldata8.ecl",
};
DIFFABLE_STATIC_ARRAY_ASSIGN(const char *, 9, g_StageSpellEclFiles) = {
    "ecldata1sp.ecl", "ecldata2sp.ecl", "ecldata3sp.ecl", "ecldata4asp.ecl", "ecldata4bsp.ecl",
    "ecldata5sp.ecl", "ecldata6sp.ecl", "ecldata7sp.ecl", "ecldata8sp.ecl",
};
DIFFABLE_STATIC_ARRAY_ASSIGN(const char *, 9, g_EffectAnms) = {
    "eff01.anm", "eff02.anm", "eff03.anm", "eff04a.anm", "eff04b.anm",
    "eff05.anm", "eff06.anm", "eff07.anm", "eff08.anm",
};

struct EffectTemplate
{
    i32 scriptIdx;
    EffectCallback updateCallback;
    EffectCallback initCallback;
};
DIFFABLE_STATIC_ARRAY_ASSIGN(EffectTemplate, 66, g_EffectTemplates) = {
    {28, NULL, NULL},
    {29, NULL, NULL},
    {30, NULL, NULL},
    {31, &Effect::FUN_00425e60, &Effect::FUN_00425ea0},
    {36, &Effect::FUN_00425e60, &Effect::FUN_00425d70},
    {37, &Effect::FUN_00425e60, &Effect::FUN_00425d70},
    {38, &Effect::FUN_00425e60, &Effect::FUN_00425d70},
    {39, &Effect::FUN_00425e60, &Effect::FUN_00425d70},
    {40, &Effect::FUN_00425e60, &Effect::FUN_00425d70},
    {41, &Effect::FUN_00425e60, &Effect::FUN_00425d70},
    {42, &Effect::FUN_00425e60, &Effect::FUN_00425d70},
    {43, &Effect::FUN_00425e60, &Effect::FUN_00425d70},
    {44, NULL, NULL},
    {45, &Effect::FUN_00426030, &Effect::FUN_00425fe0},
    {45, &Effect::FUN_00426030, &Effect::FUN_00425fe0},
    {45, &Effect::FUN_00426030, &Effect::FUN_00425fe0},
    {0, NULL, NULL},
    {32, &Effect::FUN_00426bb0, &Effect::FUN_00426b20},
    {33, &Effect::FUN_00426c90, &Effect::FUN_00426b20},
    {51, &Effect::FUN_00426d70, &Effect::FUN_00426e70},
    {56, NULL, NULL},
    {52, &Effect::FUN_004271a0, &Effect::FUN_004270c0},
    {54, &Effect::FUN_00426c40, NULL},
    {104, &Effect::FUN_00427250, NULL},
    {104, &Effect::FUN_00427250, NULL},
    {35, NULL, NULL},
    {53, &Effect::FUN_004271a0, &Effect::FUN_004270c0},
    {34, &Effect::FUN_00426bb0, &Effect::FUN_00426b20},
    {57, NULL, NULL},
    {58, NULL, NULL},
    {59, NULL, NULL},
    {60, NULL, NULL},
    {48, NULL, NULL},
    {49, NULL, NULL},
    {50, NULL, NULL},
    {88, &Effect::FUN_00427990, &Effect::FUN_004272e0},
    {88, NULL, &Effect::FUN_00411720},
    {92, NULL, &Effect::FUN_00411a80},
    {71, NULL, NULL},
    {76, &Effect::FUN_00427990, &Effect::FUN_004272e0},
    {81, &Effect::FUN_004279d0, &Effect::FUN_004272e0},
    {82, &Effect::FUN_0040eb50, &Effect::FUN_004272e0},
    {83, &Effect::FUN_0040e040, &Effect::FUN_004272e0},
    {83, &Effect::FUN_0040e120, &Effect::FUN_004272e0},
    {83, &Effect::FUN_0040e200, &Effect::FUN_004272e0},
    {83, &Effect::FUN_0040e2d0, &Effect::FUN_004272e0},
    {84, &Effect::FUN_00410bb0, &Effect::FUN_004272e0},
    {72, NULL, NULL},
    {85, &Effect::FUN_00413070, &Effect::FUN_004272e0},
    {86, &Effect::FUN_00427990, &Effect::FUN_004272e0},
    {80, &Effect::FUN_00427a60, &Effect::FUN_004272e0},
    {73, &Effect::FUN_004264f0, &Effect::FUN_00426280},
    {77, &Effect::FUN_00427990, &Effect::FUN_004272e0},
    {88, &Effect::FUN_00427ae0, &Effect::FUN_004272e0},
    {88, &Effect::FUN_00427ae0, &Effect::FUN_004272e0},
    {87, &Effect::FUN_004279d0, &Effect::FUN_004272e0},
    {96, &Effect::FUN_004279d0, &Effect::FUN_00427970},
    {55, NULL, NULL},
    {100, &Effect::FUN_004279d0, &Effect::FUN_00427970},
    {78, &Effect::FUN_00427990, &Effect::FUN_004272e0},
    {102, NULL, &Effect::FUN_00427260},
    {103, NULL, &Effect::FUN_00427260},
    {75, NULL, NULL},
    {74, &Effect::FUN_00426990, &Effect::FUN_00426720},
    {77, &Effect::FUN_00427b50, &Effect::FUN_004272e0},
    {98, &Effect::FUN_004279d0, &Effect::FUN_00427970},
};
DIFFABLE_STATIC(ChainElem, g_EffectManagerCalcChain);
DIFFABLE_STATIC(ChainElem, g_EffectManagerDrawChain);
DIFFABLE_STATIC(ChainElem, g_EnemyManagerCalcChain);
DIFFABLE_STATIC(ChainElem, g_EnemyManagerDrawChainHighPrio);
DIFFABLE_STATIC(ChainElem, g_EnemyManagerDrawChainLowPrio);

// FUNCTION: th08 0x40b4c0
f32 Float3::FUN_0040b4c0()
{
    return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

// FUNCTION: th08 0x40b500
f32 Float3::FUN_0040b500()
{
    return this->x * this->x + this->y * this->y + this->z * this->z;
}

// FUNCTION: th08 0x40b540
f32 __fastcall Float3::FUN_0040b540(Float3 *other)
{
    return this->x * other->x + this->y * other->y + this->z * other->z;
}

// FUNCTION: th08 0x41f400
EnemyFloat3Holder::EnemyFloat3Holder()
{
}

// FUNCTION: th08 0x423d70
Float3 *Float3::operator*=(f32 scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return this;
}

// FUNCTION: th08 0x42eb10
f32 FUN_0042eb10(f32 start, f32 end, f32 factor)
{
    f32 distance1;
    f32 distance2;

    if (start < end)
    {
        distance1 = end - start;
        distance2 = start + ZUN_2PI - end;
    }
    else
    {
        distance1 = start - end;
        distance2 = end + ZUN_2PI - start;
        start = end;
    }

    if (distance1 < distance2)
    {
        return distance1 * factor + start;
    }
    return distance2 * factor + start;
}

// FUNCTION: th08 0x41fdf0
void ZunTimer::operator+=(i32 value)
{
    this->Increment(value);
}

// FUNCTION: th08 0x42c3b0
#pragma var_order(interval)
void EnemyManager::FUN_0042c3b0()
{
    i32 interval;

    if (!g_Gui.IsDialoguePresent())
    {
        interval = 2400 - g_GameManager.GetLives() * 240;
        if (this->timer.current != this->timer.previous && this->timer.current % interval == 0)
        {
            g_GameManager.IncreaseSubrank(100);
        }
    }
}

// FUNCTION: th08 0x42a210
EnemyManager::EnemyManager()
{
    this->Initialize();
}

// FUNCTION: th08 0x42a280
Enemy::Enemy()
{
}

// FUNCTION: th08 0x42a450
EnemyUnkStruct2::EnemyUnkStruct2()
{
}

// FUNCTION: th08 0x42a490
EnemyUnkStruct0x1c::EnemyUnkStruct0x1c()
{
}

// FUNCTION: th08 0x42a4c0
EnemyUnkStruct3::EnemyUnkStruct3()
{
}

// FUNCTION: th08 0x4286e0
void Float3::FromAngleMagnitude(float angle, float magnitude)
{
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul [magnitude]
        fstp [eax]
        fmul [magnitude]
        fstp [eax + 4]
    }
}

// FUNCTION: th08 0x428700
void Float3::FUN_00428700(float angle, float magnitudeX, float magnitudeY)
{
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul [magnitudeX]
        fstp [eax]
        fmul [magnitudeY]
        fstp [eax + 4]
    }
}

// FUNCTION: th08 0x428720
ZunBool AnmVm::FUN_00428720()
{
    return this->currentInstruction == NULL;
}

// FUNCTION: th08 0x42fe70
void __fastcall AnmVm::FUN_0042fe70(AnmVm *other)
{
    __asm
    {
        mov esi, other
        mov edi, this
        mov ecx, 0xa9
        rep movsd
    }
}

// FUNCTION: th08 0x42fea0
void __fastcall AnmVm::FUN_0042fea0(AnmVm *other, AnmVm *reference, i32 spriteOffset)
{
    i32 sprite = other->activeSpriteIndex + spriteOffset;
    if (this->activeSpriteIndex == sprite || reference == NULL || reference->loadedSprite == NULL)
    {
        return;
    }
    if (this->anmFile != NULL)
    {
        this->anmFile->SetSprite(this, sprite);
    }
}

// FUNCTION: th08 0x42a410
EnemyUnkStruct4::EnemyUnkStruct4()
{
    memset(this, 0, sizeof(EnemyUnkStruct4));
    this->unk0x204 = -1;
}

// FUNCTION: th08 0x40d3d0
ZunBool ZunTimer::FUN_0040d3d0()
{
    return this->current != this->previous;
}

// FUNCTION: th08 0x40e350
ZunBool ZunTimer::FUN_0040e350(i32 value)
{
    return this->current != this->previous && this->current == value;
}

// FUNCTION: th08 0x40ebc0
ZunBool ZunTimer::FUN_0040ebc0(i32 value)
{
    return this->current != this->previous && this->current % value == 0;
}

// FUNCTION: th08 0x40e040
#pragma var_order(factor)
i32 Effect::FUN_0040e040()
{
    f32 factor;

    factor = 1.0f - (f32)this->timer / 40.0f;
    factor *= factor;
    factor = 1.0f - factor;
    *(f32 *)((u8 *)this + 0x314) = 256.0f * factor;
    *(i32 *)((u8 *)this + 0x324) = 64;
    *(u8 *)((u8 *)this + 0x356) = 1;
    *(f32 *)((u8 *)this + 0x334) = 5.0f;
    *(i32 *)((u8 *)this + 0x330) = 0;
    if (this->timer < 40)
    {
        *(f32 *)((u8 *)this + 0x320) = 8.0f;
    }
    else
    {
        *(f32 *)((u8 *)this + 0x32c) = 64.0f * factor;
        *(f32 *)((u8 *)this + 0x320) += 2.0f;
    }
    return 1;
}

// FUNCTION: th08 0x40e120
#pragma var_order(factor)
i32 Effect::FUN_0040e120()
{
    f32 factor;

    factor = 1.0f - (f32)this->timer / 40.0f;
    factor *= factor;
    factor = 1.0f - factor;
    *(f32 *)((u8 *)this + 0x314) = 256.0f * factor;
    *(i32 *)((u8 *)this + 0x324) = 48;
    *(u8 *)((u8 *)this + 0x356) = 1;
    *(i32 *)((u8 *)this + 0x334) = 0;
    *(f32 *)((u8 *)this + 0x32c) = 128.0f * factor;
    *(f32 *)((u8 *)this + 0x330) = ZUN_PI / 4.0f;
    if (this->timer < 40)
    {
        *(f32 *)((u8 *)this + 0x320) = 8.0f;
    }
    else
    {
        *(f32 *)((u8 *)this + 0x320) += 1.5f;
    }
    return 1;
}

// FUNCTION: th08 0x40e200
#pragma var_order(factor)
i32 Effect::FUN_0040e200()
{
    f32 factor;

    factor = 1.0f - (f32)this->timer / 40.0f;
    factor *= factor;
    factor = 1.0f - factor;
    *(f32 *)((u8 *)this + 0x314) = 256.0f * factor;
    *(i32 *)((u8 *)this + 0x324) = 48;
    *(u8 *)((u8 *)this + 0x356) = 1;
    *(f32 *)((u8 *)this + 0x32c) = 128.0f * factor;
    *(f32 *)((u8 *)this + 0x330) = -ZUN_PI / 4.0f;
    if (this->timer < 40)
    {
        *(f32 *)((u8 *)this + 0x320) = 8.0f;
    }
    else
    {
        *(f32 *)((u8 *)this + 0x320) += 1.5f;
    }
    return 1;
}

// FUNCTION: th08 0x40e2d0
i32 Effect::FUN_0040e2d0()
{
    f32 factor;

    factor = 1.0f - (f32)this->timer / 40.0f;
    factor *= factor;
    factor = 1.0f - factor;
    *(f32 *)((u8 *)this + 0x314) = 192.0f * factor;
    *(i32 *)((u8 *)this + 0x324) = 8;
    *(u8 *)((u8 *)this + 0x356) = 1;
    *(f32 *)((u8 *)this + 0x320) = 8.0f;
    return 1;
}

// FUNCTION: th08 0x40eb50
i32 Effect::FUN_0040eb50()
{
    *(u8 *)((u8 *)this + 0x356) = 1;
    *(i32 *)((u8 *)this + 0x324) = 48;
    *(f32 *)((u8 *)this + 0x320) = 32.0f;
    *(f32 *)((u8 *)this + 0x314) = 64.0f + (f32)((this->timer.AsFrames() & 1) ? 8 : 0);
    *(i32 *)((u8 *)this + 0x318) = 0;
    return 1;
}

// FUNCTION: th08 0x410bb0
i32 Effect::FUN_00410bb0()
{
    *(f32 *)((u8 *)this + 0x314) += 8.0f;
    *(u8 *)((u8 *)this + 0x356) = 1;
    *(i32 *)((u8 *)this + 0x324) = 12;
    *(f32 *)((u8 *)this + 0x320) = 32.0f;
    return 1;
}

// FUNCTION: th08 0x4114e0
#pragma var_order(factor)
i32 Effect::FUN_004114e0()
{
    f32 factor;

    *(u8 *)((u8 *)this + 0x356) = 1;
    if (this->timer <= 40)
    {
        factor = 1.0f - (f32)this->timer.AsFrames() / 40.0f;
        *(f32 *)((u8 *)this + 0x320) = ZUN_PI - g_Rng.GetRandomF32() * (2.0f * ZUN_PI) / 40.0f;
        *(f32 *)((u8 *)this + 0x314) = 64.0f - 64.0f * factor * factor;
        (*(i32 *)((u8 *)this + 0x324))--;
    }
    else
    {
        this->active = 0;
    }
    return 1;
}

// FUNCTION: th08 0x4117b0
#pragma var_order(factor, angle)
i32 Effect::FUN_004117b0()
{
    f32 angle;
    f32 factor;

    angle = *(f32 *)((u8 *)this + 0x318) +
            ((*(i32 *)((u8 *)this + 0x328) & 1) ? 0.039269908f : -0.039269908f);
    *(f32 *)((u8 *)this + 0x318) = sin(angle) * 1.5707964f;
    *(u8 *)((u8 *)this + 0x356) = 1;
    if (this->timer <= 50)
    {
        factor = 1.0f - (f32)this->timer.AsFrames() / 50.0f;
        angle = ((f32)(*(i32 *)((u8 *)this + 0x328) - 4) * 0.4f) + 1.0f;
        *(f32 *)((u8 *)this + 0x320) = ZUN_PI - g_Rng.GetRandomF32() * (2.0f * ZUN_PI) / 50.0f;
        *(f32 *)((u8 *)this + 0x314) = 64.0f +
                                              (f32)(*(i32 *)((u8 *)this + 0x328) - 4) * 0.4f -
                                              angle * factor * factor;
        (*(i32 *)((u8 *)this + 0x324))--;
    }
    else
    {
        this->active = 0;
    }
    return 1;
}

// FUNCTION: th08 0x411720
#pragma var_order(pos2, pos)
i32 Effect::FUN_00411720()
{
    Float3 pos = this->position;
    Float3 pos2 = this->custom;
    g_EffectManager.SpawnSpecialEffect2(35, &pos, &pos2, *(i32 *)((u8 *)this + 0x328), 1, -1);
    this->updateCallback = &Effect::FUN_004114e0;
    *(i32 *)((u8 *)this + 0x324) = 44;
    *(f32 *)((u8 *)this + 0x320) = 4.0f;
    return 0;
}

// FUNCTION: th08 0x411a80
#pragma var_order(pos2, pos)
i32 Effect::FUN_00411a80()
{
    Float3 pos = this->position;
    Float3 pos2 = this->custom;
    g_EffectManager.SpawnSpecialEffect2(35, &pos, &pos2, *(i32 *)((u8 *)this + 0x328), 1, -1);
    this->updateCallback = &Effect::FUN_004117b0;
    *(i32 *)((u8 *)this + 0x324) = 54;
    *(f32 *)((u8 *)this + 0x320) = 6.0f;
    return 0;
}

// FUNCTION: th08 0x413070
#pragma var_order(factor)
i32 Effect::FUN_00413070()
{
    f32 factor;

    if (this->timer == 30)
    {
        *(f32 *)((u8 *)this + 0x314) = 192.0f;
        *(i32 *)((u8 *)this + 0x324) = 48;
        *(f32 *)((u8 *)this + 0x320) = 3.0f;
        *(f32 *)((u8 *)this + 0x32c) = 0.0001f;
        *(f32 *)((u8 *)this + 0x330) = ZUN_PI / 2.0f;
    }
    else
    {
        factor = ((f32)this->timer - 30.0f) / 30.0f;
        factor = factor * factor * factor;
        *(f32 *)((u8 *)this + 0x32c) = 192.0f * factor + 0.0001f;
        *(f32 *)((u8 *)this + 0x320) = 80.0f * factor + 3.0f;
    }
    *(u8 *)((u8 *)this + 0x356) = 1;
    return 1;
}

// FUNCTION: th08 0x425d70
#pragma var_order(randomY, randomX)
i32 Effect::FUN_00425d70()
{
    this->velocity[0] = (g_Rng.GetRandomF32InRange(256.0f) - 128.0f) / 12.0f;
    this->velocity[1] = (g_Rng.GetRandomF32InRange(256.0f) - 128.0f) / 12.0f;
    this->velocity[2] = 0.0f;
    this->acceleration = -this->velocity / 19.0f;
    this->velocity *= g_Supervisor.framerateMultiplier;
    this->acceleration *= g_Supervisor.framerateMultiplier;
    return 0;
}

// FUNCTION: th08 0x425e60
i32 Effect::FUN_00425e60()
{
    *(Float3 *)((u8 *)this + 0x2a4) += *(Float3 *)((u8 *)this + 0x2bc);
    *(Float3 *)((u8 *)this + 0x2bc) += *(Float3 *)((u8 *)this + 0x2c8);
    return 1;
}

// FUNCTION: th08 0x425ea0
#pragma var_order(randomY, randomX)
i32 Effect::FUN_00425ea0()
{
    this->velocity[0] = (g_Rng.GetRandomF32InRange(256.0f) - 128.0f) * 4.0f / 33.0f;
    this->velocity[1] = (g_Rng.GetRandomF32InRange(256.0f) - 128.0f) * 4.0f / 33.0f;
    this->velocity[2] = 0.0f;
    this->acceleration = -this->velocity / 20.0f;
    this->velocity *= g_Supervisor.framerateMultiplier;
    this->acceleration *= g_Supervisor.framerateMultiplier;
    return 0;
}

// FUNCTION: th08 0x425fe0
i32 Effect::FUN_00425fe0()
{
    *(u8 *)((u8 *)this + 0x354) = 2;
    *(i32 *)((u8 *)this + 0x2ec) = 0;
    *(i32 *)((u8 *)this + 0x2f0) = 0;
    *(i32 *)((u8 *)this + 0x2f4) = 0;
    *(i32 *)((u8 *)this + 0x314) = 0;
    return 0;
}

// FUNCTION: th08 0x426030
#pragma var_order(fadeRatio, localVector, sine, matrix, cosine, normalized)
i32 Effect::FUN_00426030()
{
    Float3 normalized;
    f32 cosine;
    D3DXMATRIX matrix;
    f32 sine;
    Float3 localVector;
    f32 fadeRatio;

    D3DXVec3Normalize((D3DXVECTOR3 *)&normalized, (D3DXVECTOR3 *)&this->direction);
    sine = sinf(*(f32 *)((u8 *)this + 0x318));
    cosine = cosf(*(f32 *)((u8 *)this + 0x318));
    *(f32 *)((u8 *)this + 0x304) = normalized.x * sine;
    *(f32 *)((u8 *)this + 0x308) = normalized.y * sine;
    *(f32 *)((u8 *)this + 0x30c) = normalized.z * sine;
    *(f32 *)((u8 *)this + 0x310) = cosine;
    D3DXMatrixRotationQuaternion(&matrix, (D3DXQUATERNION *)((u8 *)this + 0x304));
    localVector.x = normalized.y;
    localVector.y = -normalized.x;
    localVector.z = 0.0f;
    if (localVector.FUN_0040b500() < 0.00001f)
    {
        normalized = Float3(1.0f, 0.0f, 0.0f);
    }
    else
    {
        D3DXVec3Normalize((D3DXVECTOR3 *)&localVector, (D3DXVECTOR3 *)&localVector);
    }
    localVector *= *(f32 *)((u8 *)this + 0x314);
    D3DXVec3TransformCoord((D3DXVECTOR3 *)&localVector, (D3DXVECTOR3 *)&localVector, &matrix);
    localVector.z *= 6.0f;
    this->position = localVector + this->emitterPosition;
    this->position.z = 0.0f;
    if (*(i8 *)((u8 *)this + 0x352))
    {
        (*(i8 *)((u8 *)this + 0x353))++;
        if (*(i8 *)((u8 *)this + 0x353) >= 16)
        {
            return 0;
        }
        fadeRatio = 1.0f - (f32)*(i8 *)((u8 *)this + 0x353) / 16.0f;
        this->vm.color1.d3dColor =
            (this->vm.color1.d3dColor & 0xffffff) | (u32)(fadeRatio * 255.0f) << 24;
        this->vm.scale.y = 2.0f - fadeRatio;
        this->vm.scale.x = this->vm.scale.y;
    }
    return 1;
}

// FUNCTION: th08 0x426280
#pragma var_order(cameraLookAtInverse)
i32 Effect::FUN_00426280()
{
    Float3 cameraLookAtInverse;

    cameraLookAtInverse = -g_Background.vectors0x6394.vector1;
    this->basePosition = g_Background.vectors0x6394.vector1 + g_Background.vectors0x6394.vector0;
    this->basePosition.x += g_Rng.GetRandomF32SignedInRange(60.0f) + cameraLookAtInverse.x / 2.0f;
    this->basePosition.y += g_Rng.GetRandomF32SignedInRange(100.0f) - 50.0f + cameraLookAtInverse.y / 2.0f;
    this->basePosition.z += g_Rng.GetRandomF32InRange(100.0f) - 100.0f + cameraLookAtInverse.z / 2.0f;
    this->velocity.x = g_Rng.GetRandomF32SignedInRange(0.001f) + this->custom.x;
    this->velocity.y = g_Rng.GetRandomF32SignedInRange(0.03f) + this->custom.y;
    this->velocity.z = -g_Rng.GetRandomF32InRange(0.1f) - 0.3f + this->custom.z;
    this->acceleration.x = g_Rng.GetRandomF32SignedInRange(0.0001f);
    this->acceleration.y = g_Rng.GetRandomF32SignedInRange(0.0001f);
    this->acceleration.z = -0.0003f;
    this->velocity = this->velocity * g_Supervisor.framerateMultiplier;
    this->acceleration = this->acceleration * g_Supervisor.framerateMultiplier;
    this->drawType = 1;
    *(f32 *)((u8 *)&this->vm + 0x288) = -9999.0f;
    *(i32 *)((u8 *)&this->vm + 0x238) = 0;
    *(i32 *)((u8 *)&this->vm + 0x244) = 0;
    *(i32 *)((u8 *)&this->vm + 0x248) = 0;
    *(i32 *)((u8 *)&this->vm + 0x24c) = 0;
    *(i32 *)((u8 *)&this->vm + 0x250) = 0;
    *(i32 *)((u8 *)&this->vm + 0x254) = 0;
    *(i32 *)((u8 *)&this->vm + 0x258) = 0;
    return 0;
}

// FUNCTION: th08 0x4264f0
#pragma var_order(localPosition, dot)
i32 Effect::FUN_004264f0()
{
    Float3 localPosition;
    f32 dot;

    this->velocity += this->acceleration;
    this->basePosition += this->velocity;
    this->position = this->basePosition;
    localPosition = this->position - g_Background.vectors0x6394.vector0;
    D3DXVec3Normalize((D3DXVECTOR3 *)&localPosition, (D3DXVECTOR3 *)&localPosition);
    dot = g_Background.vectors0x6394.vector3.FUN_0040b540(&localPosition);
    if (dot < 0.94f)
    {
        return 0;
    }
    *(u32 *)((u8 *)&this->vm + 0x1f8) |= 0x20000;
    this->vm.color2.r = (u32)(this->vm.color1.r * g_Background.vm0x844.color1.r) >> 8;
    this->vm.color2.g = (u32)(this->vm.color1.g * g_Background.vm0x844.color1.g) >> 8;
    this->vm.color2.b = (u32)(this->vm.color1.b * g_Background.vm0x844.color1.b) >> 8;
    this->vm.color2.a = (u32)(this->vm.color1.a * g_Background.vm0x844.color1.a) >> 8;
    return 1;
}

// FUNCTION: th08 0x426720
#pragma var_order(cameraLookAtInverse)
i32 Effect::FUN_00426720()
{
    Float3 cameraLookAtInverse;

    cameraLookAtInverse = -g_Background.vectors0x6394.vector1;
    this->basePosition = g_Background.vectors0x6394.vector1 + g_Background.vectors0x6394.vector0;
    this->basePosition.x += g_Rng.GetRandomF32SignedInRange(60.0f) + cameraLookAtInverse.x / 2.0f;
    this->basePosition.y += g_Rng.GetRandomF32SignedInRange(200.0f) - 200.0f + cameraLookAtInverse.y / 2.0f;
    this->basePosition.z += g_Rng.GetRandomF32InRange(100.0f) - 100.0f + cameraLookAtInverse.z / 2.0f;
    this->velocity.x = g_Rng.GetRandomF32SignedInRange(0.001f) + this->custom.x;
    this->velocity.y = g_Rng.GetRandomF32SignedInRange(0.03f) + 0.4f;
    this->velocity.z = -g_Rng.GetRandomF32InRange(0.1f) - 0.3f + this->custom.z;
    this->acceleration.x = g_Rng.GetRandomF32SignedInRange(0.0001f);
    this->acceleration.y = g_Rng.GetRandomF32SignedInRange(0.0001f);
    this->acceleration.z = -0.0003f;
    this->velocity = this->velocity * g_Supervisor.framerateMultiplier;
    this->acceleration = this->acceleration * g_Supervisor.framerateMultiplier;
    this->drawType = 1;
    *(f32 *)((u8 *)&this->vm + 0x288) = -9999.0f;
    *(i32 *)((u8 *)&this->vm + 0x238) = 0;
    *(i32 *)((u8 *)&this->vm + 0x244) = 0;
    *(i32 *)((u8 *)&this->vm + 0x248) = 0;
    *(i32 *)((u8 *)&this->vm + 0x24c) = 0;
    *(i32 *)((u8 *)&this->vm + 0x250) = 0;
    *(i32 *)((u8 *)&this->vm + 0x254) = 0;
    *(i32 *)((u8 *)&this->vm + 0x258) = 0;
    return 0;
}

// FUNCTION: th08 0x426990
#pragma var_order(localPosition, dot)
i32 Effect::FUN_00426990()
{
    Float3 localPosition;
    f32 dot;

    this->velocity += this->acceleration;
    this->basePosition += this->velocity;
    this->position = this->basePosition;
    localPosition = this->position - g_Background.vectors0x6394.vector0;
    D3DXVec3Normalize((D3DXVECTOR3 *)&localPosition, (D3DXVECTOR3 *)&localPosition);
    dot = g_Background.vectors0x6394.vector3.FUN_0040b540(&localPosition);
    if (dot < 0.94f)
    {
        return 0;
    }
    return 1;
}

// FUNCTION: th08 0x426b20
i32 Effect::FUN_00426b20()
{
    f32 angle;

    *(Float3 *)((u8 *)this + 0x2e0) = this->position;
    *(f32 *)((u8 *)this + 0x2e8) = 0.0f;
    angle = g_Rng.GetRandomF32InRange(ZUN_2PI) - ZUN_PI;
    *(f32 *)((u8 *)this + 0x2ec) = cosf(angle);
    *(f32 *)((u8 *)this + 0x2f0) = sinf(angle);
    *(f32 *)((u8 *)this + 0x2f4) = 0.0f;
    return 0;
}

// FUNCTION: th08 0x426bb0
i32 Effect::FUN_00426bb0()
{
    f32 distance;

    distance = 256.0f - (f32)*(ZunTimer *)((u8 *)this + 0x338) * 256.0f / 60.0f;
    this->position = *(Float3 *)((u8 *)this + 0x2ec) * distance + *(Float3 *)((u8 *)this + 0x2e0);
    this->position.z = 0.0f;
    return 1;
}

// FUNCTION: th08 0x426c40
i32 Effect::FUN_00426c40()
{
    if (this->vm.FUN_00428720())
    {
        return 0;
    }
    this->position = g_Player.position;
    return 1;
}

// FUNCTION: th08 0x426c90
i32 Effect::FUN_00426c90()
{
    f32 distance;

    distance = 256.0f - (f32)*(ZunTimer *)((u8 *)this + 0x338) * 256.0f / 240.0f;
    this->position = *(Float3 *)((u8 *)this + 0x2ec) * distance + *(Float3 *)((u8 *)this + 0x2e0);
    return 1;
}

// FUNCTION: th08 0x426d70
#pragma var_order(localPosition, dot)
i32 Effect::FUN_00426d70()
{
    Float3 localPosition;
    f32 dot;

    this->velocity += this->acceleration;
    this->basePosition += this->velocity;
    this->position = this->basePosition;
    localPosition = this->position - g_Background.vectors0x6394.vector0;
    D3DXVec3Normalize((D3DXVECTOR3 *)&localPosition, (D3DXVECTOR3 *)&localPosition);
    dot = g_Background.vectors0x6394.vector3.FUN_0040b540(&localPosition);
    if (dot < 0.94f)
    {
        return 0;
    }
    this->vm.SetZRotation(AddNormalizeAngle(this->vm.rotation.z, this->vm.rotation.x));
    if (this->position.z >= 0.0f)
    {
        return 0;
    }
    return 1;
}

// FUNCTION: th08 0x426e70
#pragma var_order(cameraLookAtInverse)
i32 Effect::FUN_00426e70()
{
    Float3 cameraLookAtInverse;

    cameraLookAtInverse = -g_Background.vectors0x6394.vector1;
    this->basePosition = g_Background.vectors0x6394.vector1 + g_Background.vectors0x6394.vector0;
    this->basePosition.x += g_Rng.GetRandomF32InRange(120.0f) - 60.0f + cameraLookAtInverse.x / 2.0f;
    this->basePosition.y += g_Rng.GetRandomF32InRange(200.0f) - 100.0f + cameraLookAtInverse.y / 2.0f;
    this->basePosition.z += g_Rng.GetRandomF32InRange(100.0f) - 100.0f + cameraLookAtInverse.z / 2.0f;
    this->velocity.x = g_Rng.GetRandomF32InRange(0.06f) - 0.03f + this->custom.x;
    this->velocity.y = g_Rng.GetRandomF32InRange(0.06f) - 0.03f + this->custom.y;
    this->velocity.z = g_Rng.GetRandomF32InRange(0.1f) + 0.03f + this->custom.z;
    this->acceleration.x = g_Rng.GetRandomF32InRange(0.0002f) - 0.0001f;
    this->acceleration.y = g_Rng.GetRandomF32InRange(0.0002f) - 0.0001f;
    this->velocity = this->velocity * g_Supervisor.framerateMultiplier;
    this->acceleration = this->acceleration * g_Supervisor.framerateMultiplier;
    this->drawType = 1;
    this->vm.rotation.z = g_Rng.GetRandomF32InRange(ZUN_2PI) - ZUN_PI;
    this->vm.rotation.x = g_Rng.GetRandomF32InRange(ZUN_PI / 100.0f) - ZUN_PI / 200.0f;
    return 0;
}

// FUNCTION: th08 0x4270c0
#pragma var_order(angle)
i32 Effect::FUN_004270c0()
{
    f32 angle;

    if ((f64)this->custom.x >= -990.0)
    {
        angle = AddNormalizeAngle(this->custom.x, 0.0f);
    }
    else
    {
        angle = g_Rng.GetRandomF32InRange(ZUN_2PI) - ZUN_PI;
    }
    this->emitterPosition = this->position;
    this->emitterPosition.z = 0.0f;
    this->direction.x = cosf(angle);
    this->direction.y = sinf(angle);
    this->direction.z = 0.0f;
    this->direction *= g_Rng.GetRandomF32InRange(1.5f) + 1.0f;
    return 0;
}

// FUNCTION: th08 0x4271a0
#pragma var_order(factor)
i32 Effect::FUN_004271a0()
{
    f32 factor;

    factor = (f32)this->timer / 90.0f;
    factor = 1.0f - (1.0f - factor) * (1.0f - factor);
    this->position = this->direction * factor * 128.0f + this->emitterPosition;
    this->position.z = 0.0f;
    return 1;
}

// FUNCTION: th08 0x427260
#pragma var_order(offset)
i32 Effect::FUN_00427260()
{
    Float3 offset;

    offset.FromAngleMagnitude(this->custom.x, 128.0f);
    this->position.x += offset.x;
    this->position.y += offset.y;
    this->vm.rotation.z = AddNormalizeAngle(this->custom.x, ZUN_PI / 2.0f);
    return 0;
}

// FUNCTION: th08 0x426d10
#pragma var_order(effect, i)
void Float3::FUN_00426d10()
{
    Effect *effect;
    i32 i;

    effect = (Effect *)((u8 *)&g_EffectManager + 0x1c);
    for (i = 0; i < 0x200; i++, effect = (Effect *)((u8 *)effect + 0x360))
    {
        if (*(i8 *)((u8 *)effect + 0x351) == 0x33)
        {
            *(Float3 *)((u8 *)effect + 0x2d4) += *this;
        }
    }
}

// FUNCTION: th08 0x4272e0
i32 Effect::FUN_004272e0()
{
    this->resource = g_ZunMemory.Alloc(0x1c38, "effect");
    if (this->resource == NULL)
    {
        return -1;
    }
    *(i32 *)((u8 *)this + 0x324) = 3;
    this->emitterPosition = this->position;
    this->direction = Float3(0.0f, 0.0f, 1.0f);
    this->unk0x2f8 = Float3(0.0f, -1.0f, 0.0f);
    *(i32 *)((u8 *)this + 0x318) = *(i32 *)((u8 *)this + 0x2b0);
    *(i32 *)((u8 *)this + 0x314) = *(i32 *)((u8 *)this + 0x2b4);
    *(i32 *)((u8 *)this + 0x320) = *(i32 *)((u8 *)this + 0x2b8);
    *(u8 *)((u8 *)this + 0x356) = 1;
    this->drawCallback = NULL;
    *(i32 *)((u8 *)this + 0x32c) = 0;
    *(i32 *)((u8 *)this + 0x330) = 0;
    *(i32 *)((u8 *)this + 0x334) = 0;
    *(i32 *)((u8 *)this + 0x324) = 24;
    return 0;
}

// FUNCTION: th08 0x427970
i32 Effect::FUN_00427970()
{
    this->FUN_004272e0();
    *(u8 *)((u8 *)this + 0x355) = 1;
    return 0;
}

// FUNCTION: th08 0x427990
i32 Effect::FUN_00427990()
{
    *(u8 *)((u8 *)this + 0x356) = 1;
    *(i32 *)((u8 *)this + 0x320) = *(i32 *)((u8 *)this + 0x18);
    *(i32 *)((u8 *)this + 0x314) = *(i32 *)((u8 *)this + 0x208);
    return 1;
}

// FUNCTION: th08 0x4279d0
i32 Effect::FUN_004279d0()
{
    *(i32 *)((u8 *)this + 0x324) = *(i32 *)((u8 *)this + 0x100);
    *(f32 *)((u8 *)this + 0x334) = (f32)*(i32 *)((u8 *)this + 0x104);
    *(i32 *)((u8 *)this + 0x320) = *(i32 *)((u8 *)this + 0x18);
    *(i32 *)((u8 *)this + 0x314) = *(i32 *)((u8 *)this + 0x208);
    *(i32 *)((u8 *)this + 0x32c) = *(i32 *)((u8 *)this + 0x20c);
    *(i32 *)((u8 *)this + 0x318) = *(i32 *)((u8 *)this + 0x8);
    *(i32 *)((u8 *)this + 0x330) = *(i32 *)((u8 *)this + 0x4);
    *(u8 *)((u8 *)this + 0x356) = 1;
    return 1;
}

// FUNCTION: th08 0x427a60
i32 Effect::FUN_00427a60()
{
    *(i32 *)((u8 *)this + 0x324) = 0x20;
    *(i32 *)((u8 *)this + 0x320) = *(i32 *)((u8 *)this + 0x18);
    *(i32 *)((u8 *)this + 0x314) = *(i32 *)((u8 *)this + 0x208);
    *(i32 *)((u8 *)this + 0x32c) = *(i32 *)((u8 *)this + 0x20c);
    *(u8 *)((u8 *)this + 0x356) = 1;
    if (*(ZunTimer *)((u8 *)this + 0x338) >= 120)
    {
        return 0;
    }
    return 1;
}

// FUNCTION: th08 0x427ae0
i32 Effect::FUN_00427ae0()
{
    *(u8 *)((u8 *)this + 0x356) = 1;
    *(i32 *)((u8 *)this + 0x320) = *(i32 *)((u8 *)this + 0x18);
    *(i32 *)((u8 *)this + 0x314) = *(i32 *)((u8 *)this + 0x208);
    *(i32 *)((u8 *)this + 0x32c) = *(i32 *)((u8 *)this + 0x20c);
    *(i32 *)((u8 *)this + 0x318) = *(i32 *)((u8 *)this + 0x8);
    if (*(u8 *)((u8 *)this + 0x1f3) == 0)
    {
        return 0;
    }
    return 1;
}

// FUNCTION: th08 0x427b50
i32 Effect::FUN_00427b50()
{
    *(i32 *)((u8 *)this + 0x324) = *(i32 *)((u8 *)this + 0x100);
    *(f32 *)((u8 *)this + 0x334) = (f32)*(i32 *)((u8 *)this + 0x104);
    *(i32 *)((u8 *)this + 0x320) = *(i32 *)((u8 *)this + 0x18);
    *(i32 *)((u8 *)this + 0x314) = *(i32 *)((u8 *)this + 0x114);
    *(i32 *)((u8 *)this + 0x318) = *(i32 *)((u8 *)this + 0x8);
    *(i32 *)((u8 *)this + 0x330) = *(i32 *)((u8 *)this + 0x4);
    *(u8 *)((u8 *)this + 0x356) = 1;
    *(Float3 *)((u8 *)this + 0x2e0) = *(Float3 *)((u8 *)this + 0x208);
    return 1;
}

// FUNCTION: th08 0x4253e0
Effect *EffectManager::FUN_004253e0(i32 index)
{
    return (Effect *)((u8 *)this + (index + 0x280) * 0x360 + 0x1c);
}

// FUNCTION: th08 0x4212e0
void __fastcall EndEnemySpell(void *, void *)
{
    g_Spellcard.EndSpell();
}

// FUNCTION: th08 0x4230c0
void EclExIns::FUN_004230c0(i32 value)
{
    *(i32 *)((u8 *)this + 0x30) = value;
}

// FUNCTION: th08 0x4230e0
void EclExIns::FUN_004230e0(i32 index, i32 value0, i32 value1)
{
    *(i32 *)((u8 *)this + index * 4 + 0x5c) = value0;
    *(i32 *)((u8 *)this + index * 4 + 0x3c) = value1;
}

// FUNCTION: th08 0x423110
void EclExIns::FUN_00423110(i32 index, i32 value)
{
    *(i32 *)((u8 *)this + index * 4 + 0x7c) = value;
}

// FUNCTION: th08 0x423130
void EclExIns::FUN_00423130(i32 value)
{
    *(i32 *)((u8 *)this + 0x20) = value;
}

// FUNCTION: th08 0x423400
#pragma var_order(changed)
void __fastcall EclExIns::FUN_00423400(void *)
{
    i32 changed;
    u8 *enemy;

    changed = 0;
    if (*(f32 *)((u8 *)this + 0x2d34) <= 0.0f || *(f32 *)((u8 *)this + 0x2d34) >= 384.0f)
    {
        *(f32 *)((u8 *)this + 0x2d4c) = -*(f32 *)((u8 *)this + 0x2d4c);
        changed = 1;
    }
    enemy = *(u8 **)((u8 *)this + 0x2ca0);
    if (*(f32 *)((u8 *)this + 0x2d50) >= *(f32 *)(enemy + 0x54))
    {
        *(f32 *)((u8 *)this + 0x2d50) += *(f32 *)(enemy + 0x50);
        changed = 1;
    }
    if (*(f32 *)((u8 *)this + 0x2d38) >= -64.0f)
    {
        *(f32 *)((u8 *)this + 0x2d50) = -*(f32 *)((u8 *)this + 0x2d50);
        changed = 1;
    }
    else if (*(f32 *)((u8 *)this + 0x2d38) >= 480.0f)
    {
        *(u32 *)((u8 *)this + 0x3324) &= ~0x10000000;
    }
    if (changed)
    {
        *(f32 *)((u8 *)this + 0x2d94) =
            atan2f(*(f32 *)((u8 *)this + 0x2d50), *(f32 *)((u8 *)this + 0x2d4c));
    }
}

// FUNCTION: th08 0x423530
#pragma var_order(effect)
void __fastcall EclExIns::FUN_00423530(void *)
{
    Effect *effect;
    Float3 *position = (Float3 *)((u8 *)this + 0x2d34);

    effect = g_EffectManager.SpawnSpecialEffect(56, position, 9, 1, -1);
    effect = g_EffectManager.SpawnSpecialEffect(56, position, 10, 1, -1);
    g_EffectManager.effectAnm->SetAndExecuteScriptIdx(&effect->vm, 97);
}

// FUNCTION: th08 0x423db0
#pragma var_order(effect)
void __fastcall EclExIns::FUN_00423db0(void *)
{
    Effect *effect;
    Float3 *position = (Float3 *)((u8 *)this + 0x2d34);

    effect = g_EffectManager.SpawnSpecialEffect(65, position, 9, 1, -1);
    effect = g_EffectManager.SpawnSpecialEffect(65, position, 10, 1, -1);
    g_EffectManager.effectAnm->SetAndExecuteScriptIdx(&effect->vm, 99);
}

// FUNCTION: th08 0x424170
#pragma var_order(effect)
void __fastcall EclExIns::FUN_00424170(void *)
{
    Effect *effect;

    effect = g_EffectManager.SpawnSpecialEffect(58, (Float3 *)((u8 *)this + 0x2d34), 9, 1, -1);
    effect = g_EffectManager.SpawnSpecialEffect(58, (Float3 *)((u8 *)this + 0x2d34), 10, 1, -1);
    g_EffectManager.effectAnm->SetAndExecuteScriptIdx(&effect->vm, 101);
    *(void (**)())0x4ea28c = FUN_004235a0;
}

// FUNCTION: th08 0x424130
void __fastcall EclExIns::FUN_00424130(void *)
{
    g_EffectManager.FUN_004253e0(9)->active = 0;
    g_EffectManager.FUN_004253e0(10)->active = 0;
    g_EffectManagerState = 2;
}

// FUNCTION: th08 0x4246e0
void __fastcall EclExIns::FUN_004246e0(void *)
{
    ScreenEffect::RegisterChain(SCREEN_EFFECT_PULSE, 30, 5, 0x40ffffff, 0, 21);
    ScreenEffect::RegisterChain(SCREEN_EFFECT_UNK7, 4, 120, 190, 60, 21);
}

// FUNCTION: th08 0x424730
#pragma var_order(origin, collisionSize, grazeSize, center)
void __fastcall EclExIns::FUN_00424730(void *)
{
    u8 *enemy = *(u8 **)((u8 *)this + 0x2ca0);
    Float3 origin(*(f32 *)((u8 *)this + 0x2d88) - *(f32 *)(enemy + 0x38),
                  *(f32 *)((u8 *)this + 0x2d8c) - *(f32 *)(enemy + 0x3c), 0.0f);
    Float3 collisionSize(590.0f, 160.0f, 0.0f);
    Float3 grazeSize(590.0f, 128.0f, 0.0f);
    Float3 center(origin.x + collisionSize.x / 2.0f, origin.y, 0.0f);
    f32 angle = *(f32 *)((u8 *)this + 0x14);

    if (((ZunTimer *)((u8 *)this + 0x2e14))->FUN_0040ebc0(12))
    {
        g_Player.CalcLaserHitbox(&center, &grazeSize, &origin, angle, 1);
    }
    g_Player.CalcLaserHitbox(&center, &collisionSize, &origin, angle, 0);
}

// FUNCTION: th08 0x424820
#pragma var_order(origin, collisionSize, grazeSize, center)
void __fastcall EclExIns::FUN_00424820(void *)
{
    u8 *enemy = *(u8 **)((u8 *)this + 0x2ca0);
    Float3 origin(*(f32 *)((u8 *)this + 0x2d34) - *(f32 *)(enemy + 0x38),
                  *(f32 *)((u8 *)this + 0x2d38) - *(f32 *)(enemy + 0x3c), 0.0f);
    Float3 collisionSize(590.0f, 240.0f, 0.0f);
    Float3 grazeSize(590.0f, 192.0f, 0.0f);
    Float3 center(origin.x + collisionSize.x / 2.0f, origin.y, 0.0f);
    f32 angle = *(f32 *)((u8 *)this + 0x14);

    if (((ZunTimer *)((u8 *)this + 0x2e14))->FUN_0040ebc0(12))
    {
        g_Player.CalcLaserHitbox(&center, &grazeSize, &origin, angle, 1);
    }
    g_Player.CalcLaserHitbox(&center, &collisionSize, &origin, angle, 0);
}

// FUNCTION: th08 0x424910
#pragma var_order(origin, collisionSize, grazeSize, center)
void __fastcall EclExIns::FUN_00424910(void *)
{
    u8 *enemy = *(u8 **)((u8 *)this + 0x2ca0);
    Float3 origin(*(f32 *)((u8 *)this + 0x2d88) - *(f32 *)(enemy + 0x38),
                  *(f32 *)((u8 *)this + 0x2d8c) - *(f32 *)(enemy + 0x3c), 0.0f);
    Float3 collisionSize(590.0f, 288.0f, 0.0f);
    Float3 grazeSize(590.0f, 224.0f, 0.0f);
    Float3 center(origin.x + collisionSize.x / 2.0f, origin.y, 0.0f);
    f32 angle = *(f32 *)((u8 *)this + 0x14);

    if (((ZunTimer *)((u8 *)this + 0x2e14))->FUN_0040ebc0(12))
    {
        g_Player.CalcLaserHitbox(&center, &grazeSize, &origin, angle, 1);
    }
    g_Player.CalcLaserHitbox(&center, &collisionSize, &origin, angle, 0);
}

// FUNCTION: th08 0x424a00
void __fastcall EclExIns::FUN_00424a00(void *instruction)
{
    g_ScreenEffectCounter = *(i32 *)((u8 *)instruction + 0x10);
}

// FUNCTION: th08 0x4233d0
void __fastcall EclExIns::FUN_004233d0(void *)
{
    ScreenEffect::RegisterChain(SCREEN_EFFECT_FULL_FADE_OUT, 60, 1, -1, 0, 21);
}

// FUNCTION: th08 0x423150
void Enemy::FUN_00423150()
{
    if (*(i32 *)((u8 *)this + 0x2dfc) <= 0 || *(i16 *)((u8 *)this + 0x3338) < 0)
        return;
    f32 horizontalVelocity = this->position0x2d4c.x;
    i32 state = 0;
    bool reverse = (*(u32 *)((u8 *)this + 0x3324) & 0x40000) != 0;
    if (horizontalVelocity <= -0.01f) state = reverse ? 2 : 1;
    else if (horizontalVelocity >= 0.01f) state = reverse ? 1 : 2;
    u8 previousState = *(u8 *)((u8 *)this + 0x332e);
    if (previousState == state) return;
    i32 script;
    if (state == 0)
    {
        if (previousState == 0xff) script = *(i16 *)((u8 *)this + 0x3332);
        else if (previousState == 1) script = *(i16 *)((u8 *)this + 0x3334);
        else script = *(i16 *)((u8 *)this + 0x3336);
    }
    else if (state == 1) script = *(i16 *)((u8 *)this + 0x3338);
    else script = *(i16 *)((u8 *)this + 0x333a);
    if (script >= 0 && this->vm.anmFile != NULL)
        this->vm.anmFile->SetAndExecuteScriptIdx(&this->vm, script);
    *(u8 *)((u8 *)this + 0x332e) = (u8)state;
}

// FUNCTION: th08 0x427450
int Effect::FUN_00427450()
{
    if (*(u8 *)((u8 *)this + 0x356) != 0)
    {
        *(u8 *)((u8 *)this + 0x356) = 0;
        this->timer = 0;
    }
    if (this->resource == NULL)
    {
        this->vm.pos = this->position;
        this->vm.rotation.z = AddNormalizeAngle(this->vm.rotation.z,
                                                g_Supervisor.framerateMultiplier * this->velocity.z);
        g_AnmManager->Draw2D(&this->vm);
        return 1;
    }
    i32 count = *(i32 *)((u8 *)this + 0x324);
    if (count < 0) count = 0;
    Float3 *points = (Float3 *)this->resource;
    for (i32 i = 0; i < count; i++)
    {
        this->vm.pos = points[i * 2] + this->position;
        this->vm.rotation.z = *(f32 *)((u8 *)this + 0x318) + i * *(f32 *)((u8 *)this + 0x314);
        g_AnmManager->Draw2D(&this->vm);
    }
    return 1;
}

// FUNCTION: th08 0x4235a0
void FUN_004235a0()
{
    Effect *effect9 = g_EffectManager.FUN_004253e0(9);
    Effect *effect10 = g_EffectManager.FUN_004253e0(10);
    if (effect9 != NULL && effect9->active)
    {
        effect9->vm.pos = effect9->position;
        g_AnmManager->Draw2D(&effect9->vm);
    }
    if (effect10 != NULL && effect10->active)
    {
        effect10->vm.pos = effect10->position;
        g_AnmManager->Draw2D(&effect10->vm);
    }
}

// FUNCTION: th08 0x423a60
void __fastcall Enemy::FUN_00423a60(void *)
{
    for (i32 i = 0; i < MAX_BULLETS; i++)
    {
        Bullet *bullet = &g_BulletManager.bullets[i];
        if (bullet->state == 0) continue;
        if (bullet->reimuBarrierCooldownFrames > 0)
        {
            bullet->reimuBarrierCooldownFrames--;
            continue;
        }
        Float3 next = bullet->position + bullet->velocity;
        i32 oldRegion = bullet->position.x < 0.0f ? 0 : (bullet->position.x > 384.0f ? 1 : 2);
        i32 newRegion = next.x < 0.0f ? 0 : (next.x > 384.0f ? 1 : 2);
        if (oldRegion == newRegion) continue;
        bullet->reimuBarrierCooldownFrames = 2;
        if (newRegion == 0) bullet->position.x += 384.0f;
        else if (newRegion == 1) bullet->position.x -= 384.0f;
        bullet->angle = AddNormalizeAngle(ZUN_PI - bullet->angle, 0.0f);
        f32 sine, cosine;
        fsincos(&sine, &cosine, bullet->angle);
        bullet->velocity.x = cosine * bullet->speed;
        bullet->velocity.y = sine * bullet->speed;
    }
}

// FUNCTION: th08 0x423e20
void __fastcall Enemy::FUN_00423e20(void *)
{
    for (i32 i = 0; i < MAX_BULLETS; i++)
    {
        Bullet *bullet = &g_BulletManager.bullets[i];
        if (bullet->state == 0) continue;
        if (bullet->reimuBarrierCooldownFrames > 0)
        {
            bullet->reimuBarrierCooldownFrames--;
            continue;
        }
        Float3 next = bullet->position + bullet->velocity;
        i32 oldRegion = bullet->position.y < 0.0f ? 0 : (bullet->position.y > 448.0f ? 1 : 2);
        i32 newRegion = next.y < 0.0f ? 0 : (next.y > 448.0f ? 1 : 2);
        if (oldRegion == newRegion) continue;
        bullet->reimuBarrierCooldownFrames = 2;
        if (newRegion == 0) bullet->position.y += 448.0f;
        else if (newRegion == 1) bullet->position.y -= 448.0f;
        bullet->angle = AddNormalizeAngle(-bullet->angle, 0.0f);
        f32 sine, cosine;
        fsincos(&sine, &cosine, bullet->angle);
        bullet->velocity.x = cosine * bullet->speed;
        bullet->velocity.y = sine * bullet->speed;
    }
}

// FUNCTION: th08 0x4241e0
void __fastcall Enemy::FUN_004241e0(void *)
{
    for (i32 i = 0; i < MAX_BULLETS; i++)
    {
        Bullet *bullet = &g_BulletManager.bullets[i];
        if (bullet->state == 0) continue;
        if (bullet->reimuBarrierCooldownFrames > 0)
        {
            bullet->reimuBarrierCooldownFrames--;
            continue;
        }
        Float3 next = bullet->position + bullet->velocity;
        bool outside = next.x < 0.0f || next.x > 384.0f || next.y < 0.0f || next.y > 448.0f;
        if (!outside) continue;
        bullet->reimuBarrierCooldownFrames = 2;
        f32 x = bullet->position.x - 192.0f;
        f32 y = bullet->position.y - 224.0f;
        bullet->position.x = 192.0f - y;
        bullet->position.y = 224.0f + x;
        bullet->angle = AddNormalizeAngle(bullet->angle + ZUN_PI / 2.0f, 0.0f);
        f32 sine, cosine;
        fsincos(&sine, &cosine, bullet->angle);
        bullet->velocity.x = cosine * bullet->speed;
        bullet->velocity.y = sine * bullet->speed;
    }
}

// FUNCTION: th08 0x4244f0
void Enemy::FUN_004244f0(void *)
{
    u8 *context = *(u8 **)((u8 *)this + 0x2ca0);
    i32 group = *(i32 *)(context + 0x60);
    i32 count = 0;
    Enemy *first = NULL;
    Enemy *enemy = this;
    while (*(Enemy **)((u8 *)enemy + 8) != NULL)
    {
        enemy = *(Enemy **)((u8 *)enemy + 8);
        u8 *enemyContext = *(u8 **)((u8 *)enemy + 0x2ca0);
        if (*(i32 *)(enemyContext + 0x60) != group)
        {
            continue;
        }
        *(i32 *)(enemyContext + 0x5c) = count;
        if (first == NULL) first = enemy;
        count++;
    }
    *(i32 *)(context + 0x2c) = 0;
    if (*(i32 *)(context + 0x30) != count)
    {
        if (*(i32 *)(context + 0x30) != 0) *(i32 *)(context + 0x2c) = 1;
        *(i32 *)(context + 0x30) = count;
    }
    i32 ordinal = *(i32 *)(context + 0x5c);
    (*(i32 *)(context + 0x34))++;
    if (first != NULL && ordinal != 0 && count != 0)
    {
        f32 offset = (f32)ordinal * ZUN_2PI / count;
        *(f32 *)((u8 *)this + 0x2d9c) = AddNormalizeAngle(*(f32 *)((u8 *)first + 0x2d9c), offset);
    }
}

// FUNCTION: th08 0x424e50
void Enemy::FUN_00424e50(void *)
{
    for (i32 i = 0; i < MAX_BULLETS; i++)
    {
        Bullet *bullet = &g_BulletManager.bullets[i];
        if (bullet->state == 0 || (bullet->flags & 0x100000) == 0)
        {
            continue;
        }
        Enemy *enemy = this;
        while (*(Enemy **)((u8 *)enemy + 8) != NULL)
        {
            enemy = *(Enemy **)((u8 *)enemy + 8);
            u8 *context = *(u8 **)((u8 *)enemy + 0x2ca0);
            if (*(i32 *)(context + 0x60) == 0)
            {
                Float3 delta = bullet->position - enemy->position0x2d34;
                if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z <= 4096.0f)
                {
                    *(i32 *)(context + 0x60) = 60;
                    *(i32 *)(context + 0x34) = *(i32 *)((u8 *)*(void **)((u8 *)this + 0x2ca0) + 0x34);
                }
            }
        }
    }
}

// FUNCTION: th08 0x4250d0
void Enemy::FUN_004250d0(void *)
{
    u8 *context = *(u8 **)((u8 *)this + 0x2ca0);
    for (i32 i = 0; i < MAX_BULLETS; i++)
    {
        Bullet *bullet = &g_BulletManager.bullets[i];
        if (bullet->state == 0 || (bullet->flags & 0x100000) == 0)
        {
            continue;
        }
        *(f32 *)(context + 0x38) = bullet->angle;
        g_EnemyManager.FUN_0042a680((i16)*(i32 *)(context + 0x60), &bullet->position, 800, -2, 10,
                                    context + 0x18);
        bullet->flags &= ~0x100000;
    }
}

// FUNCTION: th08 0x424a20
void __fastcall Enemy::FUN_00424a20(void *)
{
    u8 *context = *(u8 **)((u8 *)this + 0x2ca0);
    u32 mask = *(u32 *)(context + 0x18);
    for (i32 i = 0; i < MAX_BULLETS; i++)
    {
        Bullet *bullet = &g_BulletManager.bullets[i];
        if (bullet->state == 0 || (bullet->flags & mask) == 0)
        {
            continue;
        }
        AnmVm *vm = &bullet->sprites.spriteBullet;
        if (vm->type == 1)
        {
            vm->type = 0;
            vm->blendMode = 1;
            if (vm->anmFile != NULL) vm->anmFile->SetSprite(vm, vm->activeSpriteIndex + 16);
            *(u8 *)((u8 *)bullet + 0x10b4) = 1;
            bullet->velocity.FromAngleMagnitude(*(f32 *)(context + 0x38),
                                                *(f32 *)(context + 0x3c) * g_Supervisor.framerateMultiplier);
        }
        else
        {
            vm->type = 1;
            vm->blendMode = 0;
            if (vm->anmFile != NULL) vm->anmFile->SetSprite(vm, vm->activeSpriteIndex - 16);
            *(u8 *)((u8 *)bullet + 0x10b4) = 0;
            bullet->velocity.FromAngleMagnitude(bullet->angle,
                                                bullet->speed * g_Supervisor.framerateMultiplier);
        }
    }
    Enemy *enemy = this;
    i32 enabled = *(i32 *)(context + 0x1c) == 0;
    while (*(Enemy **)((u8 *)enemy + 8) != NULL)
    {
        enemy = *(Enemy **)((u8 *)enemy + 8);
        if (enabled) *(u32 *)((u8 *)enemy + 0x3328) |= 0x80;
        else *(u32 *)((u8 *)enemy + 0x3328) &= ~0x80;
    }
    g_EclEffectVm0.SetInterrupt(enabled ? 2 : 1);
    g_EclEffectVm1.SetInterrupt(enabled ? 2 : 1);
}

// FUNCTION: th08 0x424c40
void __fastcall Enemy::FUN_00424c40(void *)
{
    u8 *context = *(u8 **)((u8 *)this + 0x2ca0);
    u32 mask = *(u32 *)(context + 0x18);
    for (i32 i = 0; i < MAX_BULLETS; i++)
    {
        Bullet *bullet = &g_BulletManager.bullets[i];
        if (bullet->state == 0 || (bullet->flags & mask) == 0)
        {
            continue;
        }
        AnmVm *vm = &bullet->sprites.spriteBullet;
        if (vm->type == 1)
        {
            vm->type = 0;
            vm->blendMode = 1;
            vm->color1.a = 0;
            if (vm->anmFile != NULL) vm->anmFile->SetSprite(vm, vm->activeSpriteIndex + 16);
            *(u8 *)((u8 *)bullet + 0x10b4) = 1;
            bullet->velocity.FromAngleMagnitude(bullet->angle,
                                                *(f32 *)(context + 0x3c) * g_Supervisor.framerateMultiplier);
        }
        else if (vm->type == 0)
        {
            vm->type = 2;
            vm->FUN_0040ed50(15, 0, 0, 255);
        }
        else
        {
            vm->type = 1;
            vm->blendMode = 0;
            if (vm->anmFile != NULL) vm->anmFile->SetSprite(vm, vm->activeSpriteIndex - 16);
            *(u8 *)((u8 *)bullet + 0x10b4) = 0;
            bullet->velocity.FromAngleMagnitude(bullet->angle,
                                                bullet->speed * g_Supervisor.framerateMultiplier);
        }
    }
}

// FUNCTION: th08 0x424e00
void __fastcall EclExIns::FUN_00424e00(void *)
{
    g_Background.FUN_00409160(0xffc03030);
}

// FUNCTION: th08 0x424e20
void __fastcall EclExIns::FUN_00424e20(void *)
{
    ScreenEffect::RegisterChain(SCREEN_EFFECT_UNK7, 16, 20, 20, 20, 21);
}

// FUNCTION: th08 0x424f60
void __fastcall EclExIns::FUN_00424f60(void *)
{
    ScreenEffect::RegisterChain(SCREEN_EFFECT_PULSE, 180, 1, -1, 0, 21);
}

// FUNCTION: th08 0x424f90
void __fastcall EclExIns::FUN_00424f90(void *instruction)
{
    i32 divisor;
    f32 result;

    divisor = *(i32 *)((u8 *)instruction + 0x10);
    result = 1.0f / divisor;
    *(f32 *)((u8 *)&g_Supervisor + 0x188) = result;
}

// FUNCTION: th08 0x424fc0
void __fastcall EclExIns::FUN_00424fc0(void *)
{
    *(i32 *)((u8 *)this->enemyData + 0x18) = g_GameManager.currentSpellCardNumber;
}

// FUNCTION: th08 0x424ff0
void __fastcall EclExIns::FUN_00424ff0(void *)
{
    g_Spellcard.FUN_00415d10("\x81\x75\x83\x8a\x83\x55\x83\x8c\x83\x4e\x83\x56\x83\x87\x83\x93\x81\x76", this);
}

// FUNCTION: th08 0x425020
void __fastcall EclExIns::FUN_00425020(void *)
{
    g_Spellcard.spellcard_fun_00416160();
}

// FUNCTION: th08 0x425040
void __fastcall EclExIns::FUN_00425040(void *)
{
    *(i32 *)((u8 *)this->enemyData + 0x18) = g_GameManager.globals->spellcardsCaptured;
}

// FUNCTION: th08 0x425070
void __fastcall EclExIns::FUN_00425070(void *instruction)
{
    *(i8 *)((u8 *)&g_GameManager + 0x2c) = *(i8 *)((u8 *)instruction + 0x10);
    if (*(i8 *)((u8 *)&g_GameManager + 0x2c))
    {
        g_EclEffectVm0.SetInterrupt(2);
        g_EclEffectVm1.SetInterrupt(2);
    }
    else
    {
        g_EclEffectVm0.SetInterrupt(1);
        g_EclEffectVm1.SetInterrupt(1);
    }
}

// FUNCTION: th08 0x4251b0
#pragma var_order(i, bullet)
void __fastcall EclExIns::FUN_004251b0(void *instruction)
{
    Bullet *bullet;
    i32 i;

    *(f32 *)((u8 *)&g_Supervisor + 0x188) = 1.0f / *(i32 *)((u8 *)instruction + 0x10);
    g_EclEffectVm0.SetInterrupt(2);
    g_EclEffectVm1.SetInterrupt(2);
    bullet = &g_BulletManager.bullets[0];
    for (i = 0; i < MAX_BULLETS; i++, bullet++)
    {
        if (bullet->state == 0)
        {
            continue;
        }
        bullet->velocity *= *(f32 *)((u8 *)&g_Supervisor + 0x188);
        bullet->sprites.spriteBullet.baseSpriteIndex = bullet->sprites.spriteBullet.activeSpriteIndex;
        if (bullet->sprites.spriteBullet.activeSpriteIndex >= 96 &&
            bullet->sprites.spriteBullet.activeSpriteIndex <= 111)
        {
            g_BulletManager.bonusAnm->SetSprite(&bullet->sprites.spriteBullet, 111);
        }
    }
}

// FUNCTION: th08 0x425290
#pragma var_order(i, factor, bullet)
void __fastcall EclExIns::FUN_00425290(void *instruction)
{
    Bullet *bullet;
    f32 factor;
    i32 i;

    bullet = &g_BulletManager.bullets[0];
    factor = 1.0f / *(f32 *)((u8 *)&g_Supervisor + 0x188);
    for (i = 0; i < MAX_BULLETS; i++, bullet++)
    {
        if (bullet->state == 0)
        {
            continue;
        }
        bullet->velocity *= factor;
        if (bullet->sprites.spriteBullet.activeSpriteIndex >= 96 &&
            bullet->sprites.spriteBullet.activeSpriteIndex <= 111)
        {
            g_BulletManager.bonusAnm->SetSprite(&bullet->sprites.spriteBullet,
                                                bullet->sprites.spriteBullet.baseSpriteIndex);
        }
    }
    *(f32 *)((u8 *)&g_Supervisor + 0x188) = 1.0f / *(i32 *)((u8 *)instruction + 0x10);
    if (*(f32 *)((u8 *)&g_Supervisor + 0x188) >= 1.0f)
    {
        *(u32 *)((u8 *)&g_Supervisor + 0x1a4) |= 0x20;
    }
    *(f32 *)((u8 *)&g_Supervisor + 0x188) = 1.0f;
    g_EclEffectVm0.SetInterrupt(1);
    g_EclEffectVm1.SetInterrupt(1);
}

// FUNCTION: th08 0x425390
void __fastcall EclExIns::FUN_00425390(void *)
{
    if (*(i32 *)((u8 *)&g_Player + 0xfdc) != 0)
    {
        g_ItemManager.SpawnItem((Float3 *)((u8 *)this + 0x2d34), ITEM_BOMB, 0);
    }
    else
    {
        g_ItemManager.SpawnItem((Float3 *)((u8 *)this + 0x2d34), ITEM_EXTEND, 0);
    }
}

// FUNCTION: th08 0x42deb0
void EclExIns::FUN_0042deb0()
{
    *(Float3 *)((u8 *)this + 0x2d64) =
        *(Float3 *)((u8 *)this + 0x2d34) - *(Float3 *)((u8 *)this + 0x2d58);
    *(Float3 *)((u8 *)this + 0x2d58) = *(Float3 *)((u8 *)this + 0x2d34);

    if ((*(u32 *)((u8 *)this + 0x3324) & 0x40000) == 0)
    {
        *(f32 *)((u8 *)this + 0x2d34) +=
            *(f32 *)((u8 *)&g_Supervisor + 0x188) * *(f32 *)((u8 *)this + 0x2d4c);
    }
    else
    {
        *(f32 *)((u8 *)this + 0x2d34) -=
            *(f32 *)((u8 *)&g_Supervisor + 0x188) * *(f32 *)((u8 *)this + 0x2d4c);
    }
    *(f32 *)((u8 *)this + 0x2d38) +=
        *(f32 *)((u8 *)&g_Supervisor + 0x188) * *(f32 *)((u8 *)this + 0x2d50);
    *(f32 *)((u8 *)this + 0x2d3c) +=
        *(f32 *)((u8 *)&g_Supervisor + 0x188) * *(f32 *)((u8 *)this + 0x2d54);
}

// FUNCTION: th08 0x423390
void __fastcall EclExIns::MystiaNightBlindness(void *)
{
    *(i32 *)((u8 *)&g_AsciiManager + 93960) = *(i32 *)((u8 *)this->enemyData + 0x18);
    *(i32 *)((u8 *)&g_AsciiManager + 93956) = *(i32 *)((u8 *)this->enemyData + 0x38);
}

// FUNCTION: th08 0x4182e0
EclManager::EclManager()
{
}

// FUNCTION: th08 0x418300
EclManagerSub::EclManagerSub()
{
}

// FUNCTION: th08 0x418330
#pragma var_order(i)
ZunResult EclManager::Load(const char *path)
{
    i32 i;

    this->timelineFile = (EclTimelineHeader *)FileSystem::OpenFile(path, NULL, FALSE);
    if (this->timelineFile == NULL)
    {
        g_GameErrorContext.Log("\x93\x47\x83\x66\x81\x5b\x83\x5e\x82\xcc\x93\xc7\x82\xdd\x8d\x9e\x82\xdd\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81\x41\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xe9\x82\xa9\x8e\xb8\x82\xed\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }
    if (this->timelineFile->magic != 0x800)
    {
        g_GameErrorContext.Log("\x93\x47\x83\x66\x81\x5b\x83\x5e\x82\xcc\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x82\xaa\x88\xe1\x82\xa2\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }
    for (i = 0; i < 16; i++)
    {
        this->timelineFile->timelines[i] =
            (EclTimeline *)((u8 *)this->timelineFile->timelines[i] + (u32)this->timelineFile);
    }
    this->unknown = this->timelineFile + 1;
    for (i = 0; i < this->timelineFile->subCount; i++)
    {
        ((void **)this->unknown)[i] = (u8 *)((void **)this->unknown)[i] + (u32)this->timelineFile;
    }
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x418420
void EclManager::FUN_00418420()
{
    if (this->timelineFile != NULL)
    {
        g_ZunMemory.Free(this->timelineFile);
    }
    this->timelineFile = NULL;
}

// FUNCTION: th08 0x418450
i32 EclManager::FUN_00418450(EclTimelineContext *context, i16 timelineIndex)
{
    if (timelineIndex < 0)
    {
        return 0;
    }

    context->timeline = ((void **)this->unknown)[timelineIndex];
    context->timer1 = 0;
    context->timer2 = 0;
    context->timelineIndex = timelineIndex;
    return 0;
}

// FUNCTION: th08 0x4184b0
u32 EclManager::RunEcl(void *enemyData)
{
    if (enemyData == NULL) return 1;
    EclTimelineContext *context = (EclTimelineContext *)((u8 *)enemyData + 0x7f8);
    u8 *instruction = (u8 *)context->timeline;
    if (instruction == NULL) return 1;
    for (i32 guard = 0; guard < 256; guard++)
    {
        i32 time = *(i32 *)instruction;
        if (time < 0)
        {
            context->timeline = NULL;
            return 1;
        }
        if (context->timer1.current < time) break;
        i16 opcode = *(i16 *)(instruction + 4);
        u8 size = *(u8 *)(instruction + 6);
        if (size < 8)
        {
            context->timeline = NULL;
            return 1;
        }
        if (opcode == 0 || opcode == 1)
        {
            context->timeline = NULL;
            return 1;
        }
        instruction += size;
        context->timeline = instruction;
    }
    context->timer1.Tick();
    context->timer2.Tick();
    return 0;
}

// FUNCTION: th08 0x449f50
EclTimeline::EclTimeline()
{
}

// FUNCTION: th08 0x42dfb0
i32 EclManager::GetTimelineCount()
{
    return this->timelineFile->timelineCount;
}

// FUNCTION: th08 0x42dfd0
EclTimeline *EclManager::GetTimeline(i32 timelineIdx)
{
    return this->timelineFile->timelines[timelineIdx];
}

// FUNCTION: th08 0x4287e0
Effect::Effect()
{
}

// FUNCTION: th08 0x428740
EffectManager::EffectManager()
{
    this->ResetEffects();
    this->colorMultiplierR = 1.0f;
    this->colorMultiplierG = 1.0f;
    this->colorMultiplierB = 1.0f;
    this->colorMultiplierA = 1.0f;
}

// FUNCTION: th08 0x4284b0
ZunResult EffectManager::AddedCallback(EffectManager *effectManager)
{
    effectManager->ResetEffects();
    effectManager->effectAnm = g_AnmManager->GetAnm(6);
    if (!IsDisableResourceReload())
    {
        effectManager->effectAnm2 = g_AnmManager->PreloadAnm(9, g_EffectAnms[g_GameManager.currentStage]);
        if (effectManager->effectAnm2 == NULL)
        {
            return ZUN_ERROR;
        }
    }
    else
    {
        effectManager->effectAnm2 = g_AnmManager->GetAnm(9);
    }
    return ZUN_SUCCESS;
}

static ChainCallbackResult __fastcall EffectManagerUpdateCallback(EffectManager *effectManager)
{
    return effectManager->UpdateEffects();
}

static ChainCallbackResult __fastcall EffectManagerDrawCallback(EffectManager *effectManager)
{
    return effectManager->DrawEffects();
}

// FUNCTION: th08 0x428620
ZunResult EffectManager::RegisterChain()
{
    EffectManager *effectManager;

    effectManager = &g_EffectManager;
    effectManager->ResetEffects();
    g_EffectManagerCalcChain.SetCallback((ChainCallback)EffectManagerUpdateCallback);
    g_EffectManagerCalcChain.addedCallback = (ChainLifetimeCallback)EffectManager::AddedCallback;
    g_EffectManagerCalcChain.deletedCallback = (ChainLifetimeCallback)EffectManager::DeletedCallback;
    g_EffectManagerCalcChain.arg = effectManager;
    if (g_Chain.AddToCalcChain(&g_EffectManagerCalcChain, 13) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }
    g_EffectManagerDrawChain.SetCallback((ChainCallback)EffectManagerDrawCallback);
    g_EffectManagerDrawChain.arg = effectManager;
    g_Chain.AddToDrawChain(&g_EffectManagerDrawChain, 12);
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x428590
#pragma var_order(effect, i)
ZunResult EffectManager::DeletedCallback(EffectManager *effectManager)
{
    Effect *effect;
    i32 i;

    effect = (Effect *)((u8 *)effectManager + 0x1c);
    for (i = 0; i < 0x28d; i++, effect = (Effect *)((u8 *)effect + 0x360))
    {
        if (effect->resource != NULL)
        {
            g_ZunMemory.Free(effect->resource);
            effect->resource = NULL;
        }
    }
    if (!IsDisableResourceReload())
    {
        g_AnmManager->ReleaseAnm(9);
    }
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x427bf0
#pragma var_order(i, effect)
ChainCallbackResult EffectManager::UpdateEffects()
{
    Effect *effect;
    i32 i;

    effect = &this->effects[0];
    this->unk0x8 = 0;
    this->listTails[0] = &this->specialEffect0;
    this->listTails[1] = &this->specialEffect1;
    this->listTails[2] = &this->specialEffect2;
    this->listTails[3] = &this->specialEffect3;
    this->listTails[4] = &this->specialEffect4;
    this->specialEffect0.next = NULL;
    this->specialEffect1.next = NULL;
    this->specialEffect2.next = NULL;
    this->specialEffect3.next = NULL;
    this->specialEffect4.next = NULL;

    for (i = 0; i < 0x28d; i++, effect++)
    {
        if (!effect->active)
        {
            if (effect->resource != NULL)
            {
                g_ZunMemory.Free(effect->resource);
                effect->resource = NULL;
            }
            continue;
        }
        this->unk0x8++;
        if ((*(u32 *)((u8 *)&g_GameManager + 0x3dbac) & 0x400) == 0 || *(u8 *)((u8 *)effect + 0x357))
        {
            if (effect->updateCallback != NULL && (effect->*effect->updateCallback)() != 1)
            {
                effect->active = 0;
                continue;
            }
            if (g_AnmManager->ExecuteScript(&effect->vm) != 0)
            {
                effect->active = 0;
                continue;
            }
            effect->timer++;
        }
        effect->next = NULL;
        if (effect->effectId == 64)
        {
            continue;
        }
        i32 listIndex;
        if (effect->drawType == 1 || effect->drawType >= 3)
        {
            listIndex = 1;
        }
        else if (effect->drawType == 0)
        {
            if (*(u8 *)((u8 *)effect + 0x355))
            {
                listIndex = 3;
            }
            else if (effect->vm.blendMode == 1)
            {
                listIndex = 4;
            }
            else
            {
                listIndex = 0;
            }
        }
        else
        {
            listIndex = 2;
        }
        this->listTails[listIndex]->next = effect;
        this->listTails[listIndex] = effect;
    }
    this->frameCounter++;
    if (this->frameCounter % 300 == 100 && g_GameManager.IsTampered())
    {
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x4281e0
#pragma var_order(effectCounter, effect)
ChainCallbackResult EffectManager::FUN_004281e0()
{
    Effect *effect;
    i32 effectCounter;

    effect = this->specialEffect1.next;
    effectCounter = 0;
    if (g_Supervisor.cfg.effectQuality == 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    while (effect != NULL)
    {
        effectCounter++;
        if (g_Supervisor.cfg.effectQuality == 1 && (effectCounter & 1))
        {
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        effect->vm.pos = effect->position;
        if (effect->drawType == 4)
        {
            g_AnmManager->Draw2D(&effect->vm);
        }
        else if (effect->drawType == 1)
        {
            if (effect->effectId == 51 || effect->effectId == 63)
            {
                g_AnmManager->DrawWorld(&effect->vm);
            }
            else
            {
                g_AnmManager->DrawWorld(&effect->vm);
            }
        }
        else
        {
            g_AnmManager->FUN_00464070(&effect->vm);
        }
        effect = effect->next;
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x428310
#pragma var_order(delta, transformed)
void __fastcall FUN_00428310(Effect *effect, Float3 *position)
{
    Float3 transformed;
    Float3 delta;

    if (*(u8 *)((u8 *)&g_GameManager + 0x12) == 0 && *(u8 *)((u8 *)&g_GameManager + 0x13) == 0)
    {
        transformed = *position + effect->vm.posFinal;
        delta = effect->vm.pos2 - transformed;
        if (effect->vm.pos2.x >= -9999.0f)
        {
            delta.x += 32.0f;
            delta.y += 16.0f;
            delta.z = 0.0f;
            if (delta.FUN_0040b500() >= 25600.0f)
            {
                effect->vm.posInitial.x += 0.0005f;
                effect->vm.posFinal += delta * effect->vm.posInitial.x;
            }
        }

        delta = transformed - g_Player.position;
        delta.x -= 32.0f;
        delta.y -= 16.0f;
        delta.z = 0.0f;
        if (delta.FUN_0040b500() >= 7744.0f)
        {
            effect->vm.posFinal += delta * 0.02f;
        }
    }

    *position += effect->vm.posFinal;
}

// FUNCTION: th08 0x427f00
#pragma var_order(effect)
ChainCallbackResult EffectManager::DrawEffects()
{
    Effect *effect;

    effect = this->specialEffect0.next;
    while (effect != NULL)
    {
        if (effect->drawCallback != NULL)
        {
            effect->drawCallback(effect);
        }
        else
        {
            effect->vm.pos = effect->position;
            effect->vm.pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
            effect->vm.pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
            effect->vm.pos.z = 0.07f;
            effect->vm.pos += effect->vm.pos2;
            g_AnmManager->Draw2D(&effect->vm);
        }
        effect = effect->next;
    }

    effect = this->specialEffect2.next;
    while (effect != NULL)
    {
        effect->vm.pos = effect->position;
        g_AnmManager->DrawWorld(&effect->vm);
        effect = effect->next;
    }

    effect = this->specialEffect4.next;
    while (effect != NULL)
    {
        if (effect->drawCallback != NULL)
        {
            effect->drawCallback(effect);
        }
        else
        {
            effect->vm.pos = effect->position;
            effect->vm.pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
            effect->vm.pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
            effect->vm.pos.z = 0.07f;
            effect->vm.pos += effect->vm.pos2;
            g_AnmManager->Draw2D(&effect->vm);
        }
        effect = effect->next;
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x428100
ChainCallbackResult EffectManager::DrawUnkTypeEffects()
{
    Effect *effect;

    effect = this->specialEffect3.next;
    while (effect != NULL)
    {
        if (effect->drawCallback != NULL)
        {
            effect->drawCallback(effect);
        }
        else
        {
            effect->vm.pos = effect->position;
            effect->vm.pos.x += g_GameManager.arcadeRegionTopLeftPos.x;
            effect->vm.pos.y += g_GameManager.arcadeRegionTopLeftPos.y;
            effect->vm.pos += effect->vm.pos2;
            effect->vm.pos.z = 0.04f;
            g_AnmManager->Draw2D(&effect->vm);
        }
        effect = effect->next;
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x425410
void EffectManager::ResetEffects()
{
    memset(this, 0, sizeof(EffectManager));
}

// FUNCTION: th08 0x425430
#pragma var_order(effect, i, result)
Effect *EffectManager::SpawnEffect(i32 effectId, Float3 *position, i32 count, D3DCOLOR color)
{
    Effect *effect;
    i32 i;
    Effect *result;

    effect = &this->effects[this->unk0x0];
    for (i = 0; i < 0x200; i++)
    {
        this->unk0x0++;
        if (this->unk0x0 >= 0x200)
        {
            this->unk0x0 = 0;
        }
        if (effect->active)
        {
            if (this->unk0x0 == 0)
            {
                effect = &this->effects[0];
            }
            else
            {
                effect++;
            }
            continue;
        }
        if (effect->resource != NULL)
        {
            g_ZunMemory.Free(effect->resource);
        }
        memset(effect, 0, sizeof(Effect));
        effect->active = 1;
        effect->effectId = effectId;
        effect->position = *position;
        this->effectAnm->SetAndExecuteScriptIdx(&effect->vm, g_EffectTemplates[effectId].scriptIdx);
        effect->vm.zWriteDisabled = true;
        effect->vm.color1.d3dColor = color;
        *(i32 *)((u8 *)&effect->vm + 0x288) = 0;
        *(i32 *)((u8 *)&effect->vm + 0x28c) = 0;
        *(i32 *)((u8 *)&effect->vm + 0x290) = 0;
        effect->updateCallback = g_EffectTemplates[effectId].updateCallback;
        if (g_EffectTemplates[effectId].initCallback != NULL &&
            (effect->*g_EffectTemplates[effectId].initCallback)() != 0)
        {
            effect->active = 0;
        }
        count--;
        if (count == 0)
        {
            break;
        }
        if (this->unk0x0 == 0)
        {
            effect = &this->effects[0];
        }
        else
        {
            effect++;
        }
    }
    g_ReplayManager->inputFlags |= 0x400;
    result = i >= 0x200 ? &this->effects[0x280] : effect;
    return result;
}

// FUNCTION: th08 0x425650
Effect *EffectManager::FUN_00425650(i32 effectId, Float3 *position, Float3 *custom, i32 count, D3DCOLOR color)
{
    Effect *effect = &this->effects[this->unk0x0];
    i32 i;
    for (i = 0; i < 0x200; i++)
    {
        this->unk0x0++;
        if (this->unk0x0 >= 0x200) this->unk0x0 = 0;
        if (effect->active)
        {
            effect = this->unk0x0 == 0 ? &this->effects[0] : effect + 1;
            continue;
        }
        if (effect->resource != NULL) g_ZunMemory.Free(effect->resource);
        memset(effect, 0, sizeof(Effect));
        effect->active = 1;
        effect->effectId = effectId;
        effect->position = *position;
        this->effectAnm->SetAndExecuteScriptIdx(&effect->vm, g_EffectTemplates[effectId].scriptIdx);
        effect->vm.zWriteDisabled = true;
        effect->vm.color1.d3dColor = color;
        *(i32 *)((u8 *)&effect->vm + 0x288) = 0;
        *(i32 *)((u8 *)&effect->vm + 0x28c) = 0;
        *(i32 *)((u8 *)&effect->vm + 0x290) = 0;
        effect->updateCallback = g_EffectTemplates[effectId].updateCallback;
        effect->custom = *custom;
        if (g_EffectTemplates[effectId].initCallback != NULL &&
            (effect->*g_EffectTemplates[effectId].initCallback)() != 0)
            effect->active = 0;
        if (--count == 0) break;
        effect = this->unk0x0 == 0 ? &this->effects[0] : effect + 1;
    }
    g_ReplayManager->inputFlags |= 0x400;
    return i >= 0x200 ? &this->effects[0x28d] : effect;
}

// FUNCTION: th08 0x425b70
Effect *EffectManager::FUN_00425b70(i32 effectId, Float3 *position, i32 count, D3DCOLOR color)
{
    Effect *effect = &this->effects[0x200];
    i32 i;
    for (i = 0; i < 0x80; i++, effect++)
    {
        if (effect->active) continue;
        if (effect->resource != NULL) g_ZunMemory.Free(effect->resource);
        effect->resource = NULL;
        effect->drawCallback = NULL;
        effect->drawType = 0;
        effect->active = 1;
        effect->effectId = effectId;
        effect->position = *position;
        this->effectAnm->SetAndExecuteScriptIdx(&effect->vm, g_EffectTemplates[effectId].scriptIdx);
        effect->vm.zWriteDisabled = true;
        effect->vm.color1.d3dColor = color;
        *(i32 *)((u8 *)&effect->vm + 0x288) = 0;
        *(i32 *)((u8 *)&effect->vm + 0x28c) = 0;
        *(i32 *)((u8 *)&effect->vm + 0x290) = 0;
        effect->updateCallback = g_EffectTemplates[effectId].updateCallback;
        effect->timer = 0;
        *(u8 *)((u8 *)effect + 0x352) = 0;
        *(u8 *)((u8 *)effect + 0x353) = 0;
        effect->custom = Float3(0.0f, 0.0f, 0.0f);
        if (g_EffectTemplates[effectId].initCallback != NULL &&
            (effect->*g_EffectTemplates[effectId].initCallback)() != 0)
            effect->active = 0;
        if (--count == 0) break;
    }
    g_ReplayManager->inputFlags |= 0x400;
    return i >= 0x80 ? &this->effects[0x28d] : effect;
}

// FUNCTION: th08 0x425870
#pragma var_order(effect)
Effect *EffectManager::SpawnSpecialEffect(i32 effectId, Float3 *position, i32 specialIndex, i32 unused,
                                          D3DCOLOR color)
{
    Effect *effect;

    effect = &this->effects[0x280 + specialIndex];
    if (effect->resource != NULL)
    {
        g_ZunMemory.Free(effect->resource);
    }
    memset(effect, 0, sizeof(Effect));
    *(i32 *)((u8 *)effect + 0x328) = specialIndex;
    effect->active = 1;
    effect->effectId = effectId;
    effect->position = *position;
    if (g_EffectTemplates[effectId].scriptIdx >= 0)
    {
        this->effectAnm->SetAndExecuteScriptIdx(&effect->vm, g_EffectTemplates[effectId].scriptIdx);
    }
    effect->vm.zWriteDisabled = true;
    effect->vm.color1.d3dColor = color;
    *(i32 *)((u8 *)&effect->vm + 0x288) = 0;
    *(i32 *)((u8 *)&effect->vm + 0x28c) = 0;
    *(i32 *)((u8 *)&effect->vm + 0x290) = 0;
    effect->updateCallback = g_EffectTemplates[effectId].updateCallback;
    if (g_EffectTemplates[effectId].initCallback != NULL &&
        (effect->*g_EffectTemplates[effectId].initCallback)() != 0)
    {
        effect->active = 0;
    }
    if (g_ReplayManager != NULL)
    {
        g_ReplayManager->inputFlags |= 0x400;
    }
    return effect;
}

// FUNCTION: th08 0x4259e0
#pragma var_order(effect)
Effect *EffectManager::SpawnSpecialEffect2(i32 effectId, Float3 *position, Float3 *custom, i32 specialIndex,
                                           i32 unused, D3DCOLOR color)
{
    Effect *effect;

    effect = &this->effects[0x280 + specialIndex];
    if (effect->resource != NULL)
    {
        g_ZunMemory.Free(effect->resource);
    }
    memset(effect, 0, sizeof(Effect));
    *(i32 *)((u8 *)effect + 0x328) = specialIndex;
    effect->custom = *custom;
    effect->active = 1;
    effect->effectId = effectId;
    effect->position = *position;
    if (g_EffectTemplates[effectId].scriptIdx >= 0)
    {
        this->effectAnm->SetAndExecuteScriptIdx(&effect->vm, g_EffectTemplates[effectId].scriptIdx);
    }
    effect->vm.zWriteDisabled = true;
    effect->vm.color1.d3dColor = color;
    *(i32 *)((u8 *)&effect->vm + 0x288) = 0;
    *(i32 *)((u8 *)&effect->vm + 0x28c) = 0;
    *(i32 *)((u8 *)&effect->vm + 0x290) = 0;
    effect->updateCallback = g_EffectTemplates[effectId].updateCallback;
    effect->timer = 0;
    *(u8 *)((u8 *)effect + 0x352) = 0;
    *(u8 *)((u8 *)effect + 0x353) = 0;
    if (g_EffectTemplates[effectId].initCallback != NULL &&
        (effect->*g_EffectTemplates[effectId].initCallback)() != 0)
    {
        effect->active = 0;
    }
    if (g_ReplayManager != NULL)
    {
        g_ReplayManager->inputFlags |= 0x400;
    }
    return effect;
}

// FUNCTION: th08 0x4286b0
void EffectManager::CutChain()
{
    g_Chain.Cut(&g_EffectManagerCalcChain);
    g_Chain.Cut(&g_EffectManagerDrawChain);
}

// FUNCTION: th08 0x415c80
void Enemy::FUN_00415c80()
{
    *(f32 *)((u8 *)this + 0x2dec) = -0.5f;
    *(f32 *)((u8 *)this + 0x2df0) = 0.5f;
    *(i16 *)((u8 *)this + 0x2df4) = 0;
    *(i16 *)((u8 *)this + 0x2df6) = 0;
    *(i16 *)((u8 *)this + 0x2df8) = 0;
    *(i16 *)((u8 *)this + 0x2dfa) = 0;
}

// FUNCTION: th08 0x41efc0
Enemy *Enemy::FUN_0041efc0()
{
    Enemy *current;

    current = this;
    if (this->FUN_0041f000())
    {
        while (*(Enemy **)((u8 *)current + 8) != NULL)
        {
            current = *(Enemy **)((u8 *)current + 8);
        }
    }
    return current;
}

// FUNCTION: th08 0x41f000
ZunBool Enemy::FUN_0041f000()
{
    ZunBool result;

    if (*(i32 *)((u8 *)this + 0x2da4) == 0 && *(Enemy **)((u8 *)this + 8) != NULL)
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    return result;
}

// FUNCTION: th08 0x41f040
void Enemy::FUN_0041f040(i32 x, i32 y, i32 z)
{
    *(i32 *)((u8 *)*(void **)((u8 *)this + 0xf4) + 0x2a4) = x;
    *(i32 *)((u8 *)*(void **)((u8 *)this + 0xf4) + 0x2a8) = y;
    *(i32 *)((u8 *)*(void **)((u8 *)this + 0xf4) + 0x2ac) = z;
}

// FUNCTION: th08 0x41f0b0
void Enemy::FUN_0041f0b0(i32 value)
{
    ((EnemyFlags *)this)->flag6 = value;
}

// FUNCTION: th08 0x41f0e0
void Enemy::FUN_0041f0e0(i32 value)
{
    ((EnemyFlags *)this)->flag11 = value;
}

// FUNCTION: th08 0x41f110
Enemy *__fastcall Enemy::FUN_0041f110(void *instruction)
{
    u8 *ins = (u8 *)instruction;
    if (*(i32 *)((u8 *)this + 0x2dfc) <= 0 || (*(u32 *)((u8 *)this + 0x3324) & 0x400) != 0)
        return &g_EnemyManager.enemies[480];
    u16 mask = *(u16 *)(ins + 0xa);
    Float3 position;
    position.x = (mask & 2) ? this->FUN_00420120(*(f32 *)(ins + 0x10)) : *(f32 *)(ins + 0x10);
    position.y = (mask & 4) ? this->FUN_00420120(*(f32 *)(ins + 0x14)) : *(f32 *)(ins + 0x14);
    position.z = 0.0f;
    i32 health = (mask & 8) ? this->FUN_0041f420(*(i32 *)(ins + 0x18)) : *(i32 *)(ins + 0x18);
    i32 arg4 = (mask & 0x10) ? this->FUN_0041f420(*(i32 *)(ins + 0x1c)) : *(i32 *)(ins + 0x1c);
    i32 field = (mask & 0x20) ? this->FUN_0041f420(*(i32 *)(ins + 0x20)) : *(i32 *)(ins + 0x20);
    u8 *context = *(u8 **)((u8 *)this + 0x2ca0);
    return g_EnemyManager.FUN_0042a680(*(i16 *)(ins + 0xc), &position, health, (i8)arg4, field, context + 0x18);
}

// FUNCTION: th08 0x41f280
Enemy *__fastcall Enemy::FUN_0041f280(void *instruction)
{
    u8 *ins = (u8 *)instruction;
    if (*(i32 *)((u8 *)this + 0x2dfc) <= 0 || (*(u32 *)((u8 *)this + 0x3324) & 0x400) != 0)
        return &g_EnemyManager.enemies[480];
    u16 mask = *(u16 *)(ins + 0xa);
    Float3 position;
    position.x = (mask & 2) ? this->FUN_00420120(*(f32 *)(ins + 0x10)) : *(f32 *)(ins + 0x10);
    position.y = (mask & 4) ? this->FUN_00420120(*(f32 *)(ins + 0x14)) : *(f32 *)(ins + 0x14);
    position.z = 0.0f;
    position += this->position0x2d88;
    i32 health = (mask & 8) ? this->FUN_0041f420(*(i32 *)(ins + 0x18)) : *(i32 *)(ins + 0x18);
    i32 arg4 = (mask & 0x10) ? this->FUN_0041f420(*(i32 *)(ins + 0x1c)) : *(i32 *)(ins + 0x1c);
    i32 field = (mask & 0x20) ? this->FUN_0041f420(*(i32 *)(ins + 0x20)) : *(i32 *)(ins + 0x20);
    u8 *context = *(u8 **)((u8 *)this + 0x2ca0);
    return g_EnemyManager.FUN_0042a680(*(i16 *)(ins + 0xc), &position, health, (i8)arg4, field, context + 0x18);
}

// FUNCTION: th08 0x41f420
int __fastcall Enemy::FUN_0041f420(i32 value)
{
    i32 *resolved = this->FUN_0041fe10(&value, 1, 0);
    return resolved != NULL ? *resolved : value;
}

// FUNCTION: th08 0x41fe10
i32 *__fastcall Enemy::FUN_0041fe10(void *operand, i32 flags, i32 operandIndex)
{
    if (operandIndex >= 0 && ((flags >> operandIndex) & 1) == 0)
    {
        return (i32 *)operand;
    }
    i32 id = *(i32 *)operand;
    u8 *context = *(u8 **)((u8 *)this + 0x2ca0);
    if (id >= 10000 && id < 10008)
    {
        return (i32 *)(context + 0x18 + (id - 10000) * 4);
    }
    if (id >= 10008 && id < 10016)
    {
        return (i32 *)((u8 *)this + 0x2ca8 + (id - 10008) * 4);
    }
    if (id >= 10016 && id < 10020)
    {
        return (i32 *)(context + 0x70 + (id - 10016) * 4);
    }
    if (id >= 10020 && id < 10024)
    {
        return (i32 *)(context + 0x58 + (id - 10020) * 4);
    }
    if (id == 10026) return (i32 *)((u8 *)this + 0x2e1c);
    if (id == 10027) return (i32 *)((u8 *)this + 0x2dfc);
    if (id == 10028) return (i32 *)((u8 *)this + 0x3304);
    if (id == 10029) return (i32 *)((u8 *)this + 0x2e08);
    return (i32 *)operand;
}

// FUNCTION: th08 0x420120
f32 Enemy::FUN_00420120(f32 value)
{
    i32 id = (i32)value;
    u8 *context = *(u8 **)((u8 *)this + 0x2ca0);
    if (id >= 10000 && id < 10008)
    {
        return (f32)*(i32 *)(context + 0x18 + (id - 10000) * 4);
    }
    if (id >= 10008 && id < 10016)
    {
        return (f32)*(i32 *)((u8 *)this + 0x2ca8 + (id - 10008) * 4);
    }
    if (id >= 10016 && id < 10024)
    {
        return *(f32 *)(context + 0x38 + (id - 10016) * 4);
    }
    if (id >= 10024 && id < 10032)
    {
        return *(f32 *)((u8 *)this + 0x2cc8 + (id - 10024) * 4);
    }
    return value;
}

// FUNCTION: th08 0x420950
f32 *__fastcall Enemy::FUN_00420950(void *operand, i32 flags, i32 operandIndex)
{
    if (operandIndex >= 0 && ((flags >> operandIndex) & 1) == 0)
    {
        return (f32 *)operand;
    }
    i32 id = (i32)*(f32 *)operand;
    u8 *context = *(u8 **)((u8 *)this + 0x2ca0);
    if (id >= 10016 && id < 10024)
    {
        return (f32 *)(context + 0x38 + (id - 10016) * 4);
    }
    if (id >= 10024 && id < 10096)
    {
        return (f32 *)((u8 *)this + 0x2cc8 + (id - 10024) * 4);
    }
    return (f32 *)operand;
}

// FUNCTION: th08 0x420d10
void __fastcall Enemy::FUN_00420d10(void *instruction)
{
    u16 flags = *(u16 *)((u8 *)instruction + 0xa);
    i32 duration = *this->FUN_0041fe10((u8 *)instruction + 0xc, flags, 0);
    i32 mode = *this->FUN_0041fe10((u8 *)instruction + 0x10, flags, 1);
    f32 angle = AddNormalizeAngle(this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x14)), 0.0f);
    f32 magnitude = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x18));
    *(f32 *)((u8 *)this + 0x2dc4) = cosf(angle) * magnitude * duration;
    *(f32 *)((u8 *)this + 0x2dc8) = sinf(angle) * magnitude * duration;
    *(f32 *)((u8 *)this + 0x2dcc) = 0.0f;
    this->position0x2dd0 = this->position0x2d88;
    *(i32 *)((u8 *)this + 0x2de8) = duration;
    this->timer0x2ddc.SetCurrent(duration);
    u32 *enemyFlags = (u32 *)((u8 *)this + 0x3324);
    *enemyFlags = (*enemyFlags & 0xfffe3fff) | ((mode & 7) << 14);
    *enemyFlags = (*enemyFlags & 0xffffcfff) | 0x2000;
    if ((*enemyFlags & 0x40000) != 0)
    {
        *(f32 *)((u8 *)this + 0x2dc4) = -*(f32 *)((u8 *)this + 0x2dc4);
    }
}

// FUNCTION: th08 0x420f40
void __fastcall Enemy::FUN_00420f40(void *instruction)
{
    u16 flags = *(u16 *)((u8 *)instruction + 0xa);
    i32 duration = *this->FUN_0041fe10((u8 *)instruction + 0xc, flags, 0);
    i32 mode = *this->FUN_0041fe10((u8 *)instruction + 0x10, flags, 1);
    Float3 target(this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x14)),
                  this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x18)), 0.0f);
    *(Float3 *)((u8 *)this + 0x2dc4) = target - this->position0x2d88;
    this->position0x2dd0 = this->position0x2d34;
    *(i32 *)((u8 *)this + 0x2de8) = duration;
    this->timer0x2ddc.SetCurrent(duration);
    u32 *enemyFlags = (u32 *)((u8 *)this + 0x3324);
    *enemyFlags = (*enemyFlags & 0xfffe3fff) | ((mode & 7) << 14);
    *enemyFlags = (*enemyFlags & 0xffffcfff) | 0x2000;
    this->position0x2d4c = Float3(0.0f, 0.0f, 0.0f);
    if ((*enemyFlags & 0x40000) != 0)
    {
        *(f32 *)((u8 *)this + 0x2dc4) = -*(f32 *)((u8 *)this + 0x2dc4);
    }
}

// FUNCTION: th08 0x421120
void __fastcall Enemy::FUN_00421120(void *instruction, f32 interpolation)
{
    f32 start = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x1c));
    f32 end = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x20));
    *this->FUN_00420950((u8 *)instruction + 0x2c, 0, -1) = start + (end - start) * interpolation;
}

// FUNCTION: th08 0x421180
void __fastcall Enemy::FUN_00421180(void *instruction, f32 interpolation)
{
    f32 p0 = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x1c));
    f32 p1 = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x20));
    f32 p2 = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x24));
    f32 p3 = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x28));
    f32 t = interpolation;
    f32 result = (t - 1.0f) * (t - 1.0f) * (2.0f * t + 1.0f) * p0 +
                 t * t * (3.0f - 2.0f * t) * p1 +
                 (1.0f - t) * (1.0f - t) * t * p2 +
                 (t - 1.0f) * t * t * p3;
    *this->FUN_00420950((u8 *)instruction + 0x2c, 0, -1) = result;
}

// FUNCTION: th08 0x421280
void __fastcall StartEnemySpell(Enemy *enemy, void *instruction)
{
    g_Spellcard.StartSpell(*(u16 *)((u8 *)instruction + 0xe), (const char *)instruction + 0x14,
                           *(i16 *)((u8 *)instruction + 0xc), *(i32 *)((u8 *)instruction + 0x10), enemy,
                           (const char *)instruction + 0x44, (const char *)instruction + 0x74,
                           (const char *)instruction + 0xb4);
}

// FUNCTION: th08 0x421300
void __fastcall Enemy::FUN_00421300(void *instruction)
{
    u16 flags = *(u16 *)((u8 *)instruction + 0xa);
    f32 a = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x10));
    f32 b = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x14));
    f32 scale = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x18));
    *this->FUN_00420950((u8 *)instruction + 0xc, flags, 0) = (a - b) * scale + b;
}

// FUNCTION: th08 0x4213f0
void __fastcall Enemy::FUN_004213f0(void *instruction)
{
    u8 *context = *(u8 **)((u8 *)this + 0x2ca0);
    u8 *entry = context + 0x9c;
    f32 key = *(f32 *)((u8 *)instruction + 0xc);
    for (i32 i = 0; i < 8; i++, entry += 0x30)
    {
        if (*(i32 *)entry != 0 && *(f32 *)(entry + 0x2c) != key)
        {
            continue;
        }
        ((ZunTimer *)(entry + 4))->SetCurrent(0);
        *(f32 *)(entry + 0x2c) = key;
        *(i32 *)(entry + 0x10) = (i32)this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x10));
        *(i32 *)(entry + 0x14) = (i32)this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x14));
        *(i32 *)(entry + 0x18) = (i32)this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x18));
        *(f32 *)(entry + 0x1c) = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x1c));
        *(f32 *)(entry + 0x20) = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x20));
        *(f32 *)(entry + 0x24) = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x24));
        *(f32 *)(entry + 0x28) = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x28));
        *(i32 *)entry = 1;
        break;
    }
}

// FUNCTION: th08 0x4215f0
void *__fastcall Enemy::FUN_004215f0(void *instruction)
{
    u8 *ins = (u8 *)instruction;
    u16 mask = *(u16 *)(ins + 0xa);
    i32 opcode = *(i16 *)(ins + 4);
    i32 intA = (mask & 1) ? this->FUN_0041f420(*(i32 *)(ins + 0xc)) : *(i32 *)(ins + 0xc);
    i32 intB = (mask & 2) ? this->FUN_0041f420(*(i32 *)(ins + 0x10)) : *(i32 *)(ins + 0x10);
    f32 floatA = (mask & 1) ? this->FUN_00420120(*(f32 *)(ins + 0xc)) : *(f32 *)(ins + 0xc);
    f32 floatB = (mask & 2) ? this->FUN_00420120(*(f32 *)(ins + 0x10)) : *(f32 *)(ins + 0x10);
    bool condition = false;
    switch (opcode)
    {
    case 40: condition = intA == intB; break;
    case 41: condition = floatA == floatB; break;
    case 42: condition = intA != intB; break;
    case 43: condition = floatA != floatB; break;
    case 44: condition = intA < intB; break;
    case 45: condition = floatA < floatB; break;
    case 46: condition = intA <= intB; break;
    case 47: condition = floatA <= floatB; break;
    case 48: condition = intA > intB; break;
    case 49: condition = floatA > floatB; break;
    case 50: condition = intA >= intB; break;
    case 51: condition = floatA >= floatB; break;
    }
    if (!condition) return NULL;
    u8 *context = *(u8 **)((u8 *)this + 0x2ca0);
    *(i32 *)(context + 0xc) = *(i32 *)(ins + 0x14);
    return ins + *(i32 *)(ins + 0x18);
}

// FUNCTION: th08 0x422020
void __fastcall Enemy::FUN_00422020(void *instruction)
{
    f32 angle = AddNormalizeAngle(g_Rng.GetRandomF32InRange(ZUN_PI / 2.0f), -ZUN_PI / 4.0f);
    this->FUN_004222b0(instruction, angle);
}

// FUNCTION: th08 0x4222b0
void __fastcall Enemy::FUN_004222b0(void *instruction, f32 angle)
{
    u16 flags = *(u16 *)((u8 *)instruction + 0xa);
    i32 duration = *this->FUN_0041fe10((u8 *)instruction + 0xc, flags, 0);
    i32 mode = *this->FUN_0041fe10((u8 *)instruction + 0x10, flags, 1);
    f32 magnitude = this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x14));
    *(f32 *)((u8 *)this + 0x2dc4) = cosf(angle) * magnitude * duration;
    *(f32 *)((u8 *)this + 0x2dc8) = sinf(angle) * magnitude * duration;
    *(f32 *)((u8 *)this + 0x2dcc) = 0.0f;
    this->position0x2dd0 = this->position0x2d88;
    *(i32 *)((u8 *)this + 0x2de8) = duration;
    this->timer0x2ddc.SetCurrent(duration);
    u32 *enemyFlags = (u32 *)((u8 *)this + 0x3324);
    *enemyFlags = (*enemyFlags & 0xfffe3fff) | ((mode & 7) << 14);
    *enemyFlags = (*enemyFlags & 0xffffcfff) | 0x2000;
}

// FUNCTION: th08 0x4224a0
void __fastcall Enemy::FUN_004224a0(void *instruction)
{
    u8 *ins = (u8 *)instruction;
    u16 flags = *(u16 *)(ins + 0xa);
    f32 angle = g_Player.AngleToPlayer(&this->position0x2d34);
    angle += g_Rng.GetRandomF32InRange(ZUN_PI / 2.0f) - ZUN_PI / 4.0f;
    i32 duration = *this->FUN_0041fe10(ins + 0xc, flags, 0);
    if (duration <= 0)
    {
        *(f32 *)((u8 *)this + 0x2d94) = angle;
        *(f32 *)((u8 *)this + 0x2da8) = this->FUN_00420120(*(f32 *)(ins + 0x14));
        u32 *enemyFlags = (u32 *)((u8 *)this + 0x3324);
        *enemyFlags = (*enemyFlags & 0xffffcfff) | 0x1000;
        *(i32 *)((u8 *)this + 0x2de8) = 0;
        this->timer0x2ddc = 0;
    }
    else
    {
        this->FUN_004222b0(instruction, angle);
    }
}

// FUNCTION: th08 0x422720
void __fastcall Enemy::FUN_00422720(void *instruction)
{
    u8 *ins = (u8 *)instruction;
    u8 *args = ins + 0xc;
    u8 *config = (u8 *)this + 0x2e24;
    memset(config, 0, 0x210);
    u16 mask = *(u16 *)(ins + 0xa);
    *(i16 *)config = (i16)((mask & 1) ? this->FUN_0041f420(*(i32 *)args) : *(i32 *)args);
    *(i16 *)(config + 2) = *(i16 *)(args + 2);
    *(i16 *)(config + 0x1f4) = (i16)((mask & 4) ? this->FUN_0041f420(*(i32 *)(args + 4)) : *(i32 *)(args + 4));
    *(i16 *)(config + 0x1f6) = (i16)((mask & 8) ? this->FUN_0041f420(*(i32 *)(args + 8)) : *(i32 *)(args + 8));
    if (*(i16 *)(config + 0x1f4) <= 0) *(i16 *)(config + 0x1f4) = 1;
    if (*(i16 *)(config + 0x1f6) <= 0) *(i16 *)(config + 0x1f6) = 1;
    *(i16 *)(config + 0x1f8) = *(i16 *)(ins + 4) - 0x60;
    *(Float3 *)(config + 4) = this->position0x2d88 - this->position0x2db8;
    *(f32 *)(config + 0x10) = (mask & 0x10) ? this->FUN_00420120(*(f32 *)(args + 0xc)) : *(f32 *)(args + 0xc);
    *(f32 *)(config + 0x14) = (mask & 0x40) ? this->FUN_00420120(*(f32 *)(args + 0x14)) : *(f32 *)(args + 0x14);
    *(f32 *)(config + 0x18) = (mask & 0x20) ? this->FUN_00420120(*(f32 *)(args + 0x10)) : *(f32 *)(args + 0x10);
    *(f32 *)(config + 0x1c) = *(f32 *)(args + 0x18);
    *(u32 *)(config + 0x1fc) = *(u32 *)(args + 0x1c);
    *(i32 *)(config + 0x200) = *(i16 *)(args + 2);
    g_BulletManager.FUN_00430e10(config);
}

// FUNCTION: th08 0x422c40
void Enemy::FUN_00422c40()
{
    u32 *flags = (u32 *)((u8 *)this + 0x3324);
    i32 mode = (*flags >> 12) & 3;
    if (mode == 1)
    {
        *(f32 *)((u8 *)this + 0x2d94) = AddNormalizeAngle(*(f32 *)((u8 *)this + 0x2d94),
            g_Supervisor.framerateMultiplier * *(f32 *)((u8 *)this + 0x2d98));
        *(f32 *)((u8 *)this + 0x2da8) += g_Supervisor.framerateMultiplier * *(f32 *)((u8 *)this + 0x2dac);
        f32 sine, cosine;
        fsincos(&sine, &cosine, *(f32 *)((u8 *)this + 0x2d94));
        this->position0x2d4c = Float3(cosine * *(f32 *)((u8 *)this + 0x2da8),
                                     sine * *(f32 *)((u8 *)this + 0x2da8), 0.0f);
    }
    else if (mode == 2)
    {
        this->timer0x2ddc.Tick();
        i32 duration = *(i32 *)((u8 *)this + 0x2de8);
        f32 t = duration > 0 ? (f32)this->timer0x2ddc.current / duration : 1.0f;
        if (t > 1.0f) t = 1.0f;
        Float3 destination = this->position0x2dd0 + this->position0x2dc4;
        Float3 next = this->position0x2dd0 + this->position0x2dc4 * t;
        this->position0x2d4c = next - this->position0x2d34;
        if (t >= 1.0f)
        {
            this->position0x2d34 = destination;
            this->position0x2d4c = Float3(0.0f, 0.0f, 0.0f);
            *flags &= ~0x3000;
        }
    }
    else if (mode == 3)
    {
        *(f32 *)((u8 *)this + 0x2d9c) = AddNormalizeAngle(*(f32 *)((u8 *)this + 0x2d9c),
            g_Supervisor.framerateMultiplier * *(f32 *)((u8 *)this + 0x2da0));
        *(f32 *)((u8 *)this + 0x2db0) += g_Supervisor.framerateMultiplier * *(f32 *)((u8 *)this + 0x2db4);
        f32 sine, cosine;
        fsincos(&sine, &cosine, *(f32 *)((u8 *)this + 0x2d9c));
        Float3 next = this->position0x2dd0 + Float3(cosine, sine, 0.0f) * *(f32 *)((u8 *)this + 0x2db0);
        this->position0x2d4c = next - this->position0x2d34;
    }
}

// FUNCTION: th08 0x421e50
void __fastcall Enemy::FUN_00421e50(void *instruction)
{
    u16 flags = *(u16 *)((u8 *)instruction + 0xa);
    i32 vmIndex = (flags & 1) ? (i32)this->FUN_00420120(*(f32 *)((u8 *)instruction + 0xc))
                              : *(i32 *)((u8 *)instruction + 0xc);
    i32 script = (flags & 2) ? (i32)this->FUN_00420120(*(f32 *)((u8 *)instruction + 0x10))
                              : *(i32 *)((u8 *)instruction + 0x10);
    if (vmIndex < 0 || vmIndex >= 2)
    {
        return;
    }
    if (script < 0)
    {
        this->vms[vmIndex].scriptIndex = -1;
    }
    else if (this->vms[vmIndex].anmFile != NULL)
    {
        this->vms[vmIndex].anmFile->SetAndExecuteScriptIdx(&this->vms[vmIndex], script);
    }
}

// FUNCTION: th08 0x421bd0
void __fastcall Enemy::FUN_00421bd0(void *instruction, i16 timelineIndex)
{
    EclTimelineContext *context = *(EclTimelineContext **)((u8 *)this + 0x2ca0);
    context->timeline = (u8 *)instruction + *(i16 *)((u8 *)instruction + 6);
    if ((*(u32 *)((u8 *)this + 0x3324) & 0x4000000) == 0)
    {
        i16 index = *(i16 *)((u8 *)this + 0x2cea);
        memcpy((u8 *)*(void **)((u8 *)this + 0x2ca4) + index * sizeof(EclTimelineContext),
               context, sizeof(EclTimelineContext));
    }
    g_EclManager.FUN_00418450(context, timelineIndex);
    memcpy((u8 *)context + 0x70, (u8 *)&g_EclManager + 0x168, 0x20);
    if ((*(u32 *)((u8 *)this + 0x3324) & 0x4000000) == 0 &&
        *(i16 *)((u8 *)this + 0x2cea) < 15)
    {
        (*(i16 *)((u8 *)this + 0x2cea))++;
    }
}

// FUNCTION: th08 0x421cb0
i32 __fastcall Enemy::FUN_00421cb0(void *)
{
    i16 *index = (i16 *)((u8 *)this + 0x2cea);
    (*index)--;
    if (*index < 0)
    {
        EclTimelineContext *context = *(EclTimelineContext **)((u8 *)this + 0x2ca0);
        i32 slot = context->timelineIndex - 1;
        if (slot >= 0)
        {
            *(void **)((u8 *)this + 0x3384 + slot * 4) = NULL;
        }
        *(void **)((u8 *)this + 0x2ca4) = (u8 *)this + 0xa20;
        *(void **)((u8 *)this + 0x2ca0) = (u8 *)this + 0x7f8;
        *index = *(i16 *)((u8 *)this + 0x2ce8);
        return 1;
    }
    memcpy(*(void **)((u8 *)this + 0x2ca0),
           (u8 *)*(void **)((u8 *)this + 0x2ca4) + *index * sizeof(EclTimelineContext),
           sizeof(EclTimelineContext));
    return 0;
}

// FUNCTION: th08 0x421de0
void Enemy::FUN_00421de0(i16 a, i16 b, i16 c, i16 d, i16 e, i16 f)
{
    *(i16 *)((u8 *)this + 0x3332) = a;
    *(i16 *)((u8 *)this + 0x3338) = b;
    *(i16 *)((u8 *)this + 0x333a) = c;
    *(i16 *)((u8 *)this + 0x3334) = d;
    *(i16 *)((u8 *)this + 0x3336) = e;
    *(i16 *)((u8 *)this + 0x333c) = f;
    *(u8 *)((u8 *)this + 0x332e) = 0xff;
}

// FUNCTION: th08 0x41fd20
ZunBool Enemy::FUN_0041fd20()
{
    return *(i32 *)((u8 *)this + 0x2da4) != 0;
}

// FUNCTION: th08 0x41fd90
#pragma var_order(result)
ZunBool Enemy::FUN_0041fd90()
{
    ZunBool result;

    if (((*(u32 *)this & 1) != 0) && ((*(u32 *)this >> 2) & 1) != 0)
    {
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
}

// FUNCTION: th08 0x42b370
#pragma var_order(maxHealth, i, damageAmount)
void Enemy::FUN_0042b370(i32 damage)
{
    i32 damageAmount;
    i32 i;
    i32 maxHealth;
    u8 *data;

    if (!this->FUN_0041fd20() || *(i32 *)((u8 *)&g_Player + 0xfdc) != 0)
    {
        return;
    }
    data = *(u8 **)((u8 *)this + 0x2da4);
    maxHealth = 0;
    for (i = 0; i < 4; i++)
    {
        if (maxHealth < *(i32 *)(data + 0x3358 + i * 4))
        {
            maxHealth = *(i32 *)(data + 0x3358 + i * 4);
        }
    }
    damageAmount = damage / 2;
    if (*(ZunTimer *)(data + 0x5354) > 0)
    {
        if ((*(u32 *)(data + 0x3324) & 2) != 0)
        {
            damageAmount /= 9;
        }
        else
        {
            damageAmount = 0;
        }
    }
    if (damageAmount == 0)
    {
        return;
    }
    *(i32 *)(data + 0x2dfc) -= damageAmount;
    if (*(i32 *)(data + 0x2dfc) <= maxHealth)
    {
        *(i32 *)(data + 0x2dfc) = maxHealth;
    }
}

// FUNCTION: th08 0x42bc50
void Enemy::FUN_0042bc50()
{
    *(u32 *)this &= ~4;
    *(u32 *)this |= 8;
    *(i32 *)((u8 *)this + 0xfc) = 0;
}

// FUNCTION: th08 0x42b2f0
void Enemy::FUN_0042b2f0()
{
    if (this->FUN_0041fd20())
    {
        *(Enemy **)(*(u8 **)((u8 *)this + 4) + 8) = *(Enemy **)((u8 *)this + 8);
        if (*(Enemy **)((u8 *)this + 8) != NULL)
        {
            *(Enemy **)(*(u8 **)((u8 *)this + 8) + 4) = *(Enemy **)((u8 *)this + 4);
        }
        *(i32 *)((u8 *)this + 0x2da4) = 0;
        *(Enemy **)((u8 *)this + 8) = NULL;
        *(Enemy **)((u8 *)this + 4) = NULL;
    }
    else
    {
        *(Enemy **)((u8 *)this + 4) = NULL;
        *(Enemy **)((u8 *)this + 8) = NULL;
    }
}

// FUNCTION: th08 0x42bc90
void Enemy::FUN_0042bc90()
{
    i32 i;

    for (i = 0; i < 4; i++)
    {
        if (*(void **)((u8 *)this + i * 4 + 0x3384) != NULL)
        {
            g_ZunMemory.Free(*(void **)((u8 *)this + i * 4 + 0x3384));
            *(void **)((u8 *)this + i * 4 + 0x3384) = NULL;
        }
    }
}

// FUNCTION: th08 0x42adb0
void Enemy::FUN_0042adb0(i32 spawnDrops)
{
    i32 familiarCount = this->GetFamiliarCount();
    Enemy *familiar = *(Enemy **)((u8 *)this + 8);
    for (i32 i = 0; familiar != NULL && i < familiarCount; i++)
    {
        Enemy *next = *(Enemy **)((u8 *)familiar + 8);
        *(u32 *)((u8 *)familiar + 0x3324) |= 0x400;
        *(Enemy **)((u8 *)familiar + 4) = NULL;
        *(Enemy **)((u8 *)familiar + 8) = NULL;
        familiar->position0x2d88 = familiar->position0x2d34 - familiar->position0x2d40;
        if (spawnDrops)
        {
            i32 count = familiarCount < 10 ? familiarCount * 2 + 6 : 26;
            g_EffectManager.SpawnEffect(7, &familiar->position0x2d88, count, -1);
            for (i32 j = 0; j < count; j++)
            {
                Float3 position = familiar->position0x2d88;
                position.x += g_Rng.GetRandomF32InRange(64.0f) - 32.0f;
                position.y += g_Rng.GetRandomF32InRange(64.0f) - 32.0f;
                g_ItemManager.SpawnItem(&position, ITEM_POINT_STAR, ITEM_STATE_AUTOCOLLECT);
            }
        }
        *(i32 *)((u8 *)familiar + 0x330c) = 0;
        *(i32 *)((u8 *)familiar + 0x3308) = 0;
        *(i32 *)((u8 *)familiar + 0x3304) = -2;
        familiar = next;
    }
    *(Enemy **)((u8 *)this + 8) = NULL;
    if (spawnDrops && familiarCount == 0)
    {
        g_EffectManager.SpawnEffect(7, &this->position0x2d88,
                                    *(i32 *)((u8 *)this + 0x3380) * 2, 0xfff0f00f);
    }
    this->FUN_0042b2f0();
}

// FUNCTION: th08 0x42b490
int Enemy::FUN_0042b490()
{
    *(u32 *)((u8 *)this + 0x3328) &= ~0x30;
    i32 activePhases = 0;
    for (i32 i = 0; i < 4; i++)
    {
        i32 threshold = *(i32 *)((u8 *)this + 0x3358 + i * 4);
        if (threshold < 0) continue;
        activePhases++;
        if (*(i32 *)((u8 *)this + 0x2dfc) < threshold)
        {
            *(i32 *)((u8 *)this + 0x2dfc) = threshold;
            *(i32 *)((u8 *)this + 0x2e04) = threshold;
            i16 timeline = *(i16 *)((u8 *)this + 0x3368 + i * 4);
            g_EclManager.FUN_00418450((EclTimelineContext *)((u8 *)this + 0x7f8), timeline);
            *(i32 *)((u8 *)this + 0x3358 + i * 4) = -1;
            *(i32 *)((u8 *)this + 0x53cc) = (*(i32 *)((u8 *)this + 0x3378) - this->timer0x2e14.current) / 60;
            *(i32 *)((u8 *)this + 0x3378) = -1;
            this->FUN_00415c80();
            *(i16 *)((u8 *)this + 0x2cea) = 0;
            memset((u8 *)this + 0x2e24, 0, 0x210);
            *(i32 *)((u8 *)this + 0x3060) = 0;
            this->FUN_0042adb0(1);
            for (i32 j = 0; j < 480; j++)
            {
                Enemy *enemy = &g_EnemyManager.enemies[j];
                if ((*(u32 *)((u8 *)enemy + 0x3324) & 1) != 0 &&
                    (*(u32 *)((u8 *)enemy + 0x3324) & 2) == 0)
                    *(i32 *)((u8 *)enemy + 0x2dfc) = 0;
            }
            return 1;
        }
        i32 difference = *(i32 *)((u8 *)this + 0x2dfc) - threshold;
        i32 level = difference < 120 ? 3 : difference < 300 ? 2 : difference < 600 ? 1 : 0;
        u32 *flags = (u32 *)((u8 *)this + 0x3328);
        if (((*flags >> 4) & 3) < (u32)level)
            *flags = (*flags & ~0x30) | (level << 4);
    }
    if (activePhases == 0)
    {
        i32 health = *(i32 *)((u8 *)this + 0x2dfc);
        i32 level = health < 50 ? 3 : health < 400 ? 2 : health < 1200 ? 1 : 0;
        u32 *flags = (u32 *)((u8 *)this + 0x3328);
        if (((*flags >> 4) & 3) < (u32)level)
            *flags = (*flags & ~0x30) | (level << 4);
    }
    return 0;
}

// FUNCTION: th08 0x42b930
int Enemy::FUN_0042b930()
{
    i32 transitionFrame = *(i32 *)((u8 *)this + 0x3378);
    if (transitionFrame < 0 || this->timer0x2e14.current < transitionFrame)
        return 0;
    *(i32 *)((u8 *)this + 0x53cc) = 0;
    i32 bestHealth = 0;
    i32 bestIndex = -1;
    for (i32 i = 0; i < 4; i++)
    {
        i32 value = *(i32 *)((u8 *)this + 0x3358 + i * 4);
        if (value >= bestHealth)
        {
            bestHealth = value;
            bestIndex = i;
        }
    }
    if (bestHealth > 0)
    {
        *(i32 *)((u8 *)this + 0x2dfc) = bestHealth;
        *(i32 *)((u8 *)this + 0x2e04) = bestHealth;
        *(i32 *)((u8 *)this + 0x3358 + bestIndex * 4) = -1;
    }
    i16 timeline = *(i16 *)((u8 *)this + 0x337c);
    g_EclManager.FUN_00418450((EclTimelineContext *)((u8 *)this + 0x7f8), timeline);
    *(i32 *)((u8 *)this + 0x3378) = -1;
    *(i32 *)((u8 *)this + 0x337c) = *(i16 *)((u8 *)this + 0x2cee);
    this->timer0x2e14 = 0;
    if ((*(u32 *)((u8 *)this + 0x3324) & 0x8000000) == 0)
        g_BulletManager.RemoveAllBullets(4);
    for (i32 i = 0; i < 480; i++)
    {
        Enemy *enemy = &g_EnemyManager.enemies[i];
        if ((*(u32 *)((u8 *)enemy + 0x3324) & 1) == 0 ||
            (*(u32 *)((u8 *)enemy + 0x3324) & 2) != 0)
            continue;
        *(i32 *)((u8 *)enemy + 0x2dfc) = 0;
        i16 enemyTimeline = *(i16 *)((u8 *)enemy + 0x2cee);
        if (enemyTimeline >= 0)
        {
            g_EclManager.FUN_00418450((EclTimelineContext *)((u8 *)enemy + 0x7f8), enemyTimeline);
            *(i16 *)((u8 *)enemy + 0x2cee) = -1;
        }
    }
    this->FUN_0042bc90();
    *(i16 *)((u8 *)this + 0x2cea) = 0;
    *(u32 *)((u8 *)this + 0x3328) &= ~0x30;
    return 1;
}

// FUNCTION: th08 0x42bea0
void Enemy::FUN_0042bea0(i32 itemState)
{
    i32 itemType = *(i32 *)((u8 *)this + 0x3304);
    if (itemType >= 0)
    {
        g_EffectManager.SpawnEffect(*(u8 *)((u8 *)this + 0x3311) + 4, &this->position0x2d88, 3, -1);
        g_ItemManager.SpawnItem(&this->position0x2d88, (ItemType)itemType,
                                itemState != 0 ? ITEM_STATE_AUTOCOLLECT : ITEM_STATE_DEFAULT);
    }
    else if (itemType == -1)
    {
        static u16 dropTimer;
        static u16 dropIndex;
        static const u8 dropSequence[32] = {
            0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0,
            1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1,
        };
        if ((dropTimer % 3) == 0)
        {
            g_EffectManager.SpawnEffect(*(u8 *)((u8 *)this + 0x3311) + 4, &this->position0x2d88, 6, -1);
            g_ItemManager.SpawnItem(&this->position0x2d88, (ItemType)dropSequence[dropIndex],
                                    itemState != 0 ? ITEM_STATE_AUTOCOLLECT : ITEM_STATE_DEFAULT);
            if (++dropIndex >= 32) dropIndex = 0;
        }
        dropTimer++;
    }
    i32 powerDrops = *(i32 *)((u8 *)this + 0x330c);
    for (i32 i = 0; i < powerDrops; i++)
    {
        Float3 position = this->position0x2d88;
        position.x += g_Rng.GetRandomF32() * 128.0f - 64.0f;
        position.y += g_Rng.GetRandomF32() * 128.0f - 64.0f;
        ItemType type = g_GameManager.GetPower() < 128 ? ITEM_POWER_SMALL : ITEM_POINT;
        g_ItemManager.SpawnItem(&position, type, ITEM_STATE_DEFAULT);
    }
    *(i32 *)((u8 *)this + 0x330c) = 0;
    i32 pointDrops = *(i32 *)((u8 *)this + 0x3308);
    for (i32 i = 0; i < pointDrops; i++)
    {
        Float3 position = this->position0x2d88;
        position.x += g_Rng.GetRandomF32() * 128.0f - 64.0f;
        position.y += g_Rng.GetRandomF32() * 128.0f - 64.0f;
        g_ItemManager.SpawnItem(&position, ITEM_POINT, ITEM_STATE_DEFAULT);
    }
    *(i32 *)((u8 *)this + 0x3308) = 0;
}

// FUNCTION: th08 0x42bcf0
void Enemy::FUN_0042bcf0()
{
    u32 *flags = (u32 *)((u8 *)this + 0x3324);
    if (((*flags >> 20) & 7) == 0)
    {
        *flags &= ~1;
    }
    if (*(i32 *)((u8 *)this + 0x53c0) != 0)
    {
        this->FUN_0042a820();
    }
    if (*(AnmVm **)((u8 *)this + 0x53c8) != NULL)
    {
        (*(AnmVm **)((u8 *)this + 0x53c8))->SetInterrupt(3);
        *(AnmVm **)((u8 *)this + 0x53c8) = NULL;
    }
    for (i32 i = 0; i < 4; i++)
    {
        *(i32 *)((u8 *)this + 0x3358 + i * 4) = -1;
    }
    *(i32 *)((u8 *)this + 0x3378) = -1;
    this->FUN_0042bc90();
}

// FUNCTION: th08 0x42c420
void Enemy::FUN_0042c420()
{
    ZunBool isYoukai = g_Player.IsYoukai();
    u32 *flags = (u32 *)((u8 *)this + 0x3324);
    if ((*flags & 0x800) == 0)
    {
        if (isYoukai)
        {
            g_EffectManager.SpawnEffect(31, &this->position0x2d88, 1, 0x80303080);
            if (*(AnmVm **)((u8 *)this + 0x53c8) != NULL)
                (*(AnmVm **)((u8 *)this + 0x53c8))->SetInterrupt(2);
            g_SoundPlayer.PlaySoundByIdx((SoundIdx)40, 0);
            *(u8 *)((u8 *)this + 0x332f) = 0;
        }
        if ((*(u32 *)((u8 *)this + 0x3328) & 2) != 0 && this->timer0x2e14.FUN_0040ebc0(2))
        {
            g_EffectManager.SpawnEffect(38, &this->position0x2d88, 1, -1);
        }
    }
    else if (!isYoukai)
    {
        g_EffectManager.SpawnEffect(30, &this->position0x2d88, 1, 0x80803030);
        if (*(AnmVm **)((u8 *)this + 0x53c8) != NULL)
            (*(AnmVm **)((u8 *)this + 0x53c8))->SetInterrupt(1);
        g_SoundPlayer.PlaySoundByIdx((SoundIdx)39, 0);
        *(u8 *)((u8 *)this + 0x332f) = 2;
    }
    if (isYoukai) *flags |= 0x800;
    else *flags &= ~0x800;
    *(u8 *)((u8 *)this + 0x3330) = isYoukai ? 64 : 32;
}

// FUNCTION: th08 0x42c180
void Enemy::FUN_0042c180()
{
    if ((*(u32 *)((u8 *)this + 0x3324) & 0x80000) != 0)
    {
        if (this->position0x2d34.x <= *(f32 *)((u8 *)this + 0x3340))
        {
            this->position0x2d34.x = *(f32 *)((u8 *)this + 0x3340);
        }
        else if (this->position0x2d34.x >= *(f32 *)((u8 *)this + 0x3348))
        {
            this->position0x2d34.x = *(f32 *)((u8 *)this + 0x3348);
        }
        if (this->position0x2d34.y <= *(f32 *)((u8 *)this + 0x3344))
        {
            this->position0x2d34.y = *(f32 *)((u8 *)this + 0x3344);
        }
        else if (this->position0x2d34.y >= *(f32 *)((u8 *)this + 0x334c))
        {
            this->position0x2d34.y = *(f32 *)((u8 *)this + 0x334c);
        }
    }
}

// FUNCTION: th08 0x42c290
#pragma var_order(size)
void Enemy::FUN_0042c290(Float3 *position, Float3 *hitbox)
{
    Float3 size;
    u32 flags = *(u32 *)((u8 *)this + 0x3324);

    size = *hitbox / 0.7f;
    if ((flags & 0x80) != 0 && this->timer0x2e14.current != this->timer0x2e14.previous &&
        this->timer0x2e14.current % 6 == 0)
    {
        g_Player.FUN_0044a470(position, &size);
    }
    u8 gameMode = *(u8 *)((u8 *)&g_GameManager + 0x3dba9);
    if ((gameMode == 0 || gameMode == 4) && this->FUN_0041fd20())
    {
        return;
    }
    size = *hitbox / 1.5f;
    if (g_Player.FUN_0044a360(position, &size) == 1 && (flags & 2) == 0 && (flags & 0x80) == 0)
    {
        *(i32 *)((u8 *)this + 0x2dfc) -= 10;
    }
}

// FUNCTION: th08 0x42e010
#pragma var_order(i, effect)
void Enemy::FUN_0042e010()
{
    for (i32 i = 0; i < *(i32 *)((u8 *)this + 0x53c0); i++)
    {
        u8 *effect = *(u8 **)((u8 *)this + 0x5360 + i * 4);
        if (effect == NULL)
        {
            continue;
        }
        if ((*(u32 *)((u8 *)this + 0x3324) & 0x10) == 0)
        {
            *(u32 *)(effect + 0x1f8) |= 2;
        }
        else
        {
            *(u32 *)(effect + 0x1f8) &= ~2;
        }
        *(Float3 *)(effect + 0x2e0) = this->position0x2d34;
        f32 targetScale = *(f32 *)((u8 *)this + 0x53c4);
        if (*(f32 *)(effect + 0x314) >= targetScale)
        {
            *(f32 *)(effect + 0x314) = targetScale;
        }
        else
        {
            *(f32 *)(effect + 0x314) += 0.3f;
        }
        *(f32 *)(effect + 0x318) = AddNormalizeAngle(*(f32 *)(effect + 0x318), 0.03141593f);
    }
}

// FUNCTION: th08 0x42a820
void Enemy::FUN_0042a820()
{
    i32 i;

    for (i = 0; i < *(i32 *)((u8 *)this + 0x53c0); i++)
    {
        if (*(void **)((u8 *)this + i * 4 + 0x5360) == NULL)
        {
            continue;
        }
        *(u8 *)(*(u8 **)((u8 *)this + i * 4 + 0x5360) + 0x352) = 1;
        *(void **)((u8 *)this + i * 4 + 0x5360) = NULL;
    }
    *(i32 *)((u8 *)this + 0x53c0) = 0;
}

// FUNCTION: th08 0x41fd40
i32 Enemy::GetFamiliarCount()
{
    Enemy *enemy;
    i32 count;

    enemy = this;
    count = 0;
    if (this->FUN_0041f000())
    {
        while (*(Enemy **)((u8 *)enemy + 8) != NULL)
        {
            enemy = *(Enemy **)((u8 *)enemy + 8);
            count++;
        }
    }
    return count;
}

// FUNCTION: th08 0x42a8a0
void EclTimeline::FUN_0042a8a0()
{
    u8 *instruction = (u8 *)this->unknown;
    for (i32 guard = 0; instruction != NULL && guard < 256; guard++)
    {
        i32 time = *(i32 *)instruction;
        if (time < 0 || this->timer.current < time) break;
        i16 opcode = *(i16 *)(instruction + 4);
        u8 size = *(u8 *)(instruction + 6);
        if (size < 8) break;
        if ((*(u8 *)(instruction + 7) & g_GameManager.difficultyMask) != 0)
        {
            u8 *args = instruction + 8;
            if (opcode >= 1 && opcode <= 5)
            {
                Float3 position(*(f32 *)(args + 4), *(f32 *)(args + 8), 0.0f);
                Enemy *enemy = g_EnemyManager.FUN_0042a4e0(*(i16 *)args, &position,
                    *(i32 *)(args + 0xc), (i8)*(i32 *)(args + 0x10), *(i32 *)(args + 0x14), opcode == 1);
                if (enemy != NULL && opcode == 3)
                {
                    *(i32 *)((u8 *)enemy + 0x3308) = *(i32 *)(args + 0x10);
                    *(i32 *)((u8 *)enemy + 0x330c) = *(i32 *)(args + 0x14);
                }
            }
            else if (opcode == 7)
            {
                g_BulletManager.RemoveAllBullets(*(i32 *)args);
            }
        }
        instruction += size;
        this->unknown = (u32)instruction;
    }
    this->timer.Tick();
}

// FUNCTION: th08 0x42a4e0
Enemy *EnemyManager::FUN_0042a4e0(i16 timeline, Float3 *position, i32 health, i8 arg4, i32 field2e08, i32 flag)
{
    Enemy *enemy = NULL;
    i32 index;
    for (index = 0; index < 480; index++)
    {
        if ((*(u32 *)((u8 *)&this->enemies[index] + 0x3324) & 1) == 0)
        {
            enemy = &this->enemies[index];
            break;
        }
    }
    *(i32 *)((u8 *)this + 0x9dcef8) = enemy == NULL;
    if (enemy == NULL)
    {
        return NULL;
    }
    memcpy(enemy, &this->enemyTemplate, sizeof(Enemy));
    *(i32 *)((u8 *)enemy + 0x2e0c) = index;
    if (flag & 1) *(u32 *)((u8 *)enemy + 0x3324) |= 0x40000;
    else *(u32 *)((u8 *)enemy + 0x3324) &= ~0x40000;
    if (health >= 0) *(i32 *)((u8 *)enemy + 0x2dfc) = health;
    enemy->position0x2d34 = *position;
    g_EclManager.FUN_00418450((EclTimelineContext *)&enemy->unk0x7f8, timeline);
    *(i32 *)((u8 *)enemy + 0x2e20) = enemy->vm.type;
    *(i32 *)((u8 *)enemy + 0x3304) = arg4;
    if (field2e08 >= 0) *(i32 *)((u8 *)enemy + 0x2e08) = field2e08;
    *(i32 *)((u8 *)enemy + 0x2e00) = *(i32 *)((u8 *)enemy + 0x2dfc);
    *(i32 *)((u8 *)enemy + 0x2e04) = *(i32 *)((u8 *)enemy + 0x2e00);
    return enemy;
}

// FUNCTION: th08 0x42a680
Enemy *EnemyManager::FUN_0042a680(i16 timeline, Float3 *position, i32 health, i8 arg4, i32 field2e08,
                                   void *eclData)
{
    Enemy *enemy = this->FUN_0042a4e0(timeline, position, health, arg4, field2e08, 0);
    if (enemy != NULL && eclData != NULL)
    {
        memcpy((u8 *)enemy + 0x810, eclData, 0x78);
    }
    return enemy;
}

// FUNCTION: th08 0x429e00
#pragma var_order(i, enemy)
void EnemyManager::Initialize()
{
    Enemy *enemy;
    i32 i;

    enemy = &this->enemies[0];
    memset(this, 0, sizeof(EnemyManager));
    for (i = 0; (u32)i < 4; i++)
    {
        *(i32 *)((u8 *)this + 0x9dcefc + i * 4) = -1;
    }

    enemy = &this->enemyTemplate;
    memset(enemy, 0, sizeof(Enemy));
    for (i = 0; i < 2; i++)
    {
        *(i16 *)((u8 *)enemy + 0x4ca + i * sizeof(AnmVm)) = -1;
    }
    for (i = 0; i < 0x60; i++)
    {
        *(f32 *)((u8 *)enemy + 0x3394 + i * sizeof(EnemyUnkStruct0x1c)) = -999.0f;
    }

    *(u32 *)((u8 *)enemy + 0x3324) |= 1;
    enemy->timer0x2e14 = 0;
    *(u32 *)((u8 *)enemy + 0x3324) &= 0xfeffffff;
    enemy->position0x2d70 = Float3(24.0f, 24.0f, 24.0f);
    enemy->position0x2d4c = Float3(0.0f, 0.0f, 0.0f);
    *(i32 *)((u8 *)enemy + 0x2d98) = 0;
    *(i32 *)((u8 *)enemy + 0x2d94) = 0;
    *(i32 *)((u8 *)enemy + 0x2dac) = 0;
    *(i32 *)((u8 *)enemy + 0x2da8) = 0;
    *(u32 *)((u8 *)enemy + 0x3324) &= 0xffffcfff;
    *(u32 *)((u8 *)enemy + 0x3324) &= 0xfffdffff;
    *(u32 *)((u8 *)enemy + 0x3324) &= 0xfffbffff;
    *(u32 *)((u8 *)enemy + 0x3324) &= 0xfffffffd;
    *(i16 *)((u8 *)enemy + 0x2cea) = 0;
    *(i32 *)((u8 *)enemy + 0x2dfc) = 1;
    *(i32 *)((u8 *)enemy + 0x2e08) = 100;
    *(u8 *)((u8 *)enemy + 0x3310) = 0;
    *(u8 *)((u8 *)enemy + 0x3311) = 0;
    *(u8 *)((u8 *)enemy + 0x3312) = 0;
    *(i32 *)((u8 *)enemy + 0x3060) = 0;
    enemy->timer0x3064 = 0;
    enemy->position0x2db8 = Float3(0.0f, 0.0f, 0.0f);
    *(i16 *)((u8 *)enemy + 0x3338) = -1;
    *(i16 *)((u8 *)enemy + 0x333a) = -1;
    *(i16 *)((u8 *)enemy + 0x3332) = -1;
    *(u32 *)((u8 *)enemy + 0x3324) |= 4;
    *(u32 *)((u8 *)enemy + 0x3324) |= 8;
    *(u32 *)((u8 *)enemy + 0x3324) &= 0xffffffef;
    *(u32 *)((u8 *)enemy + 0x3324) |= 0x40;
    *(u32 *)((u8 *)enemy + 0x3324) &= 0xffffff7f;
    *(u32 *)((u8 *)enemy + 0x3324) &= 0xff8fffff;
    *(i16 *)((u8 *)enemy + 0x2cee) = -1;
    *(u32 *)((u8 *)enemy + 0x3324) &= 0xfff7ffff;
    *(i32 *)((u8 *)enemy + 0x53c0) = 0;
    *(i16 *)((u8 *)enemy + 0x2d30) = -1;
    for (i = 0; i < 4; i++)
    {
        *(i32 *)((u8 *)enemy + 0x3358 + i * 4) = -1;
    }
    *(i32 *)((u8 *)enemy + 0x3378) = -1;
    *(i32 *)((u8 *)enemy + 0x3300) = 0;
    *(u8 *)((u8 *)enemy + 0x3314) = 0;
    *(u32 *)((u8 *)enemy + 0x3324) &= 0xfdffffff;
    *(f32 *)((u8 *)enemy + 0x2dec) = -0.15f;
    *(f32 *)((u8 *)enemy + 0x2df0) = 0.15f;
    *(i32 *)((u8 *)enemy + 0x3024) = 7;
    *(i32 *)((u8 *)enemy + 0x3028) = 25;
    *(f32 *)((u8 *)enemy + 0x3350) = 1024.0f;
    *(i32 *)((u8 *)enemy + 0x2e10) = g_EnemyManagerUnknown;
}

// FUNCTION: th08 0x42c590
#pragma var_order(result, enemyManager)
ZunResult EnemyManager::RegisterChain()
{
    EnemyManager *enemyManager;
    ZunResult result;

    enemyManager = &g_EnemyManager;
    result = ZUN_SUCCESS;
    enemyManager->Initialize();

    g_EnemyManagerCalcChain.SetCallback((ChainCallback)EnemyManager::OnUpdate);
    g_EnemyManagerCalcChain.addedCallback = (ChainLifetimeCallback)EnemyManager::AddedCallback;
    g_EnemyManagerCalcChain.deletedCallback = (ChainLifetimeCallback)EnemyManager::DeletedCallback;
    g_EnemyManagerCalcChain.arg = enemyManager;
    if (g_Chain.AddToCalcChain(&g_EnemyManagerCalcChain, 11) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    g_EnemyManagerDrawChainHighPrio.SetCallback((ChainCallback)EnemyManager::OnDrawHighPrio);
    g_EnemyManagerDrawChainHighPrio.arg = enemyManager;
    if (g_Chain.AddToDrawChain(&g_EnemyManagerDrawChainHighPrio, 8) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    g_EnemyManagerDrawChainLowPrio.SetCallback((ChainCallback)EnemyManager::OnDrawLowPrio);
    g_EnemyManagerDrawChainLowPrio.arg = enemyManager;
    if (g_Chain.AddToDrawChain(&g_EnemyManagerDrawChainLowPrio, 11) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x42c660
ChainCallbackResult EnemyManager::OnUpdate()
{
    EnemyManager *enemyManager = &g_EnemyManager;
    if (g_GameManager.flags.unk10) return CHAIN_CALLBACK_RESULT_CONTINUE;
    enemyManager->FUN_0042c3b0();
    Enemy **layerHeads = (Enemy **)enemyManager->unknown0x9dcedc;
    for (i32 i = 0; i < 4; i++) layerHeads[i] = NULL;
    if (g_EclManager.timelineFile != NULL)
    {
        i32 count = g_EclManager.GetTimelineCount();
        if (count > ARRAY_SIZE_SIGNED(enemyManager->timelines)) count = ARRAY_SIZE_SIGNED(enemyManager->timelines);
        for (i32 i = 0; i < count; i++) enemyManager->timelines[i].FUN_0042a8a0();
    }
    for (i32 i = 0; i < 480; i++)
    {
        Enemy *enemy = &enemyManager->enemies[i];
        u32 *flags = (u32 *)((u8 *)enemy + 0x3324);
        if ((*flags & 1) == 0) continue;
        g_EclManager.RunEcl(enemy);
        enemy->position0x2d34 += enemy->position0x2d4c * g_Supervisor.framerateMultiplier;
        enemy->FUN_0042c180();
        enemy->position0x2d88 = enemy->position0x2d34;
        if (enemy->vm.beginningOfScript != NULL) g_AnmManager->ExecuteScript(&enemy->vm);
        for (i32 j = 0; j < 2; j++)
            if (enemy->vms[j].beginningOfScript != NULL) g_AnmManager->ExecuteScript(&enemy->vms[j]);
        enemy->timer0x2ddc.Tick();
        enemy->timer0x2e14.Tick();
        enemy->timer0x3064.Tick();
        enemy->timer0x3318.Tick();
        enemy->timer0x5354.Tick();
        if ((*flags & 0x10) == 0)
        {
            u8 layer = *(u8 *)((u8 *)enemy + 0x332f);
            if (layer > 3) layer = 3;
            *(Enemy **)enemy = layerHeads[layer];
            layerHeads[layer] = enemy;
        }
    }
    enemyManager->timer.Tick();
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x42e120
ChainCallbackResult EnemyManager::OnDrawHighPrio(EnemyManager *enemyManager)
{
    return enemyManager->OnDrawImpl(0, 2);
}

// FUNCTION: th08 0x42e140
ChainCallbackResult EnemyManager::OnDrawImpl(i32 firstLayer, i32 lastLayer)
{
    Enemy **layerHeads = (Enemy **)this->unknown0x9dcedc;
    for (i32 layer = firstLayer; layer < lastLayer; layer++)
    {
        for (Enemy *enemy = layerHeads[layer]; enemy != NULL; enemy = *(Enemy **)enemy)
        {
            if (enemy->vm.scriptIndex >= 0 && enemy->vm.IsVisible())
            {
                enemy->vm.pos = enemy->position0x2d88;
                enemy->vm.pos.z = 0.4f;
                g_AnmManager->Draw2D(&enemy->vm);
            }
            for (i32 i = 0; i < 2; i++)
            {
                if (enemy->vms[i].scriptIndex < 0 || !enemy->vms[i].IsVisible()) continue;
                enemy->vms[i].pos = enemy->position0x2d88;
                enemy->vms[i].pos.z = 0.3f;
                g_AnmManager->Draw2D(&enemy->vms[i]);
            }
        }
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x42eb90
ChainCallbackResult EnemyManager::OnDrawLowPrio(EnemyManager *enemyManager)
{
    ChainCallbackResult result;

    if (g_GameManager.flags.unk10)
    {
        g_AnmManager->SetMixColor(0xfff01010);
    }
    result = enemyManager->OnDrawImpl(2, 4);
    if (g_GameManager.flags.unk10)
    {
        g_AnmManager->SetMixColorDefault();
    }
    return result;
}

// FUNCTION: th08 0x42ebf0
#pragma var_order(enemy, position)
ZunResult EnemyManager::AddedCallback(EnemyManager *enemyManager)
{
    Enemy *enemy;
    Float3 position;
    EclTimelineHeader *timelineFile;
    void *subTable;

    enemy = &enemyManager->enemies[0];
    if (FUN_004338b0())
    {
        *(AnmLoaded **)((u8 *)enemyManager + 0x9dceec) = g_AnmManager->PreloadAnm(7, "enemy.anm");
        if (*(AnmLoaded **)((u8 *)enemyManager + 0x9dceec) == NULL)
        {
            return ZUN_ERROR;
        }
    }
    else
    {
        *(AnmLoaded **)((u8 *)enemyManager + 0x9dceec) = g_AnmManager->GetAnm(7);
    }

    if (!IsDisableResourceReload())
    {
        *(AnmLoaded **)((u8 *)enemyManager + 0x9dcef0) =
            g_AnmManager->PreloadAnm(8, g_StageEnemyAnms[g_GameManager.currentStage]);
        if (*(AnmLoaded **)((u8 *)enemyManager + 0x9dcef0) == NULL)
        {
            return ZUN_ERROR;
        }
    }
    else
    {
        *(AnmLoaded **)((u8 *)enemyManager + 0x9dcef0) = g_AnmManager->GetAnm(8);
    }

    if (!IsDisableResourceReload())
    {
        memset(&g_EclManager, 0, sizeof(g_EclManager));
        if (!g_GameManager.IsSpellPractice())
        {
            if (g_EclManager.Load(g_StageEclFiles[g_GameManager.currentStage]) != ZUN_SUCCESS)
            {
                return ZUN_ERROR;
            }
        }
        else if (g_EclManager.Load(g_StageSpellEclFiles[g_GameManager.currentStage]) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
    }
    else
    {
        timelineFile = g_EclManager.timelineFile;
        subTable = g_EclManager.unknown;
        memset(&g_EclManager, 0, sizeof(g_EclManager));
        g_EclManager.timelineFile = timelineFile;
        g_EclManager.unknown = subTable;
    }

    *(i16 *)((u8 *)enemyManager + 0x9dcdc0) = g_Rng.GetRandomU16InRange(3);
    *(i16 *)((u8 *)enemyManager + 0x9dcdc2) = g_Rng.GetRandomU16InRange(8);
    position = Float3(-999.0f, -999.0f, -999.0f);
    g_AsciiManager.SetBossMarkerPosition(0, &position);
    g_AsciiManager.SetBossMarkerPosition(1, &position);
    g_AsciiManager.SetBossMarkerPosition(2, &position);
    g_AsciiManager.SetBossMarkerPosition(3, &position);
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x42ee80
#pragma var_order(i, enemy, position)
ZunResult EnemyManager::DeletedCallback(EnemyManager *enemyManager)
{
    Enemy *enemy;
    i32 i;

    enemy = &enemyManager->enemies[0];
    for (i = 0; i < 0x1e0; i++, enemy++)
    {
        enemy->FUN_0042bc90();
    }
    if (!IsDisableResourceReload())
    {
        g_AnmManager->ReleaseAnm(8);
    }
    if (::FUN_004338c0())
    {
        g_AnmManager->ReleaseAnm(7);
    }
    if (!IsDisableResourceReload())
    {
        g_EclManager.FUN_00418420();
    }

    Float3 position = Float3(-999.0f, -999.0f, -999.0f);
    g_AsciiManager.SetBossMarkerPosition(0, &position);
    g_AsciiManager.SetBossMarkerPosition(1, &position);
    g_AsciiManager.SetBossMarkerPosition(2, &position);
    g_AsciiManager.SetBossMarkerPosition(3, &position);
    return ZUN_SUCCESS;
}

// FUNCTION: th08 0x42ef70
void EnemyManager::CutChain()
{
    g_Chain.Cut(&g_EnemyManagerCalcChain);
    g_Chain.Cut(&g_EnemyManagerDrawChainHighPrio);
    g_Chain.Cut(&g_EnemyManagerDrawChainLowPrio);
}

// FUNCTION: th08 0x42efb0
#pragma var_order(score, total, enemy, i, j)
i32 EnemyManager::DespawnAllEnemies(i32 maximumScore, i32 initialTotal)
{
    Enemy *enemy;
    i32 total;
    i32 score;
    i32 i;
    i32 j;

    enemy = &this->enemies[0];
    total = initialTotal;
    score = 2000;
    for (i = 0; i < 0x1e0; i++, enemy++)
    {
        if ((*(u32 *)((u8 *)enemy + 0x3324) & 1) == 0)
        {
            continue;
        }
        if ((*(u32 *)((u8 *)enemy + 0x3324) >> 1 & 1) != 0)
        {
            continue;
        }
        if ((*(u32 *)((u8 *)enemy + 0x3328) >> 6 & 1) != 0)
        {
            continue;
        }

        *(i32 *)((u8 *)enemy + 0x2dfc) = 0;
        if ((*(u32 *)((u8 *)enemy + 0x3324) >> 7 & 1) != 0)
        {
            enemy->position0x2d88 = enemy->position0x2d34 + enemy->position0x2d40;
            g_ItemManager.SpawnItem(&enemy->position0x2d88, ITEM_POINT_STAR, 1);
            g_AsciiManager.CreateScorePopup(&enemy->position0x2d88, score,
                                            score >= maximumScore ? 0xffffff00 : 0xffffffff);
            total += score;
            score += 30;
            if (score > maximumScore)
            {
                score = maximumScore;
            }

            if (*(u8 *)((u8 *)enemy + 0x534c) != 0)
            {
                for (j = 0; j < *(i16 *)((u8 *)enemy + 0x534e); j += 6)
                {
                    g_ItemManager.SpawnItem(&enemy->unk0x3394[j].first, ITEM_POINT_STAR, 1);
                    g_AsciiManager.CreateScorePopup(&enemy->unk0x3394[j].first, score,
                                                    score >= maximumScore ? 0xffffff00 : 0xffffffff);
                    total += score;
                    score += 30;
                    if (score > maximumScore)
                    {
                        score = maximumScore;
                    }
                }
            }
        }

        enemy->FUN_0042b2f0();
        if (*(i16 *)((u8 *)enemy + 0x2cee) >= 0)
        {
            g_EclManager.FUN_00418450((EclTimelineContext *)((u8 *)enemy + 0x7f8),
                                      *(i16 *)((u8 *)enemy + 0x2cee));
            *(i16 *)((u8 *)enemy + 0x2cee) = -1;
        }
    }
    return total;
}

} /* namespace th08 */

// FUNCTION: th08 0x427250
i32 th08::Effect::FUN_00427250()
{
    return 1;
}

ZunBool FUN_0040d3d0(void *data)
{
    return *((i32 *)data + 2) != *(i32 *)data;
}
