#pragma once

#include "Tests/Camera/ICameraTest.h"
#include "Render/Object/Ground.h"
#include "Render/Object/Cube.h"
#include "Render/FBO.h"
#include "Render/Shader/TexturedRectShader.h"
#include "Render/Texture.h"
#include "Render/Shader/ShaderADSS.h"
#include "Render/Shader/RectShader.h"

class ShadowTest : public ICameraTest
{
public:
   ShadowTest(Director& director, const std::string& name);

   virtual void init();
   virtual void deinit();
   virtual void run();
   virtual void onKeyboardEvent(KeyboardEventType eventType, int keyCode);

private:
   void update();
   void renderLight();
   void renderDepth();
   void setObjectUniforms(ShaderADSS& shader, const Object& object, const Camera& camera);
   void renderScene(const Camera& camera);

private:
   ShaderADSS mShaderADSSS;
   RectShader mLightShader;
   RectRenderer mRectRenderer;
   Ground mGround;
   Cube mCube;
   Vector2i mShadowBufferSize;
   FBO mShadowBuffer;
   TexturedRectShader mFboShader;
   Light mLight;
   bool mRenderShadowBuffer;
   Camera mLightCamera;
   bool mUpdate;
   float mTime;
};