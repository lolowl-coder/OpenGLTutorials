#pragma once

#include "Tests/Camera/ICameraTest.h"
#include "Render/Ground.h"
#include "Render/Cube.h"
#include "Render/FBO.h"

class ShadowTest : public ICameraTest
{
public:
   ShadowTest(const Director& director, const std::string& name);

   virtual void init();
   virtual void deinit();
   virtual void run();

private:
   Ground mGround;
   Cube mCube;
   FBO mShadowBuffer;

};