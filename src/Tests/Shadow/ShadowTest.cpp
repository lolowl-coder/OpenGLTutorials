#include "Tests/Shadow/ShadowTest.h"

ShadowTest::ShadowTest(Director& director, const std::string& name)
: ICameraTest(director, name)
, mLight(Vector3f(0.0f, 1.5f, 5.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 100.0f)
, mRenderShadowBuffer(false)
, mRenderDepth(false)
{

}

void ShadowTest::init()
{
   ICameraTest::init();

   mShadowBuffer.init(mDirector.getViewSize(), true);
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
   mShadowBuffer.clear();

   Camera lightCamera(mDirector);
   lightCamera.setProjectionType(Camera::P_ORTHO);
   lightCamera.setPosition(mLight.mPosition);
   lightCamera.setLookAt(Vector3f(0.0f, 0.0f, 0.0f));
   lightCamera.update();

   renderScene(lightCamera);

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

   setObjectUniforms(mShaderADSSS, mGround, camera);
   mGround.render(camera);

   float t = mDirector.getTime();
   Matrix4f translation = Matrix4f::createTranslation(2.0f, 0.5f, 1.0f);
   mCube.setTranslation(translation);
   Matrix4f rotation = Matrix4f::createRotationAroundAxis(0.0f, t * 20.0f, 0.0f);
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
      case 114: //F3
         mRenderDepth = !mRenderDepth;
         break;
      }
   }
}

void ShadowTest::run()
{
   ICameraTest::run();

   renderDepth();

   if(mRenderShadowBuffer)
   {
      if(mRenderDepth)
      {
         mShadowBuffer.bindDepthTexture();
      }
      else
      {
         mShadowBuffer.bindColorTexture();
      }

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