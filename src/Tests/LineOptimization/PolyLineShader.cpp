#include "Render/RenderContext.h"
#include "Tests/LineOptimization/PolyLineShader.h"

PolyLineShader::PolyLineShader()
: mColorLocation(-1)
, mPointSizeLocation(-1)
{

}

void PolyLineShader::setColor(const Vector4f& color)
{
   if(mColorLocation == -1)
      mColorLocation = uniformLocation("uColor");

   uniform4(mColorLocation, 1, &color);
   checkGLError("Pass mvp to axes shader");
}

void PolyLineShader::setPointSize(const float pointSize)
{
   if(mPointSizeLocation == -1)
      mPointSizeLocation = uniformLocation("uPointSize");

   uniform1(mPointSizeLocation, 1, &pointSize);
   checkGLError("Pass mvp to axes shader");
}

