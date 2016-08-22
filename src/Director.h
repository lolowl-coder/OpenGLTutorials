#pragma once

#include "vmath.h"

#include <string>

class Director
{
public:
   Director();

   void update();
   unsigned int getFrame() const;
   Vector2f getViewSize() const;
   float getTimeDelta() const;
   void setTimeDelta(float timeDelta);
   std::string getAppName();
   void setAppName(const std::string& name);

private:
   unsigned int mFrame;
   Vector2f mViewSize;
   float mTimeDelta;
   std::string mAppName;
};