#pragma once

#include "Render/Camera.h"
#include "Render/Buffer.h"
#include "Render/Light.h"
#include "Render/Material.h"
#include "Render/Shader/Shader.h"

class Object
{
public:
   Object();

   virtual void init() = 0;
   virtual void deinit() = 0;
   virtual void render(const Camera& camera) = 0;
   
   void setTranslation(const Matrix4f& transform);
   Matrix4f getTranslation() const;
   void setRotation(const Matrix4f& rotation);
   Matrix4f getRotation() const;
   void setMaterial(const Material& material);
   Material getMaterial() const;
   void setLight(const Light& light);
   Light getLight() const;

protected:
   Material mMaterial;
   Light mLight;
   Matrix4f mTranslation;
   Matrix4f mRotation;
   GLuint mVBO;
   GLuint mIBO;
   Buffer mNormals;
};