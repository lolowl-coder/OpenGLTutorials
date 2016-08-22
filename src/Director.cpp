#include "Director.h"

Director::Director()
: mFrame(0)
, mViewSize(800.0f, 800.0f)
, mTimeDelta(0.0f)
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

void Director::setTimeDelta(float timeDelta)
{
   mTimeDelta = timeDelta;
}

std::string Director::getAppName()
{
   return mAppName;
}

void Director::setAppName(const std::string& name)
{
   mAppName = name;
}
