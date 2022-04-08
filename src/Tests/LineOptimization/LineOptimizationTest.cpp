#include "Render/GLHeaders.h"
#include "Render/RenderContext.h"
#include "Tests/LineOptimization/LineOptimizationTest.h"
#include "Platform/Log.h"

#include <vector>

LineOptimizationTest::LineOptimizationTest(Director& director, const std::string& name)
   : Test(director, name)
{

}

LineOptimizationTest::tPolyLine LineOptimizationTest::createRandomPolyLine()
{
   tPolyLine result;
   switch(mShape)
   {
      case SHAPE0:
      {
         int count = 50;
         for(int i = 0; i < count; i++)
         {
            /*if(i == 25)
            {
               result.push_back(Vector2f(i / float(count) * 0.8f + 0.1f, 1.0f));
            }
            else*/
            {
               result.push_back(Vector2f(i / float(count) * 0.8f + 0.1f, sin(i / 5.0f) * 0.5f + 0.5f));
            }
         }
      }
      break;
      case SHAPE1:
      {
         result.push_back(Vector2f(0.1f, 0.1f));
         result.push_back(Vector2f(0.5f, 0.5f));
         result.push_back(Vector2f(0.9f, 0.1f));
      }
      break;
      case SHAPE2:
      {
         result.push_back(Vector2f(0.1f, 0.1f));
         result.push_back(Vector2f(0.9f, 0.1f));
      }
      break;
      case SHAPE3:
      {
         result.push_back(Vector2f(0.1f, 0.1f));
         result.push_back(Vector2f(0.2f, 0.1f));
         result.push_back(Vector2f(0.3f, 0.1f));
         result.push_back(Vector2f(0.4f, 0.7f));
         result.push_back(Vector2f(0.6f, 0.2f));
         result.push_back(Vector2f(0.1f, 0.5f));
         result.push_back(Vector2f(0.9f, 0.9f));
         result.push_back(Vector2f(0.1f, 0.9f));
      }
      break;
   }

   return result;
}

void LineOptimizationTest::createPolyLineVBO(GLuint& polyLineVBO, const tPolyLine& polyLine)
{
   if(polyLineVBO == 0)
   {
      glGenBuffers(1, &polyLineVBO);
      checkGLError("Gen poly line vbo");
   }
   glBindBuffer(GL_ARRAY_BUFFER, polyLineVBO);
   checkGLError("Bind poly line vbo");
   glBufferData(GL_ARRAY_BUFFER, polyLine.size() * sizeof(Vector2f), &polyLine[0], GL_STATIC_DRAW);
   checkGLError("Pass poly line vertices to GPU");
}

void LineOptimizationTest::updateLines()
{
   //poly lines
   mPolyLine = createRandomPolyLine();
   mPolyLineOptimized = optimizePolyLine(mPolyLine, mThreshold);

   //vbo
   createPolyLineVBO(mPolyLineVBO, mPolyLine);
   createPolyLineVBO(mPolyLineOptimizedVBO, mPolyLineOptimized);
}

void LineOptimizationTest::init()
{
   //shader
   mPolyLineShader.load("Data/Shader/PolyLine/polyLine.vs", "Data/Shader/PolyLine/polyLine.fs");
   mPolyLineShader.bind();

   updateLines();

   //states
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);
}

void LineOptimizationTest::deinit()
{
   glDeleteBuffers(1, &mPolyLineVBO);
   glDeleteBuffers(1, &mPolyLineOptimizedVBO);
   mPolyLineShader.deinit();

   checkGLError("Delete GL objects");
}

LineOptimizationTest::tPolyLine LineOptimizationTest::optimizePolyLine(const tPolyLine& polyLine, const float threshold)
{
   tPolyLine result;

   auto linePointCount = polyLine.size();
   if(linePointCount > 1)
   {
      if(linePointCount == 2u)
      {
         result.push_back(polyLine[0]);
         result.push_back(polyLine[1]);
      }
      else
      {
         float prevLength = 0.0f;
         for(auto pointIndex = 0u; pointIndex < linePointCount - 1; ++pointIndex)
         {
            const auto p0 = polyLine[pointIndex];
            if(pointIndex == 0u)
            {
               result.push_back(p0);
            }
            else
            {
               const auto p1 = polyLine[pointIndex + 1];
               auto v = p1 - p0;
               auto l = v.length();

               if(l > threshold)
               {
                  result.push_back(p0);
                  prevLength = 0.0;
               }
               else
               {
                  if(prevLength + l > threshold)
                  {
                     auto t = threshold - prevLength;
                     auto vn = v;
                     vn.normalize();
                     auto newPoint = p0 + vn * t;
                     //auto newPoint = (p0 + p1) * 0.5f;
                     result.push_back(newPoint);
                     prevLength = l - t;
                  }
                  else
                  {
                     prevLength += l;
                  }

                  /*if(prevLength > threshold)
                  {
                     prevLength = threshold;
                  }*/
               }

               if(pointIndex == linePointCount - 2)
               {
                  result.push_back(p1);
               }
            }
         }
      }
   }

   logMsg("Points count reduced by %.1f%%", (1.0f - float(result.size()) / float(polyLine.size())) * 100.0f);

   return result;
}

void LineOptimizationTest::renderPolyLine(const GLuint polyLineVBO, const Vector4f& color, int count, float pointSize)
{
   mPolyLineShader.setColor(color);
   mPolyLineShader.setPointSize(pointSize);

   glBindBuffer(GL_ARRAY_BUFFER, polyLineVBO);
   checkGLError("Bind poly line vbo");
   
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   checkGLError("Unbind index vbo");

   glEnableVertexAttribArray(0);
   checkGLError("Enable vertex attrib array");
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), 0);
   checkGLError("Set vertex attrib pointer");

   glLineWidth(3);
   glDrawArrays(GL_LINE_STRIP, 0, count);
   glDrawArrays(GL_POINTS, 0, count);
}

void LineOptimizationTest::run()
{
   renderPolyLine(mPolyLineVBO, Vector4f(1.0f, 0.0f, 0.0f, 1.0f), mPolyLine.size(), 20.0f);
   renderPolyLine(mPolyLineOptimizedVBO, Vector4f(0.0f, 1.0f, 0.0f, 1.0f), mPolyLineOptimized.size(), 10.0f);
}

void LineOptimizationTest::onKeyboardEvent(KeyboardEventType eventType, int keyCode)
{
   if(eventType == KE_DOWN)
   {
      switch(keyCode)
      {
      case VK_LEFT:
         mThreshold -= 0.01f;
         updateLines();
         break;
      case VK_RIGHT:
         mThreshold += 0.01f;
         updateLines();
         break;
      case 'S':
         mShape = (mShape + 1) % SHAPE_COUNT;
         updateLines();
         break;
      case 's':
         mShape = (mShape - 1) % SHAPE_COUNT;
         updateLines();
         break;
      }
   }
}