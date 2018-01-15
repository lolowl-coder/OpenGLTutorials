#pragma once

#include "Tests/Test.h"
#include "Render/Object/ParticleSystem.h"

#define CHARS_COUNT 256

class ParticleSystemTest : public Test
{
public:
   ParticleSystemTest(Director& director, const std::string& name);

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
   bool mKeyState[CHARS_COUNT];
};