#include "Tests/Glow/GlowTest.h"

TransformedQuadShader::TransformedQuadShader()
: Shader()
, mOffsetLocation(-1)
, mColorLocation(-1)
, mScaleLocation(-1)
{
}

void TransformedQuadShader::init()
{
   load("Data/Shader/Glow/transformedQuad.vs", "Data/Shader/Common/colored.fs");

   mOffsetLocation = uniformLocation("uOffset");
   mColorLocation = uniformLocation("uColor");
   mScaleLocation = uniformLocation("uScale");
}

void TransformedQuadShader::setUniforms(const Vector3f& offset, const Vector4f& color, const Vector3f scale)
{
   uniform3(mOffsetLocation, 1, &offset);
   uniform4(mColorLocation, 1, &color);
   uniform3(mScaleLocation, 1, &scale);
}

BlurShader::BlurShader()
: Shader()
, mTextureUnitLocation(-1)
, mStepLocation(-1)
, mRadiusLocation(-1)
{

}

void BlurShader::init()
{
   load("Data/Shader/Glow/blur.vs", "Data/Shader/Glow/blur.fs");

   mTextureUnitLocation = uniformLocation("uTextureUnit");
   mStepLocation = uniformLocation("uStep");
   mRadiusLocation = uniformLocation("uRadius");
}

void BlurShader::setUniforms(const int textureUnit, const Vector2f& step, const int radius)
{
   uniform1i(mTextureUnitLocation, 1, &textureUnit);
   uniform2(mStepLocation, 1, &step);
   uniform1i(mRadiusLocation, 1, &radius);
}

CheckerShader::CheckerShader()
: Shader()
, mTextureUnitLocation(-1)
, mOffsetLocation(-1)
, mColorLocation(-1)
, mScaleLocation(-1)
{

}

void CheckerShader::init()
{
   load("Data/Shader/Glow/checker.vs", "Data/Shader/Glow/checker.fs");

   mTextureUnitLocation = uniformLocation("uTexture");
   mOffsetLocation = uniformLocation("uOffset");
   mColorLocation = uniformLocation("uColor");
   mScaleLocation = uniformLocation("uScale");
   mTileLocation = uniformLocation("uTile");
}

void CheckerShader::setUniforms(const int textureUnit, const Vector3f& offset, const Vector4f& color, const Vector3f scale, const Vector2f& tile)
{
   uniform1i(mTextureUnitLocation, 1, &textureUnit);
   uniform3(mOffsetLocation, 1, &offset);
   uniform4(mColorLocation, 1, &color);
   uniform3(mScaleLocation, 1, &scale);
   uniform2(mTileLocation, 1, &tile);
}

GlowTest::GlowTest(const Director& director, const std::string& name)
: Test(director, name)
, mQuad(GL_ARRAY_BUFFER, 0, 0)
{
   mQuad.addAttribute(Attribute(3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0));
}

static Vector3f vertices[] =
{
   Vector3f(0.0, 0.0, 0.0),
   Vector3f(0.0, 1.0, 0.0),
   Vector3f(1.0, 0.0, 0.0),
   Vector3f(1.0, 1.0, 0.0)
};

void GlowTest::init()
{
   mQuad.setData(sizeof(vertices), &vertices);
   mMainObjectShader.init();
   mBlurShader.init();
   Vector2f viewSize = mDirector.getViewSize();
   mMainObjectFBO.init(viewSize);
   mMainObjectFBO.unbind();
   mCheckerShader.init();
   mCheckerTexture.genChecker(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0.0f, 0.0f, 0.0f), 32);
   mCheckerTexture.create();

   mHBlurFBO.init(viewSize);
   mVBlurFBO.init(viewSize);

   glDisable(GL_CULL_FACE);
   glDisable(GL_DEPTH_TEST);
}

void GlowTest::deinit()
{
   mQuad.deinit();
   mMainObjectShader.deinit();
   mBlurShader.deinit();
   mCheckerTexture.deinit();
   mHBlurFBO.deinit();
   mVBlurFBO.deinit();
}

void GlowTest::render()
{
   mMainObjectShader.bind();
   mMainObjectShader.setUniforms(Vector3f(-0.5f, -0.5f, 0.0f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f), Vector3f(1.0f, 1.0f, 1.0f));
   mQuad.enable();
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void GlowTest::renderCheckerBackground()
{
   mCheckerShader.bind();
   mCheckerShader.setUniforms(0, Vector3f(-0.5f, -0.5f, 0.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector3f(2.0f, 2.0f, 1.0f), Vector2f(20.0f, 20.0f));
   mCheckerTexture.bind();
   /*mMainObjectShader.bind();
   mMainObjectShader.setUniforms(Vector3f(-1.0f, -1.0f, 0.0f), Vector4f(0.5f, 0.5f, 0.5f, 1.0f), Vector3f(2.0f, 2.0f, 1.0f));*/
   mQuad.enable();
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void GlowTest::run()
{
   FBO::unbind();

   glDisable(GL_BLEND);
   renderCheckerBackground();

   mMainObjectFBO.bind();
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);
   render();

   //blur

   Vector2i viewSize = mDirector.getViewSize();
   const int blurRadius = 55;

   mBlurShader.bind();

   //h-blur
   mHBlurFBO.bind();
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);
   Vector2f step(1.0f / viewSize.x * 1.0, 0.0f);
   mBlurShader.setUniforms(0, step, blurRadius);
   mMainObjectFBO.bindTexture();
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   //v-blur
   mVBlurFBO.bind();
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);
   step.x = 0.0f;
   step.y = 1.0f / viewSize.y * 1.0f;
   mBlurShader.setUniforms(0, step, blurRadius);
   mHBlurFBO.bindTexture();
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   
   FBO::unbind();

   //draw on-screen
   glEnable(GL_BLEND);
   //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE);
   glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   mCheckerShader.bind();
   mCheckerShader.setUniforms(0, Vector3f(-0.5f, -0.5f, 0.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector3f(2.0f, 2.0f, 1.0f), Vector2f(1.0f, 1.0f));
   mVBlurFBO.bindTexture();
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
