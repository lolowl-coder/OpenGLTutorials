#include "Render/Shader/TexturedRectShader.h"
#include "Render/RenderContext.h"

TexturedRectShader::TexturedRectShader()
: RectShader()
, mTextureLocation(-1)
{
}

void TexturedRectShader::init()
{
   load("Data/Shader/BlendEquation/fbo.vs", "Data/Shader/BlendEquation/fbo.fs");

   mOffsetLocation = uniformLocation("uOffset");
   mColorLocation = uniformLocation("uColor");
   mScaleLocation = uniformLocation("uScale");
   mTextureLocation = uniformLocation("uTexture");

   checkGLError("FBOShader::init");
}

void TexturedRectShader::setUniforms(const Vector3f& offset, const Vector4f& color, const Vector3f scale)
{
   RectShader::setUniforms(offset, color, scale);

   const int textureUnit = 0;
   uniform1i(mTextureLocation, 1, &textureUnit);

   checkGLError("FBOShader::setUniforms");
}

