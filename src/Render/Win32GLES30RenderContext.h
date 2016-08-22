#pragma once

#include "Render/RenderContextBase.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl2ext.h>
#include <khr/khrplatform.h>

#include "vmath.h"

class Win32GLES30RenderContext : public RenderContextBase
{
public:
	Win32GLES30RenderContext ();

   void init( void* window_handle );
   bool makeCurrent ( bool value );
   Vector2f surfaceSize ();
   bool destroySurface ();
   void destroy ();
   void swapBuffers ();

private:
	bool create ( Win32GLES30RenderContext* shared_with );
	bool initEGL ();
	bool chooseConfig ();
	bool checkEGLError ( const std::string& pre_msg );
	bool createSurface ( void* window_handle );
   void logExtensions();

	bool created ();

private:
	EGLDisplay m_display;
	EGLConfig m_config;
	EGLSurface m_surface;
	EGLint m_format;
	EGLContext m_context;
	int m_depthBits;
};