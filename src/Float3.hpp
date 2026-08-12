#pragma once

// The Float3 situation is a bit strange. It appears ZUN copied the
// D3DXVECTOR3 vector type wholesale into the codebase, called it
// "FVector" (as confirmed by the later games), and added some
// additional methods (e.g. FromAngleMagnitude). D3DXVECTOR3 is split
// between two files, the file that defines the datatype, "d3dx8math.h",
// and the file that contains inline definitions for the methods,
// "d3dx8math.inl". It appears the inline definition file is not in
// the precompiler header, due to the placement of the function, but the
// file containing the definition of the class is, because it's used by
// AnmManager and such. Also, due to COMDAT folding, we can't tell (at
// the moment of writing), if a method is from D3DXVECTOR3 or Float3.

#include "Global.hpp"

namespace th08
{

inline Float3::Float3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

} // namespace th08
