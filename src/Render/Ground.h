#pragma once

#include "Render/Grid.h"

class Ground : public Grid
{
protected:
   virtual Vector3f genVertex(const int row, const int col);
};