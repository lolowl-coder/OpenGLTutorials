#include "Render/RectRenderer.h"
#include "Render/RenderContext.h"

RectRenderer::RectRenderer()
: mBuffer(GL_ARRAY_BUFFER, 0, 0)
{

}

void RectRenderer::init()
{
   Vector3f vertices[] = {
      Vector3f(0.0f, 0.0f, 0.0f),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector3f(1.0f, 1.0f, 0.0f)
   };

   mBuffer.setData(sizeof(Vector3f) * 4, &vertices[0]);
}

void RectRenderer::deinit()
{
   mBuffer.deinit();
}

void RectRenderer::render()
{
   mBuffer.enable();
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   checkGLError("RectRenderer::renderQuad, glDrawArrays");
   glDisableVertexAttribArray(0);
   checkGLError("BlendEquationTest::renderQuad");
}