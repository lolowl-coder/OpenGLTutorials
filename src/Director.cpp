#include "Director.h"

Director::Director()
: mFrame(0)
, mViewSize(800.0f, 800.0f)
, mTimeDelta(0.0f)
, mTime(0.0f)
, mAppName("OpenGL-application")
{

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
