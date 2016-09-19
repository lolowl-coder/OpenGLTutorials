#include "Platform/Log.h"
#include "Render/RenderContext.h"
#include "Render/FBO.h"

FBO::FBO()
: mId(0)
, mRT(0)
{

}

bool FBO::init(const Vector2i& size)
{
   mId = 0;
   glGenFramebuffers(1, &mId);
   bind();

   checkGLError("FBO::init, bind fbo");

   glGenTextures(1, &mRT);
   glBindTexture(GL_TEXTURE_2D, mRT);
   glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, size.x, size.y, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   
   checkGLError("FBO::init, create rt");

   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRT, 0);

   GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
   glDrawBuffers(1, drawBuffers);

   checkGLError("FBO::init, set draw buffers");

   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
   {
      logMsg("Can't init FBO");

      return false;
   }

   return true;
}

void FBO::deinit()
{
   glDeleteTextures(1, &mRT);
   unbind();
   glDeleteFramebuffers(1, &mId);
}

void FBO::bind()
{
   glBindFramebuffer(GL_FRAMEBUFFER, mId);
}

void FBO::bindTexture()
{
   glBindTexture(GL_TEXTURE_2D, mRT);
}

void FBO::unbind()
{
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::clear()
{
   glClear(GL_COLOR_BUFFER_BIT);
}