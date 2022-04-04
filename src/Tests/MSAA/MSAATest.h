#pragma once

#include "Tests/Test.h"
#include "Render/FBO.h"

class MSAATest : public Test
{
public:
   MSAATest(Director& director, const std::string& name);

   virtual void init();
   virtual void deinit();
   virtual void run();
};