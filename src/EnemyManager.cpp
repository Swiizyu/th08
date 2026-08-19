#include "th_pch.h"

#include "EnemyManager.hpp"
#include "AsciiManager.hpp"
#include "Background.hpp"
#include "EffectManager.hpp"
#include "EclManager.hpp"
#include "Spellcard.hpp"
#include "ScreenEffect.hpp"
#include "ItemManager.hpp"
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
    {88, NULL, NULL},
    {92, NULL, NULL},
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
    {74, NULL, NULL},
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

// FUNCTION: th08 0x42a410
EnemyUnkStruct4::EnemyUnkStruct4()
{
    memset(this, 0, sizeof(EnemyUnkStruct4));
    this->unk0x204 = -1;
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
    if (this->timer == 40)
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
    if (this->timer == 40)
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
    if (this->timer == 40)
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
    i32 offset;

    *(u8 *)((u8 *)this + 0x356) = 1;
    *(i32 *)((u8 *)this + 0x324) = 48;
    *(f32 *)((u8 *)this + 0x320) = 32.0f;
    offset = -((this->timer.AsFrames() & 1) != 0) & 8;
    *(f32 *)((u8 *)this + 0x314) = 64.0f + (f32)offset;
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
    ScreenEffect::RegisterChain(SCREEN_EFFECT_FULL_FADE_OUT, 180, 1, -1, 0, 21);
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

// FUNCTION: th08 0x428590
#pragma var_order(effect, i)
ZunResult EffectManager::FUN_00428590()
{
    Effect *effect;
    i32 i;

    effect = (Effect *)((u8 *)this + 0x1c);
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

// FUNCTION: th08 0x41fd20
ZunBool Enemy::FUN_0041fd20()
{
    return *(i32 *)((u8 *)this + 0x2da4) != 0;
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

// STUB: th08 0x42c660
ChainCallbackResult EnemyManager::OnUpdate()
{
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: th08 0x42e120
ChainCallbackResult EnemyManager::OnDrawHighPrio(EnemyManager *enemyManager)
{
    return enemyManager->OnDrawImpl(0, 2);
}

// STUB: th08 0x42e140
ChainCallbackResult EnemyManager::OnDrawImpl(i32 firstLayer, i32 lastLayer)
{
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

// FUNCTION: th08 0x40d3d0
ZunBool FUN_0040d3d0(void *data)
{
    return *((i32 *)data + 2) != *(i32 *)data;
}

