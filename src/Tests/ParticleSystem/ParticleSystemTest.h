#pragma once

#include "Tests/Test.h"
#include "Render/Camera.h"
#include "Render/ParticleSystem.h"

#define CHARS_COUNT 256

class ParticleSystemTest : public Test
{
public:
   ParticleSystemTest(const Director& director, const std::string& name);

   virtual void init();
   virtual void deinit();
   virtual void run();
   virtual void onTouchEvent(TouchEventType eventType, const Vector2f& position);
   virtual void onKeyboardEvent(KeyboardEventType eventType, int keyCode);

private:
   void updateCamera();

private:
   Emitter* mEmitter;
   ParticleSystem mParticleSystem;
   Vector2f mLastTouchPosition;
   Camera mCamera;
   bool mKeyState[CHARS_COUNT];
};