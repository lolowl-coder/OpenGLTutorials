#pragma once

#include "Tests/Camera/ICameraTest.h"
#include "Render/Object/Ground.h"
#include "Render/Object/Cube.h"
#include "Render/FBO.h"
#include "Render/Shader/TexturedRectShader.h"
#include "Render/Texture.h"
#include "Render/Shader/ShaderADSS.h"

class ShadowTest : public ICameraTest
{
public:
   ShadowTest(Director& director, const std::string& name);

   virtual void init();
   virtual void deinit();
   virtual void run();

public:
   virtual void onKeyboardEvent(KeyboardEventType eventType, int keyCode);

private:
   void renderDepth();
   void setObjectUniforms(ShaderADSS& shader, const Object& object, const Camera& camera);
   void renderScene(const Camera& camera);

private:
   ShaderADSS mShaderADSSS;
   Ground mGround;
   Cube mCube;
   FBO mShadowBuffer;
   TexturedRectShader mFboShader;
   Light mLight;
   bool mRenderShadowBuffer;
   Camera mLightCamera;
};