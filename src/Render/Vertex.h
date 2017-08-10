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

struct VertexP3N3
{
   VertexP3N3 ( const Vector3f& position, const Vector3f& normal )
   : mPosition(position)
   , mNormal(normal)
   {
   }

   Vector3f mPosition;
   Vector3f mNormal;
};