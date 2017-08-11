#include "Render/Object/Cube.h"
#include "Render/Vertex.h"
#include "Render/RenderContext.h"

Cube::Cube()
: Object()
, mVertices(GL_ARRAY_BUFFER, 0, 0)
{

}

void Cube::init()
{
   std::vector<VertexP3N3> vertices;

   //far
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0.0f, 0.0f, -1.0f)));
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(0.0f, 0.0f, -1.0f)));
   vertices.push_back(VertexP3N3(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(0.0f, 0.0f, -1.0f)));
   vertices.push_back(VertexP3N3(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(0.0f, 0.0f, -1.0f)));

   //near
   vertices.push_back(VertexP3N3(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0.0f, 0.0f, 1.0f)));
   vertices.push_back(VertexP3N3(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0.0f, 0.0f, 1.0f)));
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(0.0f, 0.0f, 1.0f)));
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(0.0f, 0.0f, 1.0f)));

   //left
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(-1.0f, 0.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(-1.0f, 0.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(-1.0f, 0.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(-1.0f, 0.0f, 0.0f)));

   //right
   vertices.push_back(VertexP3N3(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(1.0f, 0.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(1.0f, 0.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(1.0f, 0.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(1.0f, 0.0f, 0.0f)));

   //top
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(0.0f, 1.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0.0f, 1.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(0.0f, 1.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(0.0f, 1.0f, 0.0f)));

   //bottom
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0.0f, -1.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(0.0f, -1.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(0.0f, -1.0f, 0.0f)));
   vertices.push_back(VertexP3N3(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0.0f, -1.0f, 0.0f)));

   mVertices.setData(sizeof(VertexP3N3) * vertices.size(), &vertices[0]);
}

void Cube::deinit()
{
   mVertices.deinit();
}

void Cube::render(const Camera& camera)
{
   mVertices.enable();

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP3N3), 0);

   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP3N3), (void*)offsetof(VertexP3N3, VertexP3N3::mNormal));

   for(int i = 0; i < 6; i++)
   {
      glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
   }

   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
}


