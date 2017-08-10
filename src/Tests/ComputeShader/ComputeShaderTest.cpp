#define NOMINMAX

#include "Platform/Log.h"
#include "Render/RenderContext.h"
#include "Tests/ComputeShader/ComputeShaderTest.h"
#include <cstdlib>
#include <algorithm>

#define NUM_PARTICLES_X 512
#define NUM_PARTICLES_Y 512
#define NUM_PARTICLES NUM_PARTICLES_X * NUM_PARTICLES_Y
#define WORK_GROUP_SIZE 64 // # work-items per work-group
#define SCALE 2.0f
#define ORIGIN Vector3f(-SCALE * 0.5f, -SCALE * 0.5f, 0.0f)
#define MAX_VELOCITY 0.0f
#define SPOT_SIZE 30
#define HALF_SPOT_SIZE SPOT_SIZE * 0.5f
//#define ORIGIN Vector3f()

float Ranf(float mn, float mx)
{
   return (float)(rand() / (double)RAND_MAX) * (mx - mn) + mn;
}

static Vector3f vertices[] = {
   Vector3f ( -0.5f, -0.5f, 0.0f ),
   Vector3f ( -0.5f,  0.5f, 0.0f ),
   Vector3f (  0.5f, -0.5f, 0.0f ),
   Vector3f (  0.5f,  0.5f, 0.0f )
};

ComputeShaderTest::ComputeShaderTest(const Director& director, const std::string& name)
: Test(director, name)
, mFrameId(0)
, mFrameIdLocation(-1)
, mDimensionsLocation(-1)
, mOriginLocation(-1)
, mStepLocation(-1)
, mDtLocation(-1)
{
}

void ComputeShaderTest::init()
{
   glGenBuffers(1, &quadVbo);
   glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
   glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vector3f), (const void*)&vertices[0], GL_STATIC_DRAW);

   glGenBuffers(1, &positionSSbo);
   glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionSSbo);
   glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(Vector4f), NULL, GL_STATIC_DRAW);
   //the invalidate makes a big difference when re-writing
   GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
   Vector4f* position = (Vector4f*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(Vector4f), bufMask);
   for(int y = 0; y < NUM_PARTICLES_Y; y++)
      for(int x = 0; x < NUM_PARTICLES_X; x++)
   {
      Vector4f& p = position[y * NUM_PARTICLES_X + x];
      p.x = x / (float)NUM_PARTICLES_X * SCALE;
      p.y = y / (float)NUM_PARTICLES_Y * SCALE;
      p.z = 0.0f;
   }
   glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

   glGenBuffers(1, &colorSSbo);
   glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorSSbo);

   glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(Vector4f), NULL, GL_STATIC_DRAW);

   // need to do the following for both position, velocity, and colors of the particles:
   glGenBuffers(1, &tfdSSbo);
   glBindBuffer(GL_SHADER_STORAGE_BUFFER, tfdSSbo);
   glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(Vector4f), NULL, GL_STATIC_DRAW);
   Vector4f *tfd = (Vector4f*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(Vector4f), bufMask);
   for(int i = 0; i < NUM_PARTICLES; i++)
   {
      int row = i / NUM_PARTICLES_Y;
      int col = i - row * NUM_PARTICLES_Y;
      /*if(row > 30 && row < 60 && col > 30 && col < 60)
      {
         tfd[i].x = Ranf(100.0f, 5000.0f);
         tfd[i].y = Ranf(0.0f, 100.0f);
         tfd[i].z = Ranf(0.0f, 5.0f);
      }*/
      tfd[i].x = 0.0f;
      tfd[i].y = 0.0f;
      tfd[i].z = 0.0f;
      tfd[i].w = 0.0f;
   }
   glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

   glGenBuffers(1, &velocitySSbo);
   glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocitySSbo);
   glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(Vector4f), NULL, GL_STATIC_DRAW);
   Vector4f* velocity = (Vector4f*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(Vector4f), bufMask);
   for(int i = 0; i < NUM_PARTICLES; i++)
   {
      int row = i / NUM_PARTICLES_Y;
      int col = i - row * NUM_PARTICLES_Y;
      //if(row > 30 && row < 60 && col > 30 && col < 60)
      {
         velocity[i].x = 0.0f;
         velocity[i].y = 0.0f;
         velocity[i].z = 0.0f;
         velocity[i].w = 0.0f;
      }
   }
   glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

   checkGLError("init particles");

   mComputeShader.load("Data/Shader/ComputeShader/compute.cs", GL_COMPUTE_SHADER);
   mFrameIdLocation = mComputeShader.uniformLocation("u_frameId");
   mStepLocation = mComputeShader.uniformLocation("u_step");
   mDtLocation = mComputeShader.uniformLocation("u_dt");
   mDimensionsLocation = mComputeShader.uniformLocation("u_dimensions");
   mRenderShader.load("Data/Shader/ComputeShader/compute.vs", "Data/Shader/Common/colored.fs");
   mOriginLocation = mRenderShader.uniformLocation("u_origin");

   glDisable(GL_CULL_FACE);
   glDisable(GL_DEPTH_TEST);
}

void ComputeShaderTest::deinit()
{
   mComputeShader.deinit();
   mRenderShader.deinit();

   glDeleteBuffers(1, &quadVbo);
   glDeleteBuffers(1, &tfdSSbo);
   glDeleteBuffers(1, &velocitySSbo);
   glDeleteBuffers(1, &positionSSbo);
   glDeleteBuffers(1, &colorSSbo);
}

