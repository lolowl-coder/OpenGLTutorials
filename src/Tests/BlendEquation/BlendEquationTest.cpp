#include "Render/RenderContext.h"
#include "Tests/BlendEquation/BlendEquationTest.h"

RenderShader::RenderShader()
: Shader()
, mOffsetLocation(-1)
, mColorLocation(-1)
, mScaleLocation(-1)
{
}

void RenderShader::init()
{
   load("Data/Shader/BlendEquation/transformedQuad.vs", "Data/Shader/Common/colored.fs");

   mOffsetLocation = uniformLocation("uOffset");
   mColorLocation = uniformLocation("uColor");
   mScaleLocation = uniformLocation("uScale");
}

void RenderShader::setUniforms(const Vector3f& offset, const Vector4f& color, const Vector3f scale)
{
   uniform3(mOffsetLocation, 1, &offset);
   uniform4(mColorLocation, 1, &color);
   uniform3(mScaleLocation, 1, &scale);
}

FBOShader::FBOShader()
: RenderShader()
, mTextureLocation(-1)
{
}

void FBOShader::init()
{
   load("Data/Shader/BlendEquation/fbo.vs", "Data/Shader/BlendEquation/fbo.fs");

   mOffsetLocation = uniformLocation("uOffset");
   mColorLocation = uniformLocation("uColor");
   mScaleLocation = uniformLocation("uScale");
   mTextureLocation = uniformLocation("uTexture");

   checkGLError("FBOShader::init");
}

void FBOShader::setUniforms(const Vector3f& offset, const Vector4f& color, const Vector3f scale)
{
   RenderShader::setUniforms(offset, color, scale);

   const int textureUnit = 0;
   uniform1i(mTextureLocation, 1, &textureUnit);

   checkGLError("FBOShader::setUniforms");
}

BlendEquationTest::BlendEquationTest(const Director& director, const std::string& name)
: Test(director, name)
{

}

void BlendEquationTest::init()
{
   Vector2f viewSize = mDirector.getViewSize();

   mFBO.init(Vector2i(viewSize));
   mRenderShader.init();
   mFBOShader.init();

   glDisable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);
   glEnable(GL_BLEND);
}

void BlendEquationTest::deinit()
{
   mRenderShader.deinit();
   mFBOShader.deinit();
   mFBO.deinit();
   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
}

static Vector3f vertices[] = {
   Vector3f(0.0f, 0.0f, 0.0f),
   Vector3f(0.0f, 1.0f, 0.0f),
   Vector3f(1.0f, 0.0f, 0.0f),
   Vector3f(1.0f, 1.0f, 0.0f)
};

void BlendEquationTest::renderQuad(RenderShader* shader, const Vector3f& offset, const Vector4f& color, const Vector3f& scale)
{
   shader->setUniforms(offset, color, scale);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void *)vertices);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   glDisableVertexAttribArray(0);
   checkGLError("BlendEquationTest::renderQuad");
}

void BlendEquationTest::run()
{
   checkGLError("BlendEquationTest::run, disable depth test");

   mFBO.bind();
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   mFBO.clear();

   glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
   //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   checkGLError("BlendEquationTest::run, blendFuncSeparate");
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   //glBlendEquation(GL_MAX);
   glBlendEquationSeparate(GL_MAX, GL_MAX);
   mRenderShader.bind();
   renderQuad(&mRenderShader, Vector3f(0.0f, 0.5f, 0.0f), Vector4f(1.0f, 1.0f, 1.0f, 0.5f), Vector3f(0.3f, 0.3f, 1.0f));
   renderQuad(&mRenderShader, Vector3f(-0.05f, 0.0f, 0.0f), Vector4f(0.5f, 0.5f, 0.5f, 0.4f), Vector3f(0.3f, 0.3f, 1.0f));
   renderQuad(&mRenderShader, Vector3f(0.05f, 0.0f, 0.0f), Vector4f(0.5f, 0.5f, 0.5f, 0.4f), Vector3f(0.3f, 0.3f, 1.0f));
   renderQuad(&mRenderShader, Vector3f(0.0f, 0.0f, 0.0f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f), Vector3f(0.2f, 0.2f, 1.0f));

   mFBO.unbind();
   mFBOShader.bind();
   glBlendEquation(GL_FUNC_ADD);
   renderQuad(&mFBOShader, Vector3f(-1.0f, -1.0f, 0.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector3f(2.0f, 2.0f, 1.0f));

   glClearColor ( 1.0f, 0.0f, 0.0f, 1.0f );

   checkGLError("BlendEquationTest::run");
}
