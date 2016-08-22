#pragma once

#include "Render/Camera.h"
#include "Render/Grid.h"
#include "Render/Shader.h"
#include "Tests/Test.h"

#define CHARS_COUNT 256

class AxesShader : public Shader
{
public:
   AxesShader();

   void setMVP(const Matrix4f& mvp);

private:
   GLint mMVPLocation;
};

class CameraTest : public Test
{
public:
   CameraTest(const Director& director, const std::string& name);
   virtual void init();
   virtual void deinit();
   virtual void run();
   virtual void onTouchEvent(TouchEventType eventType, const Vector2f& position);
   virtual void onKeyboardEvent(KeyboardEventType eventType, int keyCode);

private:
   void initWorldAxes();
   void updateCamera();
   void renderWorldAxes();
   void bindGridShader();

private:
   GLuint mWorldAxesVBO;
   GLuint mWorldAxesIBO;
   AxesShader mAxesShader;

   Grid mGrid;

   Camera mCamera;
   Vector2f mLastTouchPosition;
   bool mKeyState[CHARS_COUNT];
};