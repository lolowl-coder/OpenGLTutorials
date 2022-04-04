#pragma once

#include "Render/Camera.h"
#include "Tests/LineOptimization/PolyLineShader.h"
#include "Tests/Test.h"

#include <vector>

class LineOptimizationTest : public Test
{
public:
   typedef std::vector<Vector2f> tPolyLine;

   LineOptimizationTest(Director& director, const std::string& name);
   virtual void init();
   virtual void deinit();
   virtual void run();

private:
   tPolyLine createRandomPolyLine();
   void createPolyLineVBO(GLuint& polyLineVBO, const tPolyLine& polyLine);
   tPolyLine optimizePolyLine(const tPolyLine& polyLine, float threshold);
   void renderPolyLine(const GLuint polyLineVBO, const Vector4f& color, int count, float pointSize);

private:
   GLuint mPolyLineVBO;
   GLuint mPolyLineOptimizedVBO;
   tPolyLine mPolyLine;
   tPolyLine mPolyLineOptimized;
   PolyLineShader mPolyLineShader;
};