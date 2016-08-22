#pragma once

#include "Render/GLHeaders.h"

class Buffer
{
public:
   Buffer(GLenum target, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint divisor);
   void setData(unsigned int sizeInBytes, void* data);
   void bindShaderStorage();
   void bindArrayBuffer();
   void bind();
   void enable();
   void disable();
   void deinit();
   GLuint getId();
   GLuint getIndex();

private:
   GLuint mId;
   GLenum mTarget;
   GLuint mIndex;
   GLint mSize;
   GLenum mType;
   GLboolean mNormalized;
   GLsizei mStride;
   GLuint mDivisor;
};