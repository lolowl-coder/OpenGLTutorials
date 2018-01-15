#pragma once

#include "Render/Shader/Shader.h"
#include "Render/Camera.h"
#include "Render/Material.h"
#include "Render/Light.h"

#include "vmath.h"

class ShaderADSS : public Shader
{
public:
   ShaderADSS();

   void load();
   void setM(const Matrix4f& m);
   void setV(const Matrix4f& v);
   void setP(const Matrix4f& p);
   void setObjectMaterial(const Material& material);
   void setLight(const Light& light);
   void setLightSpaceMatrix(const Matrix4f& lightSpaceMatrix);
   void setEye(const Vector3f& eye);
   void setLightDir(const Vector3f& lightDir);

private:
   GLint mMLocation;
   GLint mVLocation;
   GLint mPLocation;
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
   GLint mLightSpaceMatrixLocation;
   GLint mLightDirLocation;
};