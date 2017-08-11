#pragma once

#include "Render/Buffer.h"

class RectRenderer
{
public:
   RectRenderer();

   void init();
   void deinit();
   void render();

private:
   Buffer mBuffer;
};