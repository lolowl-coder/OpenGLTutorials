#include "Render/Attribute.h"
#include "Render/RenderContext.h"

Attribute::Attribute(const GLint size, const GLenum dataType, const GLboolean normalize, const GLsizei stride, const GLuint offset)
: mSize(size)
, mDataType(dataType)
, mNormalize(normalize)
, mStride(stride)
, mOffset(offset)
{
}

void Attribute::enable(const GLuint index)
{
   glEnableVertexAttribArray(index);
   checkGLError("Enable vertex attrib array");
   glVertexAttribPointer(index, mSize, mDataType, mNormalize, mStride, (void*)mOffset);
   checkGLError("Set vertex attrib pointer");
}

void Attribute::disable(const GLuint index)
{
   glDisableVertexAttribArray(index);
   checkGLError("disable vertex attrib array");
}