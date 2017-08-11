#pragma once

#include "Render/Shader/TexturedShader.h"
#include "Tests/Test.h"
#include "Render/Texture.h"

class InstancingTest : public Test
{
public:
   InstancingTest(Director& director, const std::string& name);

   virtual void init();
   virtual void deinit();
   virtual void run();

private:
   TexturedShader mShader;
   Texture mTexture;

};