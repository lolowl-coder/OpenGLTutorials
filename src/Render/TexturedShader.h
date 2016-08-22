#pragma once

#include "Render/Shader.h"

class TexturedShader : public Shader
{
public:
   TexturedShader();

   void setTextureUnit(int unit);

private:
   GLint mTextureLocation;

};