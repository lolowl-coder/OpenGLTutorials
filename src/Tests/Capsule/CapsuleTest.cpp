#include "Render/GLHeaders.h"
#include "Render/RenderContext.h"
#include "Tests/Capsule/CapsuleTest.h"
#include "Platform/Log.h"

#include <vector>

CapsuleTest::CapsuleTest(Director& director, const std::string& name)
   : CameraTest(director, name)
   , mCapsuleVertexBuffer(GL_ARRAY_BUFFER, 0, 0)
   , mCapsuleIndexBuffer(GL_ELEMENT_ARRAY_BUFFER, 0, 0)
{
}
   
typedef std::vector<Vector3f> tVertices;
typedef std::vector<uint16_t> tIndices;

float vertices[] = {
0.866025, 0.500000, 2.000000,
1.000000, 0.000000, 2.000000,
1.000000, 0.000000, 1.000000,
0.866025, 0.500000, 1.000000,
0.500000, 0.866025, 2.000000,
0.500000, 0.866025, 1.000000,
0.000000, 1.000000, 2.000000,
0.000000, 1.000000, 1.000000,
-0.500000, 0.866025, 2.000000,
-0.500000, 0.866025, 1.000000,
-0.866025, 0.500000, 2.000000,
-0.866025, 0.500000, 1.000000,
-1.000000, 0.000000, 2.000000,
-1.000000, 0.000000, 1.000000,
-0.866025, -0.500000, 2.000000,
-0.866025, -0.500000, 1.000000,
-0.500000, -0.866025, 2.000000,
-0.500000, -0.866025, 1.000000,
-0.000000, -1.000000, 2.000000,
-0.000000, -1.000000, 1.000000,
0.500000, -0.866025, 2.000000,
0.500000, -0.866025, 1.000000,
0.866025, -0.500000, 2.000000,
0.866025, -0.500000, 1.000000,
0.866025, 0.000000, 0.500000,
0.813798, 0.296198, 0.500000,
0.500000, 0.000000, 0.133975,
0.433013, 0.750000, 0.500000,
0.000000, 0.000000, 0.000000,
0.383022, 0.321394, 0.133975,
-0.150384, 0.852869, 0.500000,
0.086824, 0.492404, 0.133975,
-0.663414, 0.556670, 0.500000,
-0.250000, 0.433013, 0.133975,
-0.866025, 0.000000, 0.500000,
-0.469846, 0.171010, 0.133975,
-0.663414, -0.556670, 0.500000,
-0.469846, -0.171010, 0.133975,
-0.150384, -0.852869, 0.500000,
-0.250000, -0.433013, 0.133975,
0.433013, -0.750000, 0.500000,
0.086824, -0.492404, 0.133975,
0.813798, -0.296198, 0.500000,
0.383022, -0.321394, 0.133975,
0.813798, 0.296198, 2.500000,
0.866025, 0.000000, 2.500000,
0.500000, 0.000000, 2.866025,
0.433013, 0.750000, 2.500000,
0.383022, 0.321394, 2.866025,
0.000000, 0.000000, 3.000000,
-0.150384, 0.852869, 2.500000,
0.086824, 0.492404, 2.866025,
-0.663414, 0.556670, 2.500000,
-0.250000, 0.433013, 2.866025,
-0.866025, 0.000000, 2.500000,
-0.469846, 0.171010, 2.866025,
-0.663414, -0.556670, 2.500000,
-0.469846, -0.171010, 2.866025,
-0.150384, -0.852869, 2.500000,
-0.250000, -0.433013, 2.866025,
0.433013, -0.750000, 2.500000,
0.086824, -0.492404, 2.866025,
0.813798, -0.296198, 2.500000,
0.383022, -0.321394, 2.866025};

