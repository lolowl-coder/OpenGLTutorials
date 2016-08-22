#include "Render/RenderContext.h"
#include "Render/Texture.h"

#include <algorithm>

Texture::Texture()
: mId(0)
, mData(NULL)
{

}

Texture::~Texture()
{
   cleanup();
}

void Texture::cleanup()
{
   if(mData)
   {
      delete [] mData;
      mData = NULL;
   }

   mSize.x = 0;
   mSize.y = 0;
}

void Texture::genChecker(const Vector3f& color0, const Vector3f& color1, const int size)
{
#if 0
   int w = 1024;
   int h = 1024;
   const std::string fileName = "d:/temp/test2.pvr";
   std::ifstream ifs ( fileName.c_str(), std::ifstream::ate | std::ifstream::binary );
   int size = ifs.tellg ();
   ifs.seekg ( 0 );
   unsigned char* data = new unsigned char[size];
   ifs.read ( (char*)data, size );
   ifs.close();
   glCompressedTexImage2D ( GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, w, h, 0, size, data );
#else
   mSize.x = size;
   mSize.y = size;

   mData = new unsigned char[mSize.x * mSize.y * 4];

   int i, j, c;
   for (i = 0; i < mSize.y; i++)
   {
      for (j = 0; j < mSize.x; j++)
      {
         c = ((((i&0x8)==0)^((j&0x8))==0));
         Vector3f color(color0 + (color1 - color0) * (float)c * 255.0f);

         mData[(i * mSize.x + j) * 4 + 0] = (GLubyte)color.r;
         mData[(i * mSize.x + j) * 4 + 1] = (GLubyte)color.g;
         mData[(i * mSize.x + j) * 4 + 2] = (GLubyte)color.b;
         mData[(i * mSize.x + j) * 4 + 3] = 255;
      }
   }
#endif
}

void Texture::genSpot(const int radius, const Vector4f& color)
{
   bind();

   cleanup();

   mData = new unsigned char[radius * radius * 4];

   int i;
   int j;
   float halfRadius = radius * 0.5f;
   mSize.x = radius;
   mSize.y = radius;
   for (i = 0; i < radius; i++)
   {
      for (j = 0; j < radius; j++)
      {
         Vector2f center(halfRadius, halfRadius);
         Vector2f dist = Vector2f((float)j, (float)i) - center;
         float currentRadius = dist.length();
         float alpha = (std::max)(0.0f, (std::min)(1.0f, 1.0f - currentRadius / halfRadius));

         mData[(i * radius + j) * 4 + 0] = (GLubyte)(color.r * alpha * 255.0f);
         mData[(i * radius + j) * 4 + 1] = (GLubyte)(color.g * alpha * 255.0f);
         mData[(i * radius + j) * 4 + 2] = (GLubyte)(color.b * alpha * 255.0f);
         mData[(i * radius + j) * 4 + 3] = (GLubyte)(color.a * alpha * 255.0f);
      }
   }
}

void Texture::gen(unsigned char*& data, const Vector2i& size)
{
   mData = data;
   mSize = size;
}

void Texture::create()
{
   glActiveTexture(GL_TEXTURE0);

   glGenTextures(1, &mId);
   checkGLError("Gen texture");

   bind();
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mSize.x, mSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, mData);
   checkGLError("Create compressed texture object");

   cleanup();

   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void Texture::bind()
{
   glBindTexture(GL_TEXTURE_2D, mId);
   checkGLError("Bind texture");
}

void Texture::unbind()
{
   glBindTexture(GL_TEXTURE_2D, 0);
   checkGLError("Unbind texture");
}

void Texture::deinit()
{
   glDeleteTextures(1, &mId);
}

GLuint Texture::getId()
{
   return mId;
}

unsigned char* Texture::getData()
{
   return mData;
}