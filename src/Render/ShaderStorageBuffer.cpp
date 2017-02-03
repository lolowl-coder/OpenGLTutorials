#include "Render/ShaderStorageBuffer.h"
#include "Render/RenderContext.h"

ShaderStorageBuffer::ShaderStorageBuffer(const GLuint index, const GLuint divisor, const int attributesOffset)
: Buffer(GL_SHADER_STORAGE_BUFFER, divisor, attributesOffset)
, mIndex(index)
{
}

void ShaderStorageBuffer::bindShaderStorage()
{
   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mIndex, mId);
   checkGLError("bind shader storage buffer");
}