uint16_t indices[] = {
0, 1, 2,
0, 2, 3,
4, 3, 5,
4, 0, 3,
6, 5, 7,
6, 4, 5,
8, 7, 9,
8, 6, 7,
10, 9, 11,
10, 8, 9,
12, 11, 13,
12, 10, 11,
14, 13, 15,
14, 12, 13,
16, 15, 17,
16, 14, 15,
18, 17, 19,
18, 16, 17,
20, 19, 21,
20, 18, 19,
22, 21, 23,
22, 20, 21,
1, 23, 2,
1, 22, 23,
24, 25, 2,
26, 25, 24,
2, 25, 3,
3, 27, 5,
5, 27, 7,
25, 27, 3,
28, 29, 26,
25, 29, 27,
26, 29, 25,
7, 30, 9,
27, 30, 7,
28, 31, 29,
27, 31, 30,
29, 31, 27,
9, 32, 11,
30, 32, 9,
28, 33, 31,
30, 33, 32,
31, 33, 30,
11, 34, 13,
13, 34, 15,
32, 34, 11,
28, 35, 33,
32, 35, 34,
33, 35, 32,
15, 36, 17,
34, 36, 15,
28, 37, 35,
35, 37, 34,
34, 37, 36,
17, 38, 19,
36, 38, 17,
28, 39, 37,
37, 39, 36,
36, 39, 38,
19, 40, 21,
21, 40, 23,
38, 40, 19,
28, 41, 39,
39, 41, 38,
38, 41, 40,
23, 42, 2,
2, 42, 24,
24, 42, 26,
40, 42, 23,
26, 43, 28,
28, 43, 41,
41, 43, 40,
42, 43, 26,
40, 43, 42,
44, 45, 1,
44, 46, 45,
44, 1, 0,
47, 0, 4,
47, 4, 6,
47, 44, 0,
48, 49, 46,
48, 44, 47,
48, 46, 44,
50, 6, 8,
50, 47, 6,
51, 49, 48,
51, 47, 50,
51, 48, 47,
52, 8, 10,
52, 50, 8,
53, 49, 51,
53, 50, 52,
53, 51, 50,
54, 10, 12,
54, 12, 14,
54, 52, 10,
55, 49, 53,
55, 52, 54,
55, 53, 52,
56, 14, 16,
56, 54, 14,
57, 49, 55,
57, 55, 54,
57, 54, 56,
58, 16, 18,
58, 56, 16,
59, 49, 57,
59, 57, 56,
59, 56, 58,
60, 18, 20,
60, 20, 22,
60, 58, 18,
61, 49, 59,
61, 59, 58,
61, 58, 60,
62, 22, 1,
62, 1, 45,
62, 45, 46,
62, 60, 22,
63, 46, 49,
63, 49, 61,
63, 61, 60,
63, 62, 46,
63, 60, 62};

/*void genHemisphere(const float xOffset, const float xScale, tVertices& vertices, tIndices& indices)
{
   const int detX = 3;
   const int detY = 3;
   const float r = 3.0f;
   const float stepX = r / detX;
   const float stepY = 2.0f * M_PI / detY;
   const float stepR = M_PI * 0.5f / detX;

   for(int x = 0; x < detX + 1; x++)
   {
      if(x == 0)
      {
         vertices.push_back(Vector3f(0.0, 0.0f, 0.0f));
      }
      else
      {
         for(int y = 0; y < detY; y++)
         {
            const float angle = y * stepY;
            const float curR = sqrt(pow(r, 2.0f) - pow(r - x * stepX, 2.0f));
            vertices.push_back(Vector3f(x * stepX, sin(angle) * curR, cos(angle) * curR));
            uint16_t indexOffset = (x - 1) * detY + 1;
            if(indexOffset + y > 1)
            {
               indices.push_back(indexOffset + y);
               indices.push_back(max(0, indexOffset + y - detY));
               indices.push_back(max(0, indexOffset + y - detY - 1));
               indices.push_back(indexOffset + y);
               indices.push_back(max(0, indexOffset + y - detY - 1));
               indices.push_back(max(0, indexOffset + y - 1));
            }
         }
      }
   }
}*/

void CapsuleTest::generateCapsule()
{
   /*tVertices vertices;
   tIndices indices;

   genHemisphere(0.0f, 1.0f, vertices, indices);*/

   mCapsuleVertexBuffer.setData(sizeof(vertices), vertices);
   mCapsuleIndexBuffer.setData(sizeof(indices), indices);
   mVerticesCount = sizeof(vertices) / sizeof(float) / 3;
   mIndicesCount = sizeof(indices) / sizeof(uint16_t);
}

void CapsuleTest::init()
{
   CameraTest::init();

   //shader
   mCapsuleShader.init();
   mCapsuleShader.bind();

   generateCapsule();

   //states
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glLineWidth(1.0f);
}

void CapsuleTest::deinit()
{
   mCapsuleVertexBuffer.deinit();
   checkGLError("Delete GL objects");
   mCapsuleIndexBuffer.deinit();
   checkGLError("Delete GL objects");
}

void CapsuleTest::renderCapsule()
{
   Camera& camera = mDirector.getCamera();
   Matrix4f v = camera.getV();
   Matrix4f p = camera.getP();
   mCapsuleShader.setUniforms(Vector4f(1.0f, 0.0f, 0.0f, 1.0f), p * v);

   mCapsuleVertexBuffer.bind();
   checkGLError("Bind poly line vbo");
   
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
   mCapsuleIndexBuffer.bind();

   glEnable(GL_DEPTH_TEST);
   glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_SHORT, nullptr);
   mCapsuleShader.setUniforms(Vector4f(0.0f, 1.0f, 0.0f, 1.0f), p * v);
   glDisable(GL_DEPTH_TEST);
   glDrawArrays(GL_POINTS, 0, mVerticesCount);
}

void CapsuleTest::run()
{
   ICameraTest::run();
   renderWorldAxes();
   mCapsuleShader.bind();
   renderCapsule();
}