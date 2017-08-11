#pragma once

#include "Render/Object/Object.h"
#include "Render/Shader/ShaderADSS.h"

class Cube : public Object
{
public:
   Cube();

   virtual void init();
   virtual void deinit();
   virtual void render(const Camera& camera);

private:
   Buffer mVertices;
};