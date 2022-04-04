#include "Tests/Shadow/ShadowTest.h"
#include "Platform/Log.h"
#include "Render/RenderContext.h"

ShadowTest::ShadowTest(Director& director, const std::string& name)
: ICameraTest(director, name)
, mLight(Vector3f(0.0f, 5.5f, 5.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 100.0f)
, mRenderShadowBuffer(false)
, mLightCamera()
, mUpdate(true)
, mTime(0.0f)
{

}

void ShadowTest::init()
{
   ICameraTest::init();

   mShadowBufferSize = mDirector.getViewSize() * 4.0f; 
   mShadowBuffer.init(mShadowBufferSize, true);
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

   mLightShader.init();

   mRectRenderer.init();

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
   glViewport(0, 0, mShadowBufferSize.x, mShadowBufferSize.y);
   mShadowBuffer.clear();

   glEnable(GL_CULL_FACE);
   glCullFace(GL_FRONT);
   static float factor = 1.005f;
   static float units = 1.0f;
   glPolygonOffset(factor, units);
   glEnable(GL_POLYGON_OFFSET_FILL);
   renderScene(mLightCamera);
   glDisable(GL_POLYGON_OFFSET_FILL);
   //glCullFace(GL_FRONT);
   glDisable(GL_CULL_FACE);

   Vector2i size = mDirector.getViewSize();
   glViewport(0, 0, size.x, size.y);

   mShadowBuffer.unbind();
}

void ShadowTest::setObjectUniforms(ShaderADSS& shader, const Object& object, const Camera& camera)
{
   /*mShaderADSSS.setM(object.getTranslation() * object.getRotation());
   mShaderADSSS.setV(camera.getV());
   mShaderADSSS.setP(camera.getP());
   mShaderADSSS.setObjectMaterial(object.getMaterial());
   mShaderADSSS.setLight(object.getLight());
   mShaderADSSS.setEye(camera.getPosition());
   mShaderADSSS.setLightDir(object.getTranslation().getTranslation() - object.getLight().mPosition);*/
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

   float t = mTime;
   Matrix4f translation = Matrix4f::createTranslation(4.0f, 0.5f, 4.0f);
   mCube.setTranslation(translation);
   Matrix4f rotation = Matrix4f::createRotationAroundAxis(0.0f, t * 20.0f, 0.0f);
   mCube.setRotation(rotation);
   setObjectUniforms(mShaderADSSS, mCube, camera);

   mCube.render(camera);

   translation = Matrix4f::createTranslation(4.0f, 0.5f, 6.0f);
   mCube.setTranslation(translation);
   rotation = Matrix4f::createRotationAroundAxis(0.0f, t * 30.0f, 0.0f);
   mCube.setRotation(rotation);
   setObjectUniforms(mShaderADSSS, mCube, camera);
   
   mCube.render(camera);
}

void ShadowTest::renderLight()
{
   mLightShader.bind();
   //Vector4f p = mCamera.getV() * Vector4f(mLight.mPosition.x, mLight.mPosition.y, mLight.mPosition.z, 1.0f);
   Camera& camera = mDirector.getCamera();
   Vector4f p = camera.getV() * Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
   mLightShader.setUniforms(Vector3f(p.x, p.y, p.z), mLight.mDiffuse, Vector3f(1.0f, 1.0f, 1.0f));
   mRectRenderer.render();
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
      case 32: //space
         mUpdate = !mUpdate;
         break;
      }
   }
}

void ShadowTest::update()
{
   if(mUpdate)
   {
      mTime += mDirector.getTimeDelta();
   }

   float t = mTime * 2.0f;
   mLight.mPosition.x = sin(t) * 5.0f;
   mLight.mPosition.z = cos(t) * 5.0f;
}

void ShadowTest::run()
{
   ICameraTest::run();

   update();

   mLightCamera.setProjectionType(Camera::P_PERSPECTIVE);
   mLightCamera.setPosition(mLight.mPosition);
   mLightCamera.setLookAt(Vector3f(4.0f, 0.0f, 4.0f));

   mLightCamera.update(mDirector.getTimeDelta());

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
      renderScene(mDirector.getCamera());

      renderLight();
   }
}