void ComputeShaderTest::run()
{
   //Invoking the Compute Shader in Your C Program
   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, positionSSbo);
   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, colorSSbo);
   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, tfdSSbo);
   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, velocitySSbo);

   mComputeShader.bind();
   mComputeShader.uniform1i(mFrameIdLocation, 1, &mFrameId);
   Vector2f dimensions(NUM_PARTICLES_X, NUM_PARTICLES_Y);
   mComputeShader.uniform2(mDimensionsLocation, 1, &dimensions);
   mComputeShader.uniform1i(mFrameIdLocation, 1, &mFrameId);
   Vector3f step(SCALE / NUM_PARTICLES_X, SCALE / NUM_PARTICLES_Y, 1.0f);
   mComputeShader.uniform3(mStepLocation, 1, &step);
   float dt = mDirector.getTimeDelta();
   mComputeShader.uniform1(mDtLocation, 1, &dt);
   mFrameId++;
   glDispatchCompute(NUM_PARTICLES / WORK_GROUP_SIZE, 1, 1);
   glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

   mRenderShader.bind();
   mRenderShader.uniform3(mOriginLocation, 1, &ORIGIN);
   glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void *)0);

   glBindBuffer(GL_ARRAY_BUFFER, positionSSbo);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4f), (void *)0);
   glVertexAttribDivisor(1, 1);
   
   glBindBuffer(GL_ARRAY_BUFFER, colorSSbo);
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4f), (void *)0);
   glVertexAttribDivisor(2, 1);

   glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, NUM_PARTICLES);

   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
   glVertexAttribDivisor(1, 0);
   glDisableVertexAttribArray(2);
   glVertexAttribDivisor(2, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   checkGLError("render particles");
}

void ComputeShaderTest::setSpot(const Vector2f& position, const Vector2f& v)
{
   Vector2f viewSize = mDirector.getViewSize();
   int y = (int)(position.y / viewSize.x * NUM_PARTICLES_Y);
   int x = (int)(position.x / viewSize.y * NUM_PARTICLES_X);

   GLint bufMask = GL_MAP_WRITE_BIT/* | GL_MAP_INVALIDATE_BUFFER_BIT*/;

   glBindBuffer(GL_SHADER_STORAGE_BUFFER, tfdSSbo);
   Vector4f *tfd = (Vector4f*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(Vector4f), bufMask);
   const Vector4f currentTfd(/*Ranf(12000.0f, 12000.0f)*/0.0f, 0.0f, 0.0f, 0.0f);

   //for(int i = 0; i < NUM_PARTICLES; i++)
   for(int yy = 0; yy < SPOT_SIZE; yy++)
      for(int xx = 0; xx < SPOT_SIZE; xx++)
      {
         float r = Vector2f(xx - HALF_SPOT_SIZE, yy - HALF_SPOT_SIZE).length();
         if(r < HALF_SPOT_SIZE)
         {
            int index = (yy + y - int(HALF_SPOT_SIZE)) * NUM_PARTICLES_X + xx + x - int(HALF_SPOT_SIZE);
            if(index > 0 && index < NUM_PARTICLES - 1)
               tfd[index] = currentTfd;
         }
      }
   glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

   glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocitySSbo);
   Vector4f* velocity = (Vector4f*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(Vector4f), bufMask);
   //const Vector4f currentV(Ranf(-MAX_VELOCITY, MAX_VELOCITY), Ranf(-MAX_VELOCITY, MAX_VELOCITY), 0.0f, 0.0f);
#if 0
   velocity[i].x = Ranf(-0.01f, 0.01f);
   velocity[i].y = Ranf(-0.01f, 0.01f);
   velocity[i].z = Ranf(0.0f, 0.0f);
#else
   for(int yy = 0; yy < SPOT_SIZE; yy++)
      for(int xx = 0; xx < SPOT_SIZE; xx++)
      {
         if(Vector2f(xx - HALF_SPOT_SIZE, yy - HALF_SPOT_SIZE).length() < HALF_SPOT_SIZE)
         {
            int index = (yy + y - int(HALF_SPOT_SIZE)) * NUM_PARTICLES_X + xx + x - int(HALF_SPOT_SIZE);
            if(index > 0 && index < NUM_PARTICLES - 1)
            {
               Vector4f& currentV = velocity[(yy + y - int(HALF_SPOT_SIZE)) * NUM_PARTICLES_X + xx + x - int(HALF_SPOT_SIZE)];

               currentV.x = v.x;
               currentV.y = v.y;
            }
         }
      }
#endif
   glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void ComputeShaderTest::onTouchEvent(TouchEventType eventType, const Vector2f& position)
{
   Vector2f viewSize = mDirector.getViewSize();
   const Vector2f p(position.x, viewSize.y - 1.0f - position.y);

   if(eventType == TE_MOVE || eventType == TE_DOWN)
   {
      //Vector2f v(p - mLastTouchPosition);
      Vector2f v(100.0f, 0.0f);
      float l = v.length();
      if(l > 0.000001)
      {
         /*const int spotsCount = (int)std::max(2.0f, l / SPOT_SIZE) * 2;
         
         Vector2f lastPosition = mLastTouchPosition;
         for(int i = 1; i < spotsCount; i++)
         {
            float t = i / ((float)spotsCount - 1.0f);
            Vector2f currentPosition = mLastTouchPosition + v * t;
            
            Vector2f currentV = currentPosition - lastPosition;
            currentV.x /= NUM_PARTICLES_X;
            currentV.y /= NUM_PARTICLES_Y;

            setSpot(currentPosition, currentV * 1000.0f);
            lastPosition = currentPosition;
         }*/
         setSpot(p, v);
      }
   }
   mLastTouchPosition = p;
}