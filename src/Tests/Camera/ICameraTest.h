#pragma once

#include "Render/Camera.h"
#include "Render/Grid.h"
#include "Render/Shader.h"
#include "Tests/Test.h"

#define CHARS_COUNT 256

class ICameraTest : public Test
{
public:
   ICameraTest(const Director& director, const std::string& name);
   virtual void init();
   virtual void run();
   virtual void onTouchEvent(TouchEventType eventType, const Vector2f& position);
   virtual void onKeyboardEvent(KeyboardEventType eventType, int keyCode);

private:
   void updateCamera();

protected:
   Camera mCamera;

private:
   Vector2f mLastTouchPosition;
   bool mKeyState[CHARS_COUNT];

   float mTime;
};