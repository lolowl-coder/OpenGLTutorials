#include "Render/RenderContext.h"
#include "Tests/ParticleSystem/ParticleSystemTest.h"

ParticleSystemTest::ParticleSystemTest(const Director& director, const std::string& name)
: Test(director, name)
, mEmitter(NULL)
, mCamera(director)
{
}

void ParticleSystemTest::init()
{
   mParticleSystem.setCount(64);
   mParticleSystem.setTtl(3.0f);
   mParticleSystem.setSize(0.2f);
   Texture spotTexture;
   spotTexture.genSpot(16, Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
   spotTexture.create();
   mParticleSystem.setTexture(spotTexture);
   PointEmitter* pointEmitter = new PointEmitter;
   pointEmitter->setPosition(Vector3f());
   pointEmitter->setVelocityRange(Rangef(-0.2f, 0.2f), Rangef(0.2f, 0.5f), Rangef(-0.2f, 0.2f));
   mEmitter = pointEmitter;
   mParticleSystem.setEmitter(mEmitter);
   mParticleSystem.init();

   mCamera.setPosition(Vector3f(0.0f, 0.0f, 1.0f));
   mCamera.setLookAt(Vector3f(0.0f, 0.0f, -1.0f));
   mCamera.setFOV(80.0f);
   mCamera.setProjectionType(Camera::P_PERSPECTIVE);
   mCamera.update();

   for(int i = 0; i < CHARS_COUNT; i++)
   {
      mKeyState[i] = false;
   }

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void ParticleSystemTest::deinit()
{
   mParticleSystem.deinit();

   delete mEmitter;
   mEmitter = NULL;
}

void ParticleSystemTest::run()
{
   updateCamera();

   mParticleSystem.render(mCamera.getV(), mCamera.getP());
}

void ParticleSystemTest::updateCamera()
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

void ParticleSystemTest::onTouchEvent(TouchEventType eventType, const Vector2f& position)
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

void ParticleSystemTest::onKeyboardEvent(KeyboardEventType eventType, int keyCode)
{
   int sign = 1;

   mKeyState[keyCode] = eventType == KE_DOWN;
}