#include "Tests/Helper.h"

#include "Render/RenderContext.h"

Vector2i Helper::mTouchPosition;

Vector2i Helper::getTouchPosition()
{
   return mTouchPosition;
}

void Helper::setTouchPosition(const Vector2i& position)
{
   Helper::mTouchPosition = position;
}