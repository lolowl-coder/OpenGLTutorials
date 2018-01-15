#pragma once

#include "Render/Camera.h"
#include "Render/Object/Grid.h"
#include "Render/Shader/Shader.h"
#include "Tests/Test.h"

#define CHARS_COUNT 256

class ICameraTest : public Test
{
public:
   ICameraTest(Director& director, const std::string& name);
   virtual void init();
   virtual void run();
   virtual void onTouchEvent(TouchEventType eventType, const Vector2f& position);
   virtual void onKeyboardEvent(KeyboardEventType eventType, int keyCode);

private:
   void updateCamera();

private:
   Vector2f mLastTouchPosition;
   bool mKeyState[CHARS_COUNT];

   float mTime;
};