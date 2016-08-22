#include "Platform/Log.h"
#include "Win32GLRenderContext.h"

#include <windows.h>

GLenum data_type[] = { GL_FLOAT
, GL_SHORT
, GL_UNSIGNED_SHORT
, GL_INT
, GL_UNSIGNED_INT
, GL_BYTE
, GL_UNSIGNED_BYTE };

HGLRC rc = 0;
HDC dc = 0;

void Win32GLRenderContext::init ( void* window_handle )
{
   dc = GetDC ( *(HWND*)window_handle );
   PIXELFORMATDESCRIPTOR pfd;
   GLenum err;
   int format;
   char* version;

   ZeroMemory ( &pfd, sizeof( pfd ) );
   pfd.nSize = sizeof( pfd );
   pfd.nVersion = 1;
   pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
      PFD_DOUBLEBUFFER;
   pfd.iPixelType = PFD_TYPE_RGBA;
   pfd.cColorBits = 32;
   pfd.cDepthBits = 24;
   pfd.iLayerType = PFD_MAIN_PLANE;

   format = ChoosePixelFormat ( dc, &pfd );

   if ( !format )
   {
      logMsg("Can't choose pixel format");

      return;
   }

   if ( !SetPixelFormat ( dc, format, &pfd ) )
   {
      logMsg("Can't set pixel format");

      return;
   }

   rc = wglCreateContext ( dc );
   if(!wglMakeCurrent ( dc, rc ))
      logMsg("Can't make GL context current");

   version = (char*)glGetString ( GL_VERSION );
   logMsg("OpenGL version: %s", version);

   err = glewInit ();
   if ( GLEW_OK != err )
   {
      logMsg("Can't initialize glew: %s", glewGetErrorString(err));

      return;
   }

   GLint majorVersion;
   glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
   GLint minorVersion;
   glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

   logMsg("OpenGL context %i.%i initialized", majorVersion, minorVersion);
}

void Win32GLRenderContext::makeCurrent ( bool value )
{
   if ( value )
      wglMakeCurrent ( dc, rc );
   else
      wglMakeCurrent ( NULL, NULL );
}

void Win32GLRenderContext::swapBuffers()
{
   SwapBuffers ( dc );
}

Vector2f Win32GLRenderContext::surfaceSize()
{
   return Vector2f();
}

bool Win32GLRenderContext::destroySurface()
{
   return true;
}

void Win32GLRenderContext::destroy()
{
   makeCurrent ( false );
   wglDeleteContext ( rc );
}