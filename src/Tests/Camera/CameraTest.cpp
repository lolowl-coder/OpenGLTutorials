#include "Render/GLHeaders.h"
#include "Render/RenderContext.h"
#include "Tests/Camera/CameraTest.h"

#include <vector>

CameraTest::CameraTest(const Director& director, const std::string& name)
: ICameraTest(director, name)
{

}

void CameraTest::initWorldAxes()
{
   Vector3f vertices[] = {Vector3f(), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f)};
   GLuint indices[] = {0, 1, 0, 2, 0, 3};

   glGenBuffers(1, &mWorldAxesVBO);
   checkGLError("Gen axes vbo");
   glBindBuffer(GL_ARRAY_BUFFER, mWorldAxesVBO);
   checkGLError("Bind axes vbo");
   glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vector3f), &vertices[0], GL_STATIC_DRAW);
   checkGLError("Pass grid vertices to GPU");

   glGenBuffers(1, &mWorldAxesIBO);
   checkGLError("Gen axes ibo");
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mWorldAxesIBO);
   checkGLError("Bind axes ibo");

   glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
   checkGLError("Pass axes indices to GPU");
}

void CameraTest::init()
{
   ICameraTest::init();

   initWorldAxes();

   const Vector2i dimensions(100, 100);
   mGrid.setDimensions(dimensions);
   const Vector2f size(10.0f, 10.0f);
   mGrid.setSize(size);
   mGrid.init();
   Material material(Vector4f(0.0f, 0.0f, 0.0f, 1.0f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 4.0f);
   mGrid.setMaterial(material);
   Light light(Vector3f(3.0f, 3.0f, 3.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 8.0f);
   mGrid.setLight(light);

   mAxesShader.load("Data/Shader/Camera/axes.vs", "Data/Shader/Common/colored.fs");
}

void CameraTest::deinit()
{
   glDeleteBuffers(1, &mWorldAxesVBO);
   glDeleteBuffers(1, &mWorldAxesIBO);

   mGrid.deinit();
   mAxesShader.deinit();

   checkGLError("Delete GL objects");
}

void CameraTest::renderWorldAxes()
{
   mAxesShader.bind();
   Matrix4f v = mCamera.getV();
   Matrix4f p = mCamera.getP();
   mAxesShader.setMVP(p * v);

   glBindBuffer(GL_ARRAY_BUFFER, mWorldAxesVBO);
   checkGLError("Bind axes vbo");

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mWorldAxesIBO);
   checkGLError("Bind axes ibo");

   glEnableVertexAttribArray(0);
   checkGLError("Enable vertex attrib array");
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
   checkGLError("Set vertex attrib pointer");

   glLineWidth(3);
   glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
}

void CameraTest::run()
{
   ICameraTest::run();

   //mTime += mDirector.getTimeDelta() * 10.0f;
   //Matrix4f transform = Matrix4f::createRotationAroundAxis(0.0f, mTime, 0.0f);
   //mGrid.setTransform(transform);

   renderWorldAxes();

   mGrid.render(mCamera);
}