#pragma once

#include "vmath.h"

struct Light
{
   Light();
   Light(const Vector3f& position, const Vector4f& ambient, const Vector4f& diffuse, const Vector4f& specular, float attenuation);

   Vector3f mPosition;
   Vector4f mAmbient;
   Vector4f mDiffuse;
   Vector4f mSpecular;
   float mAttenuation;
};