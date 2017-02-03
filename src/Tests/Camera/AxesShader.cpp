#include "Render/RenderContext.h"
#include "Tests/Camera/AxesShader.h"

AxesShader::AxesShader()
: mMVPLocation(-1)
{

}

void AxesShader::setMVP(const Matrix4f& mvp)
{
   if(mMVPLocation == -1)
      mMVPLocation = uniformLocation("uMVP");

   uniformMatrix4(mMVPLocation, 1, &mvp);
   checkGLError("Pass mvp to axes shader");
}

