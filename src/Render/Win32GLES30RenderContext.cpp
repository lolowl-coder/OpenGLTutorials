#include "Platform/Log.h"
#include "Win32GLES30RenderContext.h"

Win32GLES30RenderContext::Win32GLES30RenderContext()
: m_display(EGL_NO_DISPLAY)
, m_config(0)
, m_surface(EGL_NO_SURFACE)
, m_format(-1)
, m_context(EGL_NO_CONTEXT)
, m_depthBits(0)
{
}

void Win32GLES30RenderContext::init( void* window_handle)
{
   initEGL();
   chooseConfig();
   create(NULL);
   createSurface(window_handle);
   makeCurrent(true);

   GLint attribsCount;
   glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribsCount);

   logMsg("Max vertex attribs count: %i", attribsCount);

   GLint majorVersion;
   glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
   GLint minorVersion;
   glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

   logMsg("OpenGL ES context %i.%i initialized", majorVersion, minorVersion);
   logExtensions();
}

bool Win32GLES30RenderContext::create(Win32GLES30RenderContext* shared_with)
{
	EGLContext shared_with_context = EGL_NO_CONTEXT;

	const EGLint attrib_list[] =
	{
		EGL_CONTEXT_MAJOR_VERSION_KHR, 3,
      EGL_CONTEXT_MINOR_VERSION_KHR, 1,
		EGL_NONE
	};

	m_context = eglCreateContext(m_display, m_config, NULL, attrib_list);
	if(m_context == EGL_NO_CONTEXT)
	{
		if(!checkEGLError("Create render context"))
			return false;
	}

	//LOG("EGL context created.");

	return true;
}

bool Win32GLES30RenderContext::initEGL()
{
	EGLint  majorVersion;
	EGLint  minorVersion;
	int result;

	//LOG("Init EGL");

	m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	checkEGLError("GetDisplay");

	if(m_display == EGL_NO_DISPLAY)
	{
		logMsg("Unable to open connection to local windowing system");
		return false;
	}

	result = eglInitialize(m_display, &majorVersion, &minorVersion);

	checkEGLError("GetDisplay");

	if(!result)
	{
		logMsg("Unable to initialize EGL");
		return false;
	}

	//LOG("EGL " << majorVersion << "." << minorVersion << " initialized");

	return true;
}

bool Win32GLES30RenderContext::chooseConfig()
{
	EGLint attrib_list[] =
	{
	   EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
	   /*EGL_RED_SIZE, 8,
	   EGL_GREEN_SIZE, 8,
	   EGL_BLUE_SIZE, 8,
	   EGL_ALPHA_SIZE, 8,*/
	   EGL_DEPTH_SIZE, 16,
	   EGL_NONE
	};
	EGLint numConfigs;

	//LOG("Choose config");

	if(!eglChooseConfig(m_display, attrib_list, &m_config, 1, &numConfigs))
	{
		logMsg("Can't choose EGL config.");
		return false;
	}

	if(!eglGetConfigAttrib(m_display, m_config, EGL_NATIVE_VISUAL_ID, &m_format))
	{
		logMsg("Can't get native visual id.");
		return false;
	}

	/*EGLint red_size;
	if(eglGetConfigAttrib(m_display, m_config, EGL_RED_SIZE, &red_size))
		LOG("Red size: " << red_size);

	EGLint green_size;
	if(eglGetConfigAttrib(m_display, m_config, EGL_GREEN_SIZE, &green_size))
		LOG("Green size: " << green_size);

	EGLint blue_size;
	if(eglGetConfigAttrib(m_display, m_config, EGL_BLUE_SIZE, &blue_size))
		LOG("Blue size: " << blue_size);

	EGLint alpha_size;
	if(eglGetConfigAttrib(m_display, m_config, EGL_ALPHA_SIZE, &alpha_size))
		LOG("Alpha size: " << alpha_size);*/

	EGLint depth_size;
	eglGetConfigAttrib(m_display, m_config, EGL_DEPTH_SIZE, &depth_size);
	//LOG("Depth size: " << depth_size);

   m_depthBits = depth_size;

	//LOG("EGL config chosen.");

	return true;
}

