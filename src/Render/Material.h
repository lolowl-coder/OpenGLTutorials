#pragma once

#include "vmath.h"

struct Material
{
   Material();
   Material(Vector4f& ambient, Vector4f& diffuse, Vector4f& specular, float shininess);

   Vector4f mAmbient;
   Vector4f mDiffuse;
   Vector4f mSpecular;
   float mShininess;
};