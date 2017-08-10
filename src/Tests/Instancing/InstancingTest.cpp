#include "Render/GLHeaders.h"
#include "Render/RenderContext.h"
#include "Tests/Helper.h"
#include "Tests/Instancing/InstancingTest.h"

#include "vmath.h"

#define R_COUNT 1000

static Vector3f vertices[] = {
   Vector3f(-0.5f, -0.5f, 0.0f),
   Vector3f(-0.5f,  0.5f, 0.0f),
   Vector3f( 0.5f, -0.5f, 0.0f),
   Vector3f( 0.5f,  0.5f, 0.0f)
};

static Vector2f texCoords[] = {
   Vector2f(0.0, 0.0),
   Vector2f(0.0, 1.0),
   Vector2f(1.0, 0.0),
   Vector2f(1.0, 1.0)
};

Vector3f positions[R_COUNT];

InstancingTest::InstancingTest(const Director& director, const std::string& name)
:Test(director, name)
{

}

void InstancingTest::init()
{
   for(int i = 0; i < R_COUNT; i++)
   {
      positions[i] = Vector3f(
         //i / (float)R_COUNT * 6.0f - 0.7f,
         rand() / (float)RAND_MAX * 2.0f - 1.0f,
         rand() / (float)RAND_MAX * 2.0f - 1.0f,
         //i / (float)R_COUNT * 3.0f - 0.5f,
         //0.0f,
         //0.0f,
         0.0f );
   }

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), vertices);

   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), texCoords);

   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), positions);
   glVertexAttribDivisor(2, 1);
   
   mShader.load("Data/Shader/Instancing/instancing.vs", "Data/Shader/Instancing/instancing.fs");
   mShader.bind();
   mShader.setTextureUnit(0);

   const Vector3f checkerColor0(0.0f, 0.0f, 0.0f);
   const Vector3f checkerColor1(1.0f, 1.0f, 1.0f);
   const int checkerSize = 16;
   mTexture.genChecker(checkerColor0, checkerColor1, checkerSize);
   mTexture.create();

   glDisable(GL_CULL_FACE);
   glDisable(GL_DEPTH_TEST);
}

void InstancingTest::deinit()
{
   mShader.deinit();
   mTexture.deinit();
}

void InstancingTest::run()
{
   glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, R_COUNT);
}