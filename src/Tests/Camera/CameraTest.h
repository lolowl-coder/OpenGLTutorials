#pragma once

#include "Render/Object/Grid.h"
#include "Tests/Camera/ICameraTest.h"
#include "Tests/Camera/AxesShader.h"

class CameraTest : public ICameraTest
{
public:
   CameraTest(Director& director, const std::string& name);
   virtual void init();
   virtual void deinit();
   virtual void run();

private:
   void initWorldAxes();
   void renderWorldAxes();
   void bindGridShader();

private:
   GLuint mWorldAxesVBO;
   GLuint mWorldAxesIBO;
   AxesShader mAxesShader;

   Grid mGrid;
};