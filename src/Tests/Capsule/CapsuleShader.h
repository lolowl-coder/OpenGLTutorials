#pragma once

#include "Render/Shader/Shader.h"

class PolyLineShader : public Shader
{
public:
   PolyLineShader();

   void setColor(const Vector4f& color);
   void setPointSize(const float pointSize);

private:
   GLint mColorLocation;
   GLint mPointSizeLocation;
};