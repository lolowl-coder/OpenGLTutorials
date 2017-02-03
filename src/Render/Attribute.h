#pragma once

#include "Render/GLHeaders.h"

class Attribute
{
public:
   Attribute(const GLint size, const GLenum dataType, const GLboolean normalize, const GLsizei stride, const GLuint offset);
   void enable(const GLuint index);
   void disable(const GLuint index);

private:
   GLint mSize;
   GLenum mDataType;
   GLboolean mNormalize;
   GLsizei mStride;
   GLuint mOffset;
};