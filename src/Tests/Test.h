#pragma once

#include "Event.h"
#include "vmath.h"

#include "Director.h"

#include <string>

class Test
{
public:
   Test(const Director& director, const std::string& name);
   virtual void init() = 0;
   virtual void deinit() = 0;
   virtual void run() = 0;
   virtual void onTouchEvent(TouchEventType eventType, const Vector2f& position);
   virtual void onKeyboardEvent(KeyboardEventType eventType, int keyCode);

protected:
   const Director& mDirector;
   std::string mName;
};