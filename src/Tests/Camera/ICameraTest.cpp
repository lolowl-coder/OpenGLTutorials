#include "Render/GLHeaders.h"
#include "Render/RenderContext.h"
#include "Tests/Camera/ICameraTest.h"

#include <vector>

ICameraTest::ICameraTest(const Director& director, const std::string& name)
: Test(director, name)
, mCamera(director)
, mTime(0.0f)
{

}

void ICameraTest::init()
{
   for(int i = 0; i < CHARS_COUNT; i++)
   {
      mKeyState[i] = false;
   }

   Vector3f cameraPosition(4.0f, 1.8f, 4.0f);
   mCamera.setPosition(cameraPosition);
   mCamera.setLookAt(cameraPosition - Vector3f(1.0f, 0.0f, 1.0f));
   mCamera.setFOV(180.0f);
   mCamera.setProjectionType(Camera::P_PERSPECTIVE);
   mCamera.update();
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

   mCamera.setVelocity(cameraVelocity);
   mCamera.update();
}

void ICameraTest::run()
{
   updateCamera();
}

void ICameraTest::onTouchEvent(TouchEventType eventType, const Vector2f& position)
{
   switch (eventType)
   {
   case TE_DOWN:
   case TE_UP:
      {
         mLastTouchPosition = position;
         mCamera.setRotationDelta(Quatf(0.0f, 0.0f, 0.0f, 0.0f));
      }
      break;
   case TE_MOVE:
      {
         Vector2f delta = position - mLastTouchPosition;

         Quatf rotationDeltaQ = Quatf::fromAxisRot(Vector3f(1.0f, 0.0f, 0.0f), delta.y) * Quatf::fromAxisRot(Vector3f(0.0f, 1.0f, 0.0f), delta.x);
         mCamera.setRotationDelta(rotationDeltaQ);
         mCamera.setYawPitch(-delta.x, -delta.y);

         mLastTouchPosition = position;
      }
      break;
   }
}

void ICameraTest::onKeyboardEvent(KeyboardEventType eventType, int keyCode)
{
   mKeyState[keyCode] = eventType == KE_DOWN;
}