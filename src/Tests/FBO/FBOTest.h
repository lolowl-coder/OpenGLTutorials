#pragma once

#include "Tests/Test.h"
#include "Render/FBO.h"

class FBOTest : public Test
{
public:
   FBOTest(Director& director, const std::string& name);

   virtual void init();
   virtual void deinit();
   virtual void run();
};