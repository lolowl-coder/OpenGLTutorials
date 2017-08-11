#pragma once

#include "Render/Shader/Shader.h"

class AxesShader : public Shader
{
public:
   AxesShader();

   void setMVP(const Matrix4f& mvp);

private:
   GLint mMVPLocation;
};