#include "Render/Buffer.h"
#include "Render/RenderContext.h"

Buffer::Buffer(GLenum target, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint divisor)
: mId(0)
, mTarget(target)
, mIndex(index)
, mSize(size)
, mType(type)
, mNormalized(normalized)
, mStride(stride)
, mDivisor(divisor)
{

}

void Buffer::setData(unsigned int sizeInBytes, void* data)
{
   if(mId == 0)
   {
      glGenBuffers(1, &mId);
   }
   bind();
   glBufferData(mTarget, sizeInBytes, data, GL_STATIC_DRAW);
   checkGLError("send buffer to gpu");
}

void Buffer::bindShaderStorage()
{
   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mIndex, mId);
   checkGLError("bind shader storage buffer");
}

void Buffer::bindArrayBuffer()
{
   glBindBuffer(GL_ARRAY_BUFFER, mId);
   checkGLError("bind array buffer");
}

void Buffer::bind()
{
   glBindBuffer(mTarget, mId);
   checkGLError("bind buffer");
}

void Buffer::enable()
{
   bindArrayBuffer();
   glEnableVertexAttribArray(mIndex);
   glVertexAttribPointer(mIndex, mSize, mType, mNormalized, mStride, (void *)0);
   checkGLError("set buffer pointer");
   if(mDivisor > 0)
   {
      glVertexAttribDivisor(mIndex, mDivisor);
      checkGLError("set attrib divisor");
   }
}

void Buffer::disable()
{
   glDisableVertexAttribArray(mIndex);
   checkGLError("disable vertex attrib array");
   if(mDivisor > 0)
   {
      glVertexAttribDivisor(mIndex, 0);
      checkGLError("reset attrib divisor");
   }
}

void Buffer::deinit()
{
   glDeleteBuffers(1, &mId);
}

GLuint Buffer::getId()
{
   return mId;
}

GLuint Buffer::getIndex()
{
   return mIndex;
}