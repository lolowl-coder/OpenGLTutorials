#pragma once

#include "Tests/Test.h"
#include "Render/Shader.h"
#include "Render/FBO.h"

class RenderShader : public Shader
{
public:
   RenderShader();

   virtual void init();
   virtual void setUniforms(const Vector3f& offset, const Vector4f& color, const Vector3f scale);

protected:
   GLint mOffsetLocation;
   GLint mColorLocation;
   GLint mScaleLocation;
};

class FBOShader : public RenderShader
{
public:
   FBOShader();

   virtual void init();
   virtual void setUniforms(const Vector3f& offset, const Vector4f& color, const Vector3f scale);

private:
   GLint mTextureLocation;
};

class BlendEquationTest : public Test
{
public:
   BlendEquationTest(const Director& director, const std::string& name);

   virtual void init();
   virtual void deinit();
   virtual void run();

private:
   void renderQuad(RenderShader* shader, const Vector3f& offset, const Vector4f& color, const Vector3f& scale);

private:
   FBO mFBO;
   RenderShader mRenderShader;
   FBOShader mFBOShader;
};