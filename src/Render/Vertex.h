#pragma once

#include "vmath.h"

struct VertexP3T2
{
   VertexP3T2 ( const Vector3f& position, const Vector2f& uv )
   : mPosition ( position )
   , mUv ( uv )
   {
   }

   Vector3f mPosition;
   Vector2f mUv;
};