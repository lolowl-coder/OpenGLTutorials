#pragma once

#include "Render/Camera.h"
#include "Tests/LineOptimization/PolyLineShader.h"
#include "Tests/Test.h"

#include <vector>

enum EShapeType
{
   SHAPE0,
   SHAPE1,
   SHAPE2,
   SHAPE3,
   SHAPE_COUNT
};

enum EOptimizationKind
{
   OPT_DISTANCE,
   OPT_ANGLE,
   OPT_COUNT
};

class LineOptimizationTest : public Test
{
public:
   typedef std::vector<Vector2f> tPolyLine;

   LineOptimizationTest(Director& director, const std::string& name);
   virtual void init();
   virtual void deinit();
   virtual void run();
   virtual void onKeyboardEvent(KeyboardEventType eventType, int keyCode) override;

private:
   void updateLines();
   tPolyLine createRandomPolyLine();
   void createPolyLineVBO(GLuint& polyLineVBO, const tPolyLine& polyLine);
   tPolyLine optimizePolyLine(const tPolyLine& polyLine);
   void optimizeByDistance(const tPolyLine& polyLine, tPolyLine& result, const float distanceThreshold);
   void optimizeByAngle(const tPolyLine& polyLine, tPolyLine& result, const float angleThreshold);
   void renderPolyLine(const GLuint polyLineVBO, const Vector4f& color, int count, float pointSize);

private:
   GLuint mPolyLineVBO = 0;
   GLuint mPolyLineOptimizedVBO = 0;
   tPolyLine mPolyLine;
   tPolyLine mPolyLineOptimized;
   PolyLineShader mPolyLineShader;
   int mOptimizationKind = OPT_DISTANCE;
   float mDistanceThreshold = 0.2f;
   float mAngleThreshold = 0.8f;
   int mShape = SHAPE0;
};