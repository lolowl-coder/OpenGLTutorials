#pragma once

#include "Render/Camera.h"
#include "Render/Buffer.h"
#include "Render/Light.h"
#include "Render/Material.h"
#include "Render/Shader.h"

class Object
{
public:
   Object();

   virtual void init() = 0;
   virtual void deinit() = 0;
   virtual void render(const Camera& camera) = 0;
   
   void setTransform(const Matrix4f& transform);
   void setMaterial(const Material& material);
   void setLight(const Light& light);

protected:
   Material mMaterial;
   Light mLight;
   Matrix4f mTransform;
   GLuint mVBO;
   GLuint mIBO;
   Buffer mNormals;
};