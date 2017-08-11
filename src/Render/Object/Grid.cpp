#include "Platform/Log.h"
#include "Render/Object/Grid.h"
#include "Render/GLHeaders.h"
#include "Render/RenderContext.h"
#include "Render/Vertex.h"

NormalShader::NormalShader()
: mColorLocation(-1)
, mMVPLocation(-1)
{

}

void NormalShader::setColor(const Vector4f& color)
{
   if(mColorLocation == -1)
      mColorLocation = uniformLocation("uColor");

   uniform4(mColorLocation, 1, &color);
}

void NormalShader::setMVP(const Matrix4f& mvp)
{
   if(mMVPLocation == -1)
      mMVPLocation = uniformLocation("uMVP");

   uniformMatrix4(mMVPLocation, 1, &mvp);
}

Grid::Grid()
: mEdges(GL_ARRAY_BUFFER, 0, 0)
{
   mEdges.addAttribute(Attribute(3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0));
}

bool Grid::genNormalHelper(const Vector3f& v, const Vector2i& rowCol, const Vector2i& normal0Offset, const Vector2i& normal1Offset, Vector3f& accumulatedNormal)
{
   Vector2i neighbour0RowCol = rowCol + normal0Offset;
   Vector2i neighbour1RowCol = rowCol + normal1Offset;
   bool result = false;

   if(neighbour0RowCol.x < mDimensions.x && neighbour0RowCol.x >= 0 &&
      neighbour0RowCol.y < mDimensions.y && neighbour0RowCol.y >= 0 &&
      neighbour1RowCol.x < mDimensions.x && neighbour1RowCol.x >= 0 &&
      neighbour1RowCol.y < mDimensions.y && neighbour1RowCol.y >= 0)
   {
      Vector3f v1 = genVertex(neighbour0RowCol.x, neighbour0RowCol.y);
      Vector3f v2 = genVertex(neighbour1RowCol.x, neighbour1RowCol.y);
      Vector3f n = ((v2 - v).crossProduct(v1 - v));
      n.normalize();
      accumulatedNormal += n;

      result = true;
   }

   return result;
}

Vector3f Grid::genNormal(const int row, const int col)
{
   Vector3f result;
   Vector3f v0 = genVertex(row, col);
   Vector3f v1;
   Vector3f v2;
   int normalsCount = 0;

#if 0
   float even = (row + 1) % 2;

   if(even)
   {
      if(row + 1 < mDimensions.y && col + 1 < mDimensions.x)
      {
         v1 = genVertex(row, col + 1);
         v2 = genVertex(row + 1, col);
         Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row + 1 < mDimensions.y && col - 1 < mDimensions.x)
      {
         v1 = genVertex(row + 1, col);
         v2 = genVertex(row - 1, col - 1);
         Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row + 1 < mDimensions.y && col - 1 < mDimensions.x)
      {
         v1 = genVertex(row + 1, col - 1);
         v2 = genVertex(row, col - 1);
         Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row - 1 < mDimensions.y && col - 1 < mDimensions.x)
      {
         v1 = genVertex(row, col - 1);
         v2 = genVertex(row - 1, col - 1);
         Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row - 1 < mDimensions.y && col - 1 < mDimensions.x)
      {
         v1 = genVertex(row - 1, col - 1);
         v2 = genVertex(row - 1, col);
         Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
   }
   else
   {
      if(row + 1 < mDimensions.y && col + 1 < mDimensions.x)
      {
         v1 = genVertex(row, col + 1);
         v2 = genVertex(row + 1, col + 1);
         Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row + 1 < mDimensions.y && col + 1 < mDimensions.x)
      {
         v1 = genVertex(row + 1, col + 1);
         v2 = genVertex(row + 1, col);
         Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row + 1 < mDimensions.y && col - 1 < mDimensions.x)
      {
         v1 = genVertex(row + 1, col);
         v2 = genVertex(row, col - 1);
         Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row - 1 < mDimensions.y && col - 1 < mDimensions.x)
      {
         v1 = genVertex(row, col - 1);
         v2 = genVertex(row - 1, col);
         Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
      if(row - 1 < mDimensions.y && col + 1 < mDimensions.x)
      {
         v1 = genVertex(row - 1, col);
         v2 = genVertex(row - 1, col + 1);
         Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
         n.normalize();
         result += n;
         normalsCount++;
      }
   }
#else
   /*normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(0, 1), Vector2i(1, 1), result);
   normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(1, 1), Vector2i(1, 0), result);
   normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(1, 0), Vector2i(1, -1), result);
   normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(1, -1), Vector2i(0, -1), result);
   normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(0, -1), Vector2i(-1, -1), result);
   normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(-1, -1), Vector2i(-1, 0), result);
   normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(-1, 0), Vector2i(-1, 1), result);
   normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(-1, 1), Vector2i(0, 1), result);*/
   normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(0, 1), Vector2i(1, 0), result);
   normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(1, 0), Vector2i(0, -1), result);
   normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(0, -1), Vector2i(-1, 0), result);
   normalsCount += (int)genNormalHelper(v0, Vector2i(row, col), Vector2i(-1, 0), Vector2i(0, 1), result);
   /*if(row + 1 < mDimensions.y && col + 1 < mDimensions.x)
   {
      v1 = genVertex(row, col + 1);
      v2 = genVertex(row + 1, col);
      Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
      n.normalize();
      result += n;
      normalsCount++;
   }
   if(row + 1 < mDimensions.y && col - 1 < mDimensions.x)
   {
      v1 = genVertex(row + 1, col);
      v2 = genVertex(row, col - 1);
      Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
      n.normalize();
      result += n;
      normalsCount++;
   }
   if(row - 1 < mDimensions.y && col - 1 < mDimensions.x)
   {
      v1 = genVertex(row, col - 1);
      v2 = genVertex(row - 1, col);
      Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
      n.normalize();
      result += n;
      normalsCount++;
   }
   if(row - 1 < mDimensions.y && col + 1 < mDimensions.x)
   {
      v1 = genVertex(row - 1, col);
      v2 = genVertex(row, col + 1);
      Vector3f n = ((v1 - v0).crossProduct(v2 - v0));
      n.normalize();
      result += n;
      normalsCount++;
   }*/
