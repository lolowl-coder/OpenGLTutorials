#pragma once

#include "Render/Shader/Shader.h"

class RectShader : public Shader
{
public:
   RectShader();

   void init();
   void setUniforms(const Vector3f& offset, const Vector4f& color, const Vector3f scale);

protected:
   GLint mOffsetLocation;
   GLint mColorLocation;
   GLint mScaleLocation;
};