#include "Director.h"

Director::Director()
: mFrame(0)
, mViewSize(800.0f, 800.0f)
, mTimeDelta(0.0f)
, mTime(0.0f)
, mAppName("OpenGL-application")
, mCamera()
{

}

void Director::init()
{
   mRectRenderer.init();

   Vector3f cameraPosition(4.0f, 1.8f, 4.0f);
   mCamera.setPosition(cameraPosition);
   mCamera.setLookAt(cameraPosition - Vector3f(1.0f, 0.0f, 1.0f));
   mCamera.setFOV(180.0f);
   mCamera.setProjectionType(Camera::P_PERSPECTIVE);
   mCamera.update(getTimeDelta());
}

void Director::deinit()
{
   mRectRenderer.deinit();
}

void Director::update()
{
   mFrame++;
}

unsigned int Director::getFrame() const
{
   return mFrame;
}

Vector2f Director::getViewSize() const
{
   return mViewSize;
}

float Director::getTimeDelta() const
{
   return mTimeDelta;
}

float Director::getTime() const
{
   return mTime;
}

void Director::setTimeDelta(const float timeDelta)
{
   mTimeDelta = timeDelta;
}

void Director::setTime(const float time)
{
   mTime = time;
}

std::string Director::getAppName()
{
   return mAppName;
}

void Director::setAppName(const std::string& name)
{
   mAppName = name;
}

RectRenderer& Director::getRectRenderer()
{
   return mRectRenderer;
}

Camera& Director::getCamera()
{
   return mCamera;
}

void Director::setCamera(const Camera& camera)
{
   mCamera = camera;
}