#endif

   result /= normalsCount;
   result.normalize();

   return result;
}

Vector3f Grid::genVertex(const int row, const int col)
{
   float x = col / (float)mDimensions.x * mSize.x;
   float z = row / (float)mDimensions.y * mSize.y;
   float y = sin(Vector2f(x, z).length() * 5.0f) * 0.1f;
   
   return Vector3f(x, y, z);
}

void Grid::init()
{
   std::vector<VertexP3N3> gridVertices;
   std::vector<GLuint> gridIndices;
   std::vector<Vector3f> gridNormals;
   std::vector<Vector3f> edges;

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

         VertexP3N3 v(genVertex(row, col), genNormal(row, col));
         
         gridVertices.push_back(v);

         gridNormals.push_back(v.mPosition);
         gridNormals.push_back(v.mPosition + v.mNormal * 0.1f);
      }
   }

   for(int i = 0; i < gridIndices.size(); i++)
   {
      edges.push_back(gridVertices[gridIndices[i]].mPosition + Vector3f(0.0f, 0.01f, 0.0f));
   }

   glGenBuffers(1, &mVBO);
   checkGLError("Gen grid vbo");
   glBindBuffer(GL_ARRAY_BUFFER, mVBO);
   checkGLError("Bind grid vbo");
   glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(VertexP3N3), &gridVertices[0], GL_STATIC_DRAW);
   checkGLError("Pass grid vertices to GPU");

   glGenBuffers(1, &mIBO);
   checkGLError("Gen grid ibo");
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
   checkGLError("Bind grid ibo");
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, gridIndices.size() * sizeof(GLuint), &gridIndices[0], GL_STATIC_DRAW);
   checkGLError("Pass grid indices to GPU");

   mNormals.setData(gridNormals.size() * sizeof(Vector3f), &gridNormals[0]);

   mEdges.setData(edges.size() * sizeof(Vector3f), &edges[0]);

   mNormalShader.load("Data/Shader/Camera/normal.vs", "Data/Shader/Camera/normal.fs");
}

void Grid::deinit()
{
   glDeleteBuffers(1, &mVBO);
   glDeleteBuffers(1, &mIBO);

   mNormals.deinit();

   mNormalShader.deinit();
}

void Grid::render(const Camera& camera)
{
   glEnable(GL_DEPTH_TEST);
   checkGLError("Grid::render - enable depth test");

   glBindBuffer(GL_ARRAY_BUFFER, mVBO);
   checkGLError("Bind grid vbo");

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
   checkGLError("Bind grid ibo");

   glEnableVertexAttribArray(0);
   checkGLError("Enable vertex attrib array");
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP3N3), 0);
   checkGLError("Set vertex attrib pointer");

   glEnableVertexAttribArray(1);
   checkGLError("Enable normal attrib array");
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP3N3), (void*)offsetof(VertexP3N3, VertexP3N3::mNormal));
   checkGLError("Set normal attrib pointer");

   glCullFace(GL_BACK);

   int indicesCount =
      (mDimensions.y - 1) * 2 * mDimensions.x   //points count for each stripe(stripes count = GRID_SIZE - 1)
      +
      mDimensions.y - 2;                        //duplicated vertices
#if 0
   glLineWidth(1);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glDrawElements(GL_TRIANGLE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);

   glPointSize(5);
   glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
   glDrawElements(GL_TRIANGLE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#else
   glDrawElements(GL_TRIANGLE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);
#endif

   checkGLError("Draw arrays");
   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);

#if 0
   mNormalShader.bind();
   mNormalShader.setColor(Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
   mNormalShader.setMVP(camera.getP() * camera.getV() * mTranslation * mRotation);
   mEdges.enable();
   glLineWidth(2);
   glDrawArrays(GL_LINE_STRIP, 0, indicesCount);
   checkGLError("Draw arrays");
   mNormals.disable();
#endif

#if 0
   mNormalShader.bind();
   mNormalShader.setColor(Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
   mNormalShader.setMVP(camera.getP() * camera.getV() * mTranslation * mRotation);
   mNormals.enable();
   glLineWidth(1);
   glDrawArrays(GL_LINES, 0, mDimensions.x * mDimensions.y * 2);
   checkGLError("Draw arrays");
   mNormals.disable();
#endif
}

void Grid::setDimensions(const Vector2i& dimensions)
{
   mDimensions = dimensions;
}

void Grid::setSize(const Vector2f& size)
{
   mSize = size;
}