bool Win32GLES30RenderContext::checkEGLError(const std::string& pre_msg)
{
	EGLint error = eglGetError();
	switch(error)
	{
	case EGL_NOT_INITIALIZED:
		{
         logMsg("[%s] Not initalized.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_ACCESS:
		{
			logMsg("[%s] Bad access.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_ALLOC:
		{
			logMsg("[%s] Bad alloc.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_ATTRIBUTE:
		{
			logMsg("[%s] Bad attribute.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_CONFIG:
		{
			logMsg("[%s] Bad config.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_CONTEXT:
		{
			logMsg( "[%s] Bad context.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_CURRENT_SURFACE:
		{
			logMsg( "[%s] Bad current surface.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_DISPLAY:
		{
			logMsg( "[%s] Bad display.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_MATCH:
		{
			logMsg( "[%s] Bad match.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_NATIVE_PIXMAP:
		{
			logMsg( "[%s] Bad native pixmap.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_NATIVE_WINDOW:
		{
			logMsg("[%s] Bad native window.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_PARAMETER:
		{
			logMsg("[%s] Bad parameter.", pre_msg.c_str());
			return false;
		}
	case EGL_BAD_SURFACE:
		{
			logMsg("[%s] Bad surface.", pre_msg.c_str());
			return false;
		}
	case EGL_CONTEXT_LOST:
		{
			logMsg("[%s] Context lost.", pre_msg.c_str());
			return false;
		}
	}

	return true;
}

bool Win32GLES30RenderContext::makeCurrent(bool value)
{
	EGLBoolean result;

	if(!m_surface)
	{
		logMsg("Can't make context current or not current. Wrong surface");
		return false;
	}

	if(!m_context)
	{
		logMsg("Can't make context current or not current. Context not created");
		return false;
	}

	if(value)
		result = eglMakeCurrent(m_display, m_surface, m_surface, m_context);
	else
		result = eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

	if(!result)
	{
		checkEGLError("Make context current");
		return false;
	}

	return true;
}

bool Win32GLES30RenderContext::createSurface(void* window)
{
	/*EGLint attrib_list[] =
	{
		EGL_RENDER_BUFFER, EGL_BACK_BUFFER, EGL_NONE
	};*/

#ifdef __ANDROID__
	ANativeWindow_setBuffersGeometry((ANativeWindow*)window, 0, 0, m_format);
#endif
   NativeWindowType** w =(NativeWindowType**)window;
	m_surface = eglCreateWindowSurface(m_display
									   , m_config
									   , (NativeWindowType)*w
									   //, attrib_list);
									   , NULL);
	if(m_surface == EGL_NO_SURFACE)
	{
		switch(eglGetError())
		{
			case EGL_BAD_MATCH:
				logMsg("Check window and EGLConfig attributes to determine "
					   , "compatibility, or verify that the EGLConfig "
					   , "supports rendering to a window.");
			break;
			case EGL_BAD_CONFIG:
				logMsg("Verify that provided EGLConfig is valid.");
			break;
			case EGL_BAD_NATIVE_WINDOW:
				logMsg("Verify that provided EGLNativeWindow is valid.");
			break;
			case EGL_BAD_ALLOC:
				logMsg("Not enough resources available. Handle and recover.");
			break;
		}

		return false;
	}

	makeCurrent(true);

	//LOG("EGL surface created.");

	return true;
}

Vector2f Win32GLES30RenderContext::surfaceSize()
{
	EGLint sw = 0;
	EGLint sh = 0;

	if(m_surface != EGL_NO_SURFACE)
	{
		eglQuerySurface(m_display, m_surface, EGL_WIDTH, &sw);
		eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &sh);
	}

	return Vector2f((float)sw, (float)sh);
}

bool Win32GLES30RenderContext::destroySurface()
{
	if(m_surface != EGL_NO_SURFACE)
	{
		makeCurrent(false);
		eglDestroySurface(m_display, m_surface);
		m_surface = EGL_NO_SURFACE;

		//LOG("Surface destroyed");
		return true;
	}

	return false;
}

void Win32GLES30RenderContext::destroy()
{
	if(m_display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if(m_context != EGL_NO_CONTEXT)
		{
			eglDestroyContext(m_display, m_context);
			m_context = EGL_NO_CONTEXT;
		}

		eglTerminate(m_display);
		m_display = EGL_NO_DISPLAY;

		//LOG("Graphics deinitialized.");
	}
	/*else
		LOG("No need to deinitialize graphics.");*/
}

void Win32GLES30RenderContext::swapBuffers()
{
	eglSwapBuffers(m_display, m_surface);
}

void Win32GLES30RenderContext::logExtensions()
{
   logMsg("Extensions:");

   GLint n = 0;
   glGetIntegerv(GL_NUM_EXTENSIONS, &n);
   for(GLint i = 0; i < n; i++)
   {
      const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
      logMsg(extension);
   }
}

bool Win32GLES30RenderContext::created()
{
	return m_context != EGL_NO_CONTEXT;
}