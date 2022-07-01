#include "Render/Shader/CapsuleShader.h"

CapsuleShader::CapsuleShader()
: Shader()
, mColorLocation(-1)
{
}

void CapsuleShader::init()
{
   load("Data/Shader/Capsule/capsule.vs", "Data/Shader/Capsule/capsule.fs");

   mColorLocation = uniformLocation("uColor");
   mMVPLocation = uniformLocation("uMVP");
}

void CapsuleShader::setUniforms(const Vector4f& color, const Matrix4f& mvp)
{
   uniform4(mColorLocation, 1, &color);
   uniformMatrix4(mMVPLocation, 1, &mvp);
}