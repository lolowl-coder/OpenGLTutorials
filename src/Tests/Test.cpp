#include "Platform/Log.h"
#include "Test.h"

Test::Test(const Director& director, const std::string& name)
: mDirector(director)
, mName(name)
{
   logMsg("create test %s", name.c_str());
}

void Test::onTouchEvent(TouchEventType eventType, const Vector2f& position)
{

}

void Test::onKeyboardEvent(KeyboardEventType eventType, int keyCode)
{

}