#pragma once

#include "Render/Camera.h"
#include "Render/Buffer.h"
#include "Render/Light.h"
#include "Render/Material.h"
#include "Render/Object.h"
#include "Render/Shader.h"

class GridShader : public Shader
{
public:
   GridShader();

   void setMVP(const Matrix4f& mvp);
   void setM(const Matrix4f& m);
   void setObjectMaterial(const Material& material);
   void setLight(const Light& light);
   void setEye(const Vector3f& eye);

private:
   GLint mMVPLocation;
   GLint mMLocation;
   GLint mEyeLocation;
   GLint mMaterialAmbientLocation;
   GLint mMaterialDiffuseLocation;
   GLint mMaterialSpecularLocation;
   GLint mMaterialShininessLocation;
   GLint mLightAmbientLocation;
   GLint mLightDiffuseLocation;
   GLint mLightSpecularLocation;
   GLint mLightAttenuationLocation;
   GLint mLightPositionLocation;
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

class Grid : public Object
{
public:
   Grid();

   virtual void init();
   virtual void deinit();
   virtual void render(const Camera& camera);

   void setDimensions(const Vector2i& dimensions);
   void setSize(const Vector2f& size);

private:
   bool genNormalHelper(const Vector3f& v, const Vector2i& rowCol, const Vector2i& normal0Offset, const Vector2i& normal1Offset, Vector3f& accumulatedNormal);
   Vector3f genNormal(const int row, const int col);
   Vector3f genVertex(const int row, const int col);
   void bindShader(const Camera& camera);

private:
   Vector2i mDimensions;
   GridShader mShader;
   NormalShader mNormalShader;
   Vector2f mSize;
};