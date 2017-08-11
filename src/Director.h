#pragma once

#include "Render/RectRenderer.h"

#include "vmath.h"

#include <string>

class Director
{
public:
   Director();

   void init();
   void deinit();
   void update();
   unsigned int getFrame() const;
   Vector2f getViewSize() const;
   float getTimeDelta() const;
   float getTime() const;
   void setTimeDelta(const float timeDelta);
   void setTime(const float time);
   std::string getAppName();
   void setAppName(const std::string& name);
   RectRenderer& getRectRenderer();

private:
   RectRenderer mRectRenderer;
   unsigned int mFrame;
   Vector2f mViewSize;
   float mTimeDelta;
   float mTime;
   std::string mAppName;
};