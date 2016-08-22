#pragma once

#include "Render/GLHeaders.h"
#include "vmath.h"

class Texture
{
public:
   Texture();
   ~Texture();

   void deinit();

   void bind();
   void unbind();

   void genChecker(const Vector3f& color0, const Vector3f& color1, const int size);
   void genSpot(const int radius, const Vector4f& color);
   void gen(unsigned char*& data, const Vector2i& size);

   void create();

   GLuint getId();
   unsigned char* getData();

private:
   void cleanup();

private:
   GLuint mId;
   unsigned char* mData;
   Vector2i mSize;
};