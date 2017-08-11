#include "Render/Shader/ShaderADSS.h"

ShaderADSS::ShaderADSS()
: mMLocation(-1)
, mVLocation(-1)
, mPLocation(-1)
, mEyeLocation(-1)
, mMaterialAmbientLocation(-1)
, mMaterialDiffuseLocation(-1)
, mMaterialSpecularLocation(-1)
, mMaterialShininessLocation(-1)
, mLightAmbientLocation(-1)
, mLightDiffuseLocation(-1)
, mLightSpecularLocation(-1)
, mLightAttenuationLocation(-1)
, mLightPositionLocation(-1)
{
}

void ShaderADSS::load()
{
   Shader::load("Data/Shader/Lighting/ADSSS.vs", "Data/Shader/Lighting/ADSSS.fs");
}

void ShaderADSS::setM(const Matrix4f& m)
{
   if(mMLocation == -1)
      mMLocation = uniformLocation("uM");

   uniformMatrix4(mMLocation, 1, &m);
}

void ShaderADSS::setV(const Matrix4f& v)
{
   if(mVLocation == -1)
      mVLocation = uniformLocation("uV");

   uniformMatrix4(mVLocation, 1, &v);
}

void ShaderADSS::setP(const Matrix4f& p)
{
   if(mPLocation == -1)
      mPLocation = uniformLocation("uP");

   uniformMatrix4(mPLocation, 1, &p);
}

void ShaderADSS::setObjectMaterial(const Material& material)
{
   if(mMaterialAmbientLocation == -1)
      mMaterialAmbientLocation = uniformLocation("objectMaterial.ambient");
   uniform4(mMaterialAmbientLocation, 1, &material.mAmbient);
   if(mMaterialDiffuseLocation == -1)
      mMaterialDiffuseLocation = uniformLocation("objectMaterial.diffuse");
   uniform4(mMaterialDiffuseLocation, 1, &material.mDiffuse);
   if(mMaterialSpecularLocation == -1)
      mMaterialSpecularLocation = uniformLocation("objectMaterial.specular");
   uniform4(mMaterialSpecularLocation, 1, &material.mSpecular);
   if(mMaterialShininessLocation == -1)
      mMaterialShininessLocation = uniformLocation("objectMaterial.shininess");
   uniform1(mMaterialShininessLocation, 1, &material.mShininess);
}

void ShaderADSS::setLight(const Light& light)
{
   if(mLightPositionLocation == -1)
      mLightPositionLocation = uniformLocation("light.position");
   uniform3(mLightPositionLocation, 1, &light.mPosition);
   if(mLightAmbientLocation == -1)
      mLightAmbientLocation = uniformLocation("light.ambient");
   uniform4(mLightAmbientLocation, 1, &light.mAmbient);
   if(mLightDiffuseLocation == -1)
      mLightDiffuseLocation = uniformLocation("light.diffuse");
   uniform4(mLightDiffuseLocation, 1, &light.mDiffuse);
   if(mLightSpecularLocation == -1)
      mLightSpecularLocation = uniformLocation("light.specular");
   uniform4(mLightSpecularLocation, 1, &light.mSpecular);
   if(mLightAttenuationLocation == -1)
      mLightAttenuationLocation = uniformLocation("light.attenuation");
   uniform1(mLightAttenuationLocation, 1, &light.mAttenuation);
}

void ShaderADSS::setEye(const Vector3f& eye)
{
   if(mEyeLocation == -1)
      mEyeLocation = uniformLocation("uEye");

   uniform3(mEyeLocation, 1, &eye);
}