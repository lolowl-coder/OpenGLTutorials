#pragma once

#include "Render/Object.h"
#include "Render/ShaderADSS.h"

class Cube : public Object
{
public:
   Cube();

   virtual void init();
   virtual void deinit();
   virtual void render(const Camera& camera);

private:
   Buffer mVertices;
   ShaderADSS mShader;
};