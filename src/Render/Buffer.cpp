#include "Render/Buffer.h"
#include "Render/RenderContext.h"

Buffer::Buffer(const GLenum target, const GLuint divisor, int attributesOffset)
: mId(0)
, mTarget(target)
, mDivisor(divisor)
, mAttributesOffset(attributesOffset)
{

}

void Buffer::addAttribute(const Attribute& attribute)
{
   mAttributes.push_back(attribute);
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

   for(int i = 0; i < mAttributes.size(); i++)
   {
      mAttributes[i].enable(i + mAttributesOffset);
      if(mDivisor > 0)
      {
         glVertexAttribDivisor(i + mAttributesOffset, mDivisor);
         checkGLError("set attrib divisor");
      }
   }
}

void Buffer::disable()
{
   for(int i = 0; i < mAttributes.size(); i++)
   {
      mAttributes[i].disable(i + mAttributesOffset);
      if(mDivisor > 0)
      {
         glVertexAttribDivisor(i + mAttributesOffset, 0);
         checkGLError("reset attrib divisor");
      }
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