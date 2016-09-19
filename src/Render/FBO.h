#pragma once

#include "Render/GLHeaders.h"
#include "vmath.h"

class FBO
{
public:
   FBO();

   bool init(const Vector2i& size);
   void deinit();
   void bind();
   void bindTexture();
   void unbind();
   void clear();

private:
   GLuint mId;
   GLuint mRT;
};