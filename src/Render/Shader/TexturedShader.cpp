#include "Render/Shader/TexturedShader.h"

TexturedShader::TexturedShader()
: mTextureUnitLocation(-1)
{
}

void TexturedShader::setTextureUnit(int unit)
{
   if(mTextureUnitLocation == -1)
      mTextureUnitLocation = uniformLocation("u_texture");

   uniform1i(mTextureUnitLocation, 1, &unit);
}