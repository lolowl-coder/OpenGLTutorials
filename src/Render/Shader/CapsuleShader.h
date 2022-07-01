#pragma once

#include "Render/Shader/Shader.h"

class CapsuleShader : public Shader
{
public:
   CapsuleShader();

   void init();
   void setUniforms(const Vector4f& color, const Matrix4f& mvp);

protected:
   GLint mColorLocation;
   GLint mMVPLocation;
};