#include "Render/RenderContext.h"
#include "Tests/BlendEquation/BlendEquationTest.h"

BlendEquationTest::BlendEquationTest(Director& director, const std::string& name)
: Test(director, name)
{

}

void BlendEquationTest::init()
{
   Vector2f viewSize = mDirector.getViewSize();

   mFBO.init(Vector2i(viewSize), false);
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
   
   RectRenderer& rectRenderer = mDirector.getRectRenderer();
   
   mRenderShader.setUniforms(Vector3f(0.0f, 0.5f, 0.0f), Vector4f(1.0f, 1.0f, 1.0f, 0.5f), Vector3f(0.3f, 0.3f, 1.0f));
   rectRenderer.render();

   mRenderShader.setUniforms(Vector3f(-0.05f, 0.0f, 0.0f), Vector4f(0.5f, 0.5f, 0.5f, 0.4f), Vector3f(0.3f, 0.3f, 1.0f));
   rectRenderer.render();

   mRenderShader.setUniforms(Vector3f(0.05f, 0.0f, 0.0f), Vector4f(0.5f, 0.5f, 0.5f, 0.4f), Vector3f(0.3f, 0.3f, 1.0f));
   rectRenderer.render();

   mFBO.unbind();
   mFBOShader.bind();
   glBlendEquation(GL_FUNC_ADD);

   mFBOShader.setUniforms(Vector3f(-1.0f, -1.0f, 0.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector3f(2.0f, 2.0f, 1.0f));
   rectRenderer.render();

   glClearColor ( 1.0f, 0.0f, 0.0f, 1.0f );

   checkGLError("BlendEquationTest::run");
}
