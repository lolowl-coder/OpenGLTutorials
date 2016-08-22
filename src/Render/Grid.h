#pragma once

#include "Render/Buffer.h"
#include "Render/Shader.h"

class GridShader : public Shader
{
public:
   GridShader();

   void setMVP(const Matrix4f& mvp);
   void setColor(const Vector4f& color);
   void setEye(const Vector3f& eye);

private:
   GLint mMVPLocation;
   GLint mColorLocation;
   GLint mEyeLocation;
};

class NormalShader : public Shader
{
public:
   NormalShader();

   void setNormalLenght(const float length);
   void setNormalColor(const Vector4f& color);

private:
   GLint mNormalLengthLocation;
   GLint mNormalColorLocation;
};

class Grid
{
public:
   Grid();

   void init(const Vector2i& dimensions, const Vector2f& size);
   void deinit();
   void render(const Matrix4f& vp, const Vector3f& eye);

private:
   Vector3f genNormal(const int row, const int col);
   Vector3f genVertex(const int row, const int col);
   void bindShader(const Matrix4f& vp, const Vector3f& eye);

private:
   Vector2i mDimensions;
   GLuint mGridVBO;
   GLuint mGridIBO;
   GridShader mShader;
   NormalShader mNormalShader;
   Buffer mNormals;
   Vector2f mSize;
};