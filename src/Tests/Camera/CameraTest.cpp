#include "Render/GLHeaders.h"
#include "Render/RenderContext.h"
#include "Tests/Camera/CameraTest.h"

#include <vector>

AxesShader::AxesShader()
: mMVPLocation(-1)
{

}

void AxesShader::setMVP(const Matrix4f& mvp)
{
   if(mMVPLocation == -1)
      mMVPLocation = uniformLocation("uMVP");

   uniformMatrix4(mMVPLocation, 1, &mvp);
   checkGLError("Pass mvp to axes shader");
}

CameraTest::CameraTest(const Director& director, const std::string& name)
: Test(director, name)
, mCamera(director)
{

}

void CameraTest::initWorldAxes()
{
   Vector3f vertices[] = {Vector3f(), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f)};
   GLuint indices[] = {0, 1, 0, 2, 0, 3};

   glGenBuffers(1, &mWorldAxesVBO);
   checkGLError("Gen axes vbo");
   glBindBuffer(GL_ARRAY_BUFFER, mWorldAxesVBO);
   checkGLError("Bind axes vbo");
   glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vector3f), &vertices[0], GL_STATIC_DRAW);
   checkGLError("Pass grid vertices to GPU");

   glGenBuffers(1, &mWorldAxesIBO);
   checkGLError("Gen axes ibo");
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mWorldAxesIBO);
   checkGLError("Bind axes ibo");

   glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
   checkGLError("Pass axes indices to GPU");
}

void CameraTest::init()
{
   for(int i = 0; i < CHARS_COUNT; i++)
   {
      mKeyState[i] = false;
   }

   initWorldAxes();

   const Vector2i dimensions(10, 10);
   const Vector2f size(10.0f, 10.0f);
   mGrid.init(dimensions, size);

   mAxesShader.load("Data/Shader/Camera/axes.vs", "Data/Shader/Common/colored.fs");

   mCamera.setPosition(Vector3f(0.0f, 0.0f, 0.0f));
   mCamera.setLookAt(Vector3f(0.0f, 0.0f, -1.0f));
   mCamera.setFOV(80.0f);
   mCamera.setProjectionType(Camera::P_PERSPECTIVE);
   mCamera.update();
}

void CameraTest::deinit()
{
   glDeleteBuffers(1, &mWorldAxesVBO);
   glDeleteBuffers(1, &mWorldAxesIBO);

   mGrid.deinit();
   mAxesShader.deinit();

   checkGLError("Delete GL objects");
}

void CameraTest::updateCamera()
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

void CameraTest::renderWorldAxes()
{
   mAxesShader.bind();
   Matrix4f v = mCamera.getV();
   Matrix4f p = mCamera.getP();
   mAxesShader.setMVP(p * v);

   glBindBuffer(GL_ARRAY_BUFFER, mWorldAxesVBO);
   checkGLError("Bind axes vbo");

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mWorldAxesIBO);
   checkGLError("Bind axes ibo");

   glEnableVertexAttribArray(0);
   checkGLError("Enable vertex attrib array");
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
   checkGLError("Set vertex attrib pointer");

   glLineWidth(3);
   glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
}

void CameraTest::run()
{
   updateCamera();

   renderWorldAxes();

   Matrix4f v = mCamera.getV();
   Matrix4f p = mCamera.getP();
   Vector3f eye = mCamera.getPosition();
   mGrid.render(p * v, eye);
}

void CameraTest::onTouchEvent(TouchEventType eventType, const Vector2f& position)
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

void CameraTest::onKeyboardEvent(KeyboardEventType eventType, int keyCode)
{
   int sign = 1;

   mKeyState[keyCode] = eventType == KE_DOWN;
}