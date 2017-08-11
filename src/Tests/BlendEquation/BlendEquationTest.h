#pragma once

#include "Tests/Test.h"
#include "Render/Shader/TexturedRectShader.h"
#include "Render/FBO.h"

class BlendEquationTest : public Test
{
public:
   BlendEquationTest(Director& director, const std::string& name);

   virtual void init();
   virtual void deinit();
   virtual void run();

private:
   FBO mFBO;
   RectShader mRenderShader;
   TexturedRectShader mFBOShader;
};