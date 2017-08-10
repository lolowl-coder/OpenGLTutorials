#pragma once

#include "Render/Shader.h"
#include "Render/Camera.h"
#include "Render/Material.h"
#include "Render/Light.h"

#include "vmath.h"

class ShaderADSS : public Shader
{
public:
   ShaderADSS();

   void load();
   void bind(const Camera& camera, const Matrix4f& translation, const Matrix4f& rotation, const Material& material, const Light& light);
   void setMVP(const Matrix4f& mvp);
   void setM(const Matrix4f& m);
   void setRotation(const Matrix4f& m);
   void setObjectMaterial(const Material& material);
   void setLight(const Light& light);
   void setEye(const Vector3f& eye);

private:
   GLint mMVPLocation;
   GLint mMLocation;
   GLint mRotationLocation;
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