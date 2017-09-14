#include "Tests/Shadow/ShadowTest.h"
#include "Platform/Log.h"

ShadowTest::ShadowTest(Director& director, const std::string& name)
: ICameraTest(director, name)
, mLight(Vector3f(0.0f, 5.5f, 5.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 100.0f)
, mRenderShadowBuffer(false)
, mLightCamera(mDirector)
{

}

void ShadowTest::init()
{
   ICameraTest::init();

   mShadowBuffer.init(mDirector.getViewSize() * 2, true);
   mShadowBuffer.unbind();

   mGround.setDimensions(Vector2i(10, 10));
   mGround.setSize(Vector2i(10, 10));
   mGround.init();
   Material groundMaterial(Vector4f(0.0f, 0.0f, 0.0f, 1.0f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 4.0f);
   mGround.setMaterial(groundMaterial);
   mGround.setLight(mLight);

   mCube.init();
   Material cubeMaterial(Vector4f(0.0f, 0.0f, 0.0f, 1.0f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 4.0f);
   mCube.setMaterial(cubeMaterial);
   mCube.setLight(mLight);

   mFboShader.init();
   mShaderADSSS.load();

   glDisable(GL_BLEND);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void ShadowTest::deinit()
{
   mGround.deinit();
   mCube.deinit();
   mShadowBuffer.deinit();
   mFboShader.deinit();
}

void ShadowTest::renderDepth()
{
   mShadowBuffer.bind();
   Vector2i viewSize = mDirector.getViewSize();
   Vector2i depthSize = viewSize * 2;
   glViewport(0, 0, depthSize.x, depthSize.y);
   mShadowBuffer.clear();

   renderScene(mLightCamera);

   Vector2i size = mDirector.getViewSize() * 2;
   glViewport(0, 0, viewSize.x, viewSize.y);

   mShadowBuffer.unbind();
}

void ShadowTest::setObjectUniforms(ShaderADSS& shader, const Object& object, const Camera& camera)
{
   mShaderADSSS.setM(object.getTranslation() * object.getRotation());
   mShaderADSSS.setV(camera.getV());
   mShaderADSSS.setP(camera.getP());
   mShaderADSSS.setObjectMaterial(object.getMaterial());
   mShaderADSSS.setLight(object.getLight());
   mShaderADSSS.setEye(camera.getPosition());
}

void ShadowTest::renderScene(const Camera& camera)
{
   glEnable(GL_DEPTH_TEST);

   glDisable(GL_CULL_FACE);
   //glCullFace(GL_BACK);

   mShaderADSSS.bind();
   mShaderADSSS.setLightSpaceMatrix(mLightCamera.getP() * mLightCamera.getV());

   setObjectUniforms(mShaderADSSS, mGround, camera);
   mGround.render(camera);

   float t = mDirector.getTime();
   Matrix4f translation = Matrix4f::createTranslation(4.0f, 0.5f, 4.0f);
   mCube.setTranslation(translation);
   Matrix4f rotation = Matrix4f::createRotationAroundAxis(0.0f, t * 20.0f, 0.0f);
   mCube.setRotation(rotation);
   setObjectUniforms(mShaderADSSS, mCube, camera);
   mCube.render(camera);

   translation = Matrix4f::createTranslation(4.0f, 0.5f, 2.0f);
   mCube.setTranslation(translation);
   rotation = Matrix4f::createRotationAroundAxis(0.0f, t * 30.0f, 0.0f);
   mCube.setRotation(rotation);
   setObjectUniforms(mShaderADSSS, mCube, camera);
   mCube.render(camera);
}

void ShadowTest::onKeyboardEvent(KeyboardEventType eventType, int keyCode)
{
   ICameraTest::onKeyboardEvent(eventType, keyCode);

   if(eventType == KE_UP)
   {
      switch(keyCode)
      {
      case 113: //F2
         mRenderShadowBuffer = !mRenderShadowBuffer;
         break;
      }
   }
}

void ShadowTest::run()
{
   ICameraTest::run();

   mLightCamera.setProjectionType(Camera::P_PERSPECTIVE);
   mLightCamera.setPosition(mLight.mPosition);
   mLightCamera.setLookAt(Vector3f(4.0f, 0.0f, 4.0f));
   mLightCamera.update();

   renderDepth();

   if(mRenderShadowBuffer)
   {
      mShadowBuffer.bindDepthTexture();

      mFboShader.bind();
      mFboShader.setUniforms(Vector3f(-1.0f, -1.0f, 0.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector3f(2.0f, 2.0f, 1.0f));

      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);
      RectRenderer& rectRenderer = mDirector.getRectRenderer();
      rectRenderer.render();
   }
   else
   {
      renderScene(mCamera);
   }
}