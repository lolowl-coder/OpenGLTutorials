#include "Platform/Log.h"
#include "Render/RenderContext.h"
#include "Render/FBO.h"

#include <vector>

FBO::FBO()
: mId(0)
, mColorTexture(0)
, mDepthTexture(0)
{

}

GLuint FBO::attachTexture(bool depth, GLenum attachment, const Vector2i& size)
{
   GLuint texture = 0;

   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);

   GLint internalFormat = GL_RGBA;
   GLenum format = GL_RGBA;
   GLenum type = GL_UNSIGNED_BYTE;
   if(depth)
   {
      internalFormat = GL_DEPTH_COMPONENT24;
      format = GL_DEPTH_COMPONENT;
      type = GL_FLOAT;
   }
   
   glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, 0);
   checkGLError("FBO::attachTexture, can't create fbo texture");
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   checkGLError("FBO::attachTexture, can't set texture parameters");

   glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
   checkGLError("FBO::attachTexture, can't attach fbo texture");

   return texture;
}

bool FBO::init(const Vector2i& size, bool depth)
{
   mId = 0;
   glGenFramebuffers(1, &mId);
   bind();

   checkGLError("FBO::init, bind fbo");

   std::vector<GLenum> drawBuffers;
   drawBuffers.push_back(GL_COLOR_ATTACHMENT0);
   mColorTexture = attachTexture(false, drawBuffers.back(), size);

   if(depth)
   {
      drawBuffers.push_back(GL_DEPTH_ATTACHMENT);
      mDepthTexture = attachTexture(true, drawBuffers.back(), size);
   }
   
   glDrawBuffers(1, &drawBuffers[0]);

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
   glDeleteTextures(1, &mColorTexture);
   unbind();
   glDeleteFramebuffers(1, &mId);
}

void FBO::bind()
{
   glBindFramebuffer(GL_FRAMEBUFFER, mId);
}

void FBO::bindColorTexture()
{
   glBindTexture(GL_TEXTURE_2D, mColorTexture);
}

void FBO::bindDepthTexture()
{
   glBindTexture(GL_TEXTURE_2D, mDepthTexture);
}

void FBO::unbind()
{
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::clear()
{
   GLenum mask = GL_COLOR_BUFFER_BIT;
   if(mDepthTexture != 0)
   {
      mask |= GL_DEPTH_BUFFER_BIT;
   }

   glClear(mask);
}