#pragma once

#include "Render/Object/Grid.h"

class Ground : public Grid
{
protected:
   virtual Vector3f genVertex(const int row, const int col);
};