#pragma once

#include "Render/Shader/RectShader.h"

class TexturedRectShader : public RectShader
{
public:
   TexturedRectShader();

   virtual void init();
   virtual void setUniforms(const Vector3f& offset, const Vector4f& color, const Vector3f scale);

private:
   GLint mTextureLocation;
};