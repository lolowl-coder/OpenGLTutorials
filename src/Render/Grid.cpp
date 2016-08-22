#include "Render/Grid.h"
#include "Render/RenderContext.h"
#include "Render/GLHeaders.h"

GridShader::GridShader()
: mMVPLocation(-1)
, mColorLocation(-1)
, mEyeLocation(-1)
{

}

void GridShader::setMVP(const Matrix4f& mvp)
{
   if(mMVPLocation == -1)
      mMVPLocation = uniformLocation("uMVP");

   uniformMatrix4(mMVPLocation, 1, &mvp);
}

void GridShader::setColor(const Vector4f& color)
{
   if(mColorLocation == -1)
      mColorLocation = uniformLocation("uColor");

   uniform4(mColorLocation, 1, &color);
}

void GridShader::setEye(const Vector3f& eye)
{
   if(mEyeLocation == -1)
      mEyeLocation = uniformLocation("uEye");

   uniform3(mEyeLocation, 1, &eye);
}

NormalShader::NormalShader()
: mNormalLengthLocation(-1)
, mNormalColorLocation(-1)
{

}

void NormalShader::setNormalLenght(const float length)
{
   if(mNormalLengthLocation == -1)
      mNormalLengthLocation = uniformLocation("uNormalLength");

   uniform1(mNormalLengthLocation, 1, &length);
}

void NormalShader::setNormalColor(const Vector4f& color)
{
   if(mNormalColorLocation == -1)
      mNormalColorLocation = uniformLocation("uNormalColor");

   uniform4(mNormalColorLocation, 1, &color);
}

struct Vertex
{
   Vector3f mPosition;
   Vector3f mNormal;
};

Grid::Grid()
: mNormals(GL_ARRAY_BUFFER, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0)
{

}

Vector3f Grid::genNormal(const int row, const int col)
{
   Vector3f result;
   Vector3f v0 = genVertex(row, col);
   Vector3f v1;
   Vector3f v2;
   int normalsCount = 0;

   if(row % 2 == 0)
   {
      if(row + 1 < mDimensions.y && col + 1 < mDimensions.x)
      {
         v1 = genVertex(row, col + 1);
         v2 = genVertex(row + 1, col);
         Vector3f n = ((v1 - v0) * (v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row + 1 < mDimensions.y && col - 1 < mDimensions.x)
      {
         v1 = genVertex(row + 1, col);
         v2 = genVertex(row - 1, col - 1);
         Vector3f n = ((v1 - v0) * (v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row + 1 < mDimensions.y && col - 1 < mDimensions.x)
      {
         v1 = genVertex(row + 1, col - 1);
         v2 = genVertex(row, col - 1);
         Vector3f n = ((v1 - v0) * (v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row - 1 < mDimensions.y && col - 1 < mDimensions.x)
      {
         v1 = genVertex(row, col - 1);
         v2 = genVertex(row - 1, col - 1);
         Vector3f n = ((v1 - v0) * (v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row - 1 < mDimensions.y && col - 1 < mDimensions.x)
      {
         v1 = genVertex(row - 1, col - 1);
         v2 = genVertex(row - 1, col);
         Vector3f n = ((v1 - v0) * (v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
   }

   result /= normalsCount;
   result.normalize();

   return result;
}

Vector3f Grid::genVertex(const int row, const int col)
{
   return Vector3f(col / (float)mDimensions.x * mSize.x, sin(Vector2f(col, row).length() * 2.0f) * 0.5f, row / (float)mDimensions.y * mSize.y);
}

void Grid::init(const Vector2i& dimensions, const Vector2f& size)
{
   mDimensions = dimensions;
   mSize = size;

   std::vector<Vertex> gridVertices;
   std::vector<GLuint> gridIndices;
   std::vector<Vector3f> gridNormals;

   /*
   6-7-8
   |/|/|
   3-4-5
   |\|\|
   0-1-2
   */

   //init vertices and indices
   for(int row = 0; row < mDimensions.y; row++)
   {
      for(int col = 0; col < mDimensions.x; col++)
      {
         if(row < mDimensions.y - 1)
         {
            int index = row * mDimensions.x;
            if(row % 2 == 0)
            {
               index += col;
            }
            else
            {
               index += (mDimensions.x - 1) - col;
            }

            gridIndices.push_back(index);
            gridIndices.push_back(index + mDimensions.x);
            if(col == mDimensions.x - 1 && row < mDimensions.y - 2)
            {
               gridIndices.push_back(index + mDimensions.x);
            }
         }

         Vertex v;
         
         v.mPosition = genVertex(row, col);
         v.mNormal = genNormal(row, col);
         gridVertices.push_back(v);

         gridNormals.push_back(v.mPosition);
         gridNormals.push_back(v.mPosition + v.mNormal);
      }
   }

   glGenBuffers(1, &mGridVBO);
   checkGLError("Gen grid vbo");
   glBindBuffer(GL_ARRAY_BUFFER, mGridVBO);
   checkGLError("Bind grid vbo");

   glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(Vertex), &gridVertices[0], GL_STATIC_DRAW);
   checkGLError("Pass grid vertices to GPU");

   glGenBuffers(1, &mGridIBO);
   checkGLError("Gen grid ibo");
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGridIBO);
   checkGLError("Bind grid ibo");

   glBufferData(GL_ELEMENT_ARRAY_BUFFER, gridIndices.size() * sizeof(GLuint), &gridIndices[0], GL_STATIC_DRAW);
   checkGLError("Pass grid indices to GPU");

   mNormals.setData(gridNormals.size() * sizeof(Vector3f), &gridNormals[0]);

   mShader.load("Data/Shader/Camera/grid.vs", "Data/Shader/Camera/grid.fs");
   mNormalShader.load("Data/Shader/Camera/normal.vs", "Data/Shader/Camera/normal.fs");
}

void Grid::deinit()
{
   glDeleteBuffers(1, &mGridVBO);
   glDeleteBuffers(1, &mGridIBO);
   mNormals.deinit();

   mShader.deinit();
   mNormalShader.deinit();
}

void Grid::bindShader(const Matrix4f& vp, const Vector3f& eye)
{
   mShader.bind();
   mShader.setMVP(vp);
   mShader.setColor(Vector4f(0.0f, 0.1f, 1.0f, 1.0f));
   mShader.setEye(eye);
}

void Grid::render(const Matrix4f& vp, const Vector3f& eye)
{
   bindShader(vp, eye);

   glBindBuffer(GL_ARRAY_BUFFER, mGridVBO);
   checkGLError("Bind grid vbo");

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGridIBO);
   checkGLError("Bind grid ibo");

   glEnableVertexAttribArray(0);
   checkGLError("Enable vertex attrib array");
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
   checkGLError("Set vertex attrib pointer");

   glEnableVertexAttribArray(1);
   checkGLError("Enable vertex attrib array");
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::mNormal));
   checkGLError("Set vertex attrib pointer");

   glCullFace(GL_FRONT_AND_BACK);

   int indicesCount =
      (mDimensions.y - 1) * 2 * mDimensions.x //points count for each stripe(stripes count = GRID_SIZE - 1)
      +
      mDimensions.y - 2;                    //duplicated vertices
#if 0
   glLineWidth(1);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glDrawElements(GL_TRIANGLE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);

   glPointSize(5);
   glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
   glDrawElements(GL_TRIANGLE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);
#else
   glDrawElements(GL_TRIANGLE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);
#endif

   checkGLError("Draw arrays");
   glDisableVertexAttribArray(0);
}
