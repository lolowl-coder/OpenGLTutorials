#pragma once

#include "Render/GLHeaders.h"
#include "Tests/Test.h"
#include "Render/Shader/TexturedShader.h"

class ComputeShaderTest : public Test
{
public:
   ComputeShaderTest(Director& director, const std::string& name);

   virtual void init();
   virtual void deinit();
   virtual void run();
   virtual void onTouchEvent(TouchEventType eventType, const Vector2f& position);

private:
   void setSpot(const Vector2f& position, const Vector2f& v);

private:
   GLuint quadVbo;
   GLuint tfdSSbo;
   GLuint velocitySSbo;
   GLuint positionSSbo;
   GLuint colorSSbo;
   Shader mComputeShader;
   TexturedShader mRenderShader;
   int mFrameId;
   GLint mFrameIdLocation;
   GLint mDimensionsLocation;
   GLint mOriginLocation;
   GLint mStepLocation;
   GLint mDtLocation;
   Vector2i mLastTouchPosition;
};