#pragma once

#include "Render/Camera.h"
#include "Render/Buffer.h"
#include "Render/Light.h"
#include "Render/Material.h"
#include "Render/Object/Object.h"
#include "Render/Shader/Shader.h"


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

class Grid : public Object
{
public:
   Grid();

   virtual void init();
   virtual void deinit();
   virtual void render(const Camera& camera);

   void setDimensions(const Vector2i& dimensions);
   void setSize(const Vector2f& size);

protected:
   virtual Vector3f genVertex(const int row, const int col);

private:
   bool genNormalHelper(const Vector3f& v, const Vector2i& rowCol, const Vector2i& normal0Offset, const Vector2i& normal1Offset, Vector3f& accumulatedNormal);
   Vector3f genNormal(const int row, const int col);
   void bindShader(const Camera& camera);

protected:
   Vector2i mDimensions;
   Vector2f mSize;

private:
   NormalShader mNormalShader;
   Buffer mEdges;
};