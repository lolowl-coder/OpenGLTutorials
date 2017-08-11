#pragma once

#include "Tests/Test.h"
#include "Render/Buffer.h"
#include "Render/FBO.h"
#include "Render/Texture.h"
#include "Render/Shader/TexturedShader.h"
#include "Render/Shader/Shader.h"

class TransformedQuadShader : public Shader
{
public:
   TransformedQuadShader();

   virtual void init();
   virtual void setUniforms(const Vector3f& offset, const Vector4f& color, const Vector3f scale);

protected:
   GLint mOffsetLocation;
   GLint mColorLocation;
   GLint mScaleLocation;
};

class BlurShader : public Shader
{
public:
   BlurShader();

   virtual void init();
   void setUniforms(const int textureUnit, const Vector2f& step, const int radius);

private:
   GLint mTextureUnitLocation;
   GLint mStepLocation;
   GLint mRadiusLocation;
};

class CheckerShader : public Shader
{
public:
   CheckerShader();

   virtual void init();
   void setUniforms(const int textureUnit, const Vector3f& offset, const Vector4f& color, const Vector3f scale, const Vector2f& tile);

private:
   GLint mTextureUnitLocation;
   GLint mOffsetLocation;
   GLint mColorLocation;
   GLint mScaleLocation;
   GLint mTileLocation;
};

class GlowTest : public Test
{
public:
   GlowTest(Director& director, const std::string& name);

   virtual void init();
   virtual void deinit();
   virtual void run();

private:
   void renderCheckerBackground();
   void render();

private:
   TransformedQuadShader mMainObjectShader;
   BlurShader mBlurShader;
   Buffer mQuad;
   FBO mMainObjectFBO;
   FBO mHBlurFBO;
   FBO mVBlurFBO;

   Texture mCheckerTexture;
   CheckerShader mCheckerShader;
};