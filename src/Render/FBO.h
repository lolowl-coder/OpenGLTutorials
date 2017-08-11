#pragma once

#include "Render/GLHeaders.h"
#include "vmath.h"

class FBO
{
public:
   FBO();

   bool init(const Vector2i& size, bool depth);
   void deinit();
   void bind();
   void bindColorTexture();
   void bindDepthTexture();
   static void unbind();
   void clear();

private:
   GLuint attachTexture(bool depth, GLenum attachment, const Vector2i& size);

private:
   GLuint mId;
   GLuint mColorTexture;
   GLuint mDepthTexture;
};