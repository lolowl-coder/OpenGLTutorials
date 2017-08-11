#include "Render/Shader/RectShader.h"

RectShader::RectShader()
: Shader()
, mOffsetLocation(-1)
, mColorLocation(-1)
, mScaleLocation(-1)
{
}

void RectShader::init()
{
   load("Data/Shader/BlendEquation/transformedQuad.vs", "Data/Shader/Common/colored.fs");

   mOffsetLocation = uniformLocation("uOffset");
   mColorLocation = uniformLocation("uColor");
   mScaleLocation = uniformLocation("uScale");
}

void RectShader::setUniforms(const Vector3f& offset, const Vector4f& color, const Vector3f scale)
{
   uniform3(mOffsetLocation, 1, &offset);
   uniform4(mColorLocation, 1, &color);
   uniform3(mScaleLocation, 1, &scale);
}