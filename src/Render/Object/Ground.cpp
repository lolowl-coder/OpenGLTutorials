#include "Render/Object/Ground.h"

Vector3f Ground::genVertex(const int row, const int col)
{
   float x = col / (float)mDimensions.x * mSize.x;
   float z = row / (float)mDimensions.y * mSize.y;
   float y = 0.0f;

   return Vector3f(x, y, z);
   //return Grid::genVertex(row, col);
}