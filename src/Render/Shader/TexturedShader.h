#pragma once

#include "Render/Shader/Shader.h"

class TexturedShader : public Shader
{
public:
   TexturedShader();

   void setTextureUnit(int unit);

private:
   GLint mTextureUnitLocation;

};