#include "Tests/Shadow/ShadowTest.h"

ShadowTest::ShadowTest(const Director& director, const std::string& name)
: ICameraTest(director, name)
{

}

void ShadowTest::init()
{
   ICameraTest::init();

   mShadowBuffer.init(mDirector.getViewSize());

   Light light(Vector3f(0.0f, 1.5f, 5.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 100.0f);

   mGround.setDimensions(Vector2i(10, 10));
   mGround.setSize(Vector2i(10, 10));
   mGround.init();
   Material groundMaterial(Vector4f(0.0f, 0.0f, 0.0f, 1.0f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 4.0f);
   mGround.setMaterial(groundMaterial);
   mGround.setLight(light);

   mCube.init();
   Material cubeMaterial(Vector4f(0.0f, 0.0f, 0.0f, 1.0f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 4.0f);
   mCube.setMaterial(cubeMaterial);
   mCube.setLight(light);

   glEnable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
}

void ShadowTest::deinit()
{
   mGround.deinit();
   mCube.deinit();
   mShadowBuffer.deinit();
}

void ShadowTest::run()
{
   mShadowBuffer.bind();
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   mShadowBuffer.clear();

   ICameraTest::run();

   mGround.render(mCamera);

   float t = mDirector.getTime();
   Matrix4f translation = Matrix4f::createTranslation(2.0f, 0.5f, 1.0f);
   mCube.setTranslation(translation);
   Matrix4f rotation = Matrix4f::createRotationAroundAxis(0.0f, t * 20.0f, 0.0f);
   mCube.setRotation(rotation);
   mCube.render(mCamera);

   mShadowBuffer.unbind();
   mShadowBuffer.bindTexture();

}