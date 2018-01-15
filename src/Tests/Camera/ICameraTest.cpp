#include "Render/GLHeaders.h"
#include "Render/RenderContext.h"
#include "Tests/Camera/ICameraTest.h"

#include <vector>

ICameraTest::ICameraTest(Director& director, const std::string& name)
: Test(director, name)
, mTime(0.0f)
{

}

void ICameraTest::init()
{
   for(int i = 0; i < CHARS_COUNT; i++)
   {
      mKeyState[i] = false;
   }
}

void ICameraTest::updateCamera()
{
   Vector3f cameraVelocity;
   if(mKeyState['A'])
   {
      cameraVelocity.x -= 1.0f;
   }
   if(mKeyState['D'])
   {
      cameraVelocity.x += 1.0f;
   }
   if(mKeyState['W'])
   {
      cameraVelocity.z += 1.0f;
   }
   if(mKeyState['S'])
   {
      cameraVelocity.z -= 1.0f;
   }
   if(mKeyState['1'])
   {
      cameraVelocity.y += 1.0f;
   }
   if(mKeyState['2'])
   {
      cameraVelocity.y -= 1.0f;
   }

   Camera& camera = mDirector.getCamera();
   camera.setVelocity(cameraVelocity);
   camera.update(mDirector.getTimeDelta());
}

void ICameraTest::run()
{
   updateCamera();
}

void ICameraTest::onTouchEvent(TouchEventType eventType, const Vector2f& position)
{
   Camera& camera = mDirector.getCamera();
   switch (eventType)
   {
   case TE_DOWN:
   case TE_UP:
      {
         mLastTouchPosition = position;
         camera.setRotationDelta(Quatf(0.0f, 0.0f, 0.0f, 0.0f));
      }
      break;
   case TE_MOVE:
      {
         Vector2f delta = position - mLastTouchPosition;

         Quatf rotationDeltaQ = Quatf::fromAxisRot(Vector3f(1.0f, 0.0f, 0.0f), delta.y) * Quatf::fromAxisRot(Vector3f(0.0f, 1.0f, 0.0f), delta.x);
         camera.setRotationDelta(rotationDeltaQ);
         camera.setYawPitch(-delta.x, -delta.y);

         mLastTouchPosition = position;
      }
      break;
   }
}

void ICameraTest::onKeyboardEvent(KeyboardEventType eventType, int keyCode)
{
   mKeyState[keyCode] = eventType == KE_DOWN;
}