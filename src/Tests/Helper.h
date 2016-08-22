#pragma once

#include "Render/GLHeaders.h"
#include "vmath.h"

class Helper
{
public:
   static Vector2i getTouchPosition();
   static void setTouchPosition(const Vector2i& position);

private:
   static Vector2i mTouchPosition;
};