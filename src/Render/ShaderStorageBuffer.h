#pragma once

#include "Render/Buffer.h"

class ShaderStorageBuffer : public Buffer
{
public:
   ShaderStorageBuffer(const GLuint index, const GLuint divisor, const int attributesOffset);

   void bindShaderStorage();

private:
   GLuint mIndex;
};