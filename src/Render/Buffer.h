#pragma once

#include "Render/GLHeaders.h"
#include "Render/Attribute.h"

#include <vector>

class Buffer
{
public:
   Buffer(const GLenum target, const GLuint divisor, const int attributesOffset);
   void addAttribute(const Attribute& attribute);
   void setData(unsigned int sizeInBytes, void* data);
   void bindArrayBuffer();
   void bind();
   void enable();
   void disable();
   void deinit();
   GLuint getId();

protected:
   GLuint mId;
   int mAttributesOffset;

private:
   std::vector<Attribute> mAttributes;
   GLenum mTarget;
   GLuint mDivisor;
};