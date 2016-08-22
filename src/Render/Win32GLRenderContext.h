#pragma once

#include "Render/RenderContextBase.h"

#include "vmath.h"

class Win32GLRenderContext : public RenderContextBase
{
public:
   void init ( void* window_handle );
   void makeCurrent ( bool value );
   Vector2f surfaceSize ();
   bool destroySurface ();
   void destroy ();
   void swapBuffers ();

private:

private:
};