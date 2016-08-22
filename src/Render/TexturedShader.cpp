#include "Render/TexturedShader.h"

TexturedShader::TexturedShader()
: mTextureLocation(-1)
{
}

void TexturedShader::setTextureUnit(int unit)
{
   if(mTextureLocation == -1)
      mTextureLocation = uniformLocation("u_texture");

   uniform1i(mTextureLocation, 1, &unit);
}