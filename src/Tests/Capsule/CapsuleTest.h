#pragma once

#include "Render/Camera.h"
#include "Render/Shader/CapsuleShader.h"
#include "Tests/Camera/CameraTest.h"

#include <vector>

class CapsuleTest : public CameraTest
{
public:
   typedef std::vector<Vector2f> tPolyLine;

   CapsuleTest(Director& director, const std::string& name);
   virtual void init();
   virtual void deinit();
   virtual void run();

private:
   void generateCapsule();
   void renderCapsule();

private:
   Buffer mCapsuleVertexBuffer;
   Buffer mCapsuleIndexBuffer;
   uint16_t mVerticesCount = 0u;
   uint16_t mIndicesCount = 0u;
   CapsuleShader mCapsuleShader;
};