#pragma once

#include "Render/Camera.h"
#include "Render/Buffer.h"
#include "Render/Shader.h"

class GridShader : public Shader
{
public:
   GridShader();

   void setMVP(const Matrix4f& mvp);
   void setMV(const Matrix4f& mv);
   void setDiffuse(const Vector4f& color);
   void setEye(const Vector3f& eye);

private:
   GLint mMVPLocation;
   GLint mMVLocation;
   GLint mDiffuseLocation;
   GLint mEyeLocation;
};

class NormalShader : public Shader
{
public:
   NormalShader();

   void setColor(const Vector4f& color);
   void setMVP(const Matrix4f& mvp);

private:
   GLint mColorLocation;
   GLint mMVPLocation;
};

class Grid
{
public:
   Grid();

   void init(const Vector2i& dimensions, const Vector2f& size);
   void deinit();
   void render(const Camera& camera);

private:
   bool genNormalHelper(const Vector3f& v, const Vector2i& rowCol, const Vector2i& normal0Offset, const Vector2i& normal1Offset, Vector3f& accumulatedNormal);
   Vector3f genNormal(const int row, const int col);
   Vector3f genVertex(const int row, const int col);
   void bindShader(const Camera& camera);

private:
   Vector2i mDimensions;
   GLuint mGridVBO;
   GLuint mGridIBO;
   GridShader mShader;
   NormalShader mNormalShader;
   Buffer mNormals;
   Vector2f mSize;
};