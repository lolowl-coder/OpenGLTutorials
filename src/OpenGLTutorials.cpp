#define NOMINMAX

#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>

#include "Director.h"
#include "Platform/Log.h"
#include "Render/GLHeaders.h"
#include "Render/RenderContext.h"
#include "Render/Vertex.h"
#include "Tests/BlendEquation/BlendEquationTest.h"
#include "Tests/Camera/CameraTest.h"
#include "Tests/ComputeShader/ComputeShaderTest.h"
#include "Tests/ComputeShader/ImageProcessingTest.h"
#include "Tests/Glow/GlowTest.h"
#include "Tests/Instancing/InstancingTest.h"
#include "Tests/ParticleSystem/ParticleSystemTest.h"

#include <string>
#include <fstream>

HWND windowHandle;

enum TestType
{
   TT_GLOW,
   TT_INSTANCING,
   TT_COMPUTE_SHADER,
   TT_BLEND_EQUATION,
   TT_CAMERA,
   TT_PARTICLE_SYSTEM,
   TT_IMAGE_PROCESSING,
   TT_COUNT
};

Test* currentTest = NULL;
TestType currentTestType = TT_COMPUTE_SHADER;
bool lButtonPressed = false;

TestType tests[TT_COUNT];

Director director;

void changeTest(TestType& testType)
{
   if(currentTest)
   {
      currentTest->deinit();
      delete currentTest;
      currentTest = NULL;
   }

   switch (testType)
   {
   case TT_GLOW:
      {
         currentTest = new GlowTest(director, "Glow");
      }
      break;
   case TT_INSTANCING:
      {
         currentTest = new InstancingTest(director, "Instancing");
      }
      break;
   case TT_COMPUTE_SHADER:
      {
         currentTest = new ComputeShaderTest(director, "Grid");
      }
      break;
   case TT_BLEND_EQUATION:
      {
         currentTest = new BlendEquationTest(director, "Blend equation");
      }
      break;
   case TT_CAMERA:
      {
         currentTest = new CameraTest(director, "Camera");
      }
      break;
   case TT_PARTICLE_SYSTEM:
      {
         currentTest = new ParticleSystemTest(director, "Particle system");
      }
      break;
   case TT_IMAGE_PROCESSING:
      {
         currentTest = new ComputeShaders::ImageProcessingTest(director, "Image processing");
      }
      break;
   }

   if(currentTest)
   {
      currentTest->init();
   }
}

void sendKeyEvent(UINT uiMsg, WPARAM wParam)
{
   switch(wParam)
   {
   case VK_UP:
      {
         currentTestType = (TestType)((currentTestType + 1) % TT_COUNT);
         changeTest(currentTestType);
      }
      break;
   }
}

LRESULT CALLBACK windowProc ( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uiMsg )
	{
		case WM_CLOSE:
				PostQuitMessage ( 0 );
				return 0;

		case WM_ACTIVATE:
			{
#if 0
				WORD active = LOWORD(wParam);
				BOOL fMinimized = (BOOL) HIWORD(wParam);
#endif
			}
			break;
		case WM_KEYDOWN:
			{
            if(currentTest)
            {
               currentTest->onKeyboardEvent(KE_DOWN, (int)wParam);
            }
			}
			break;
		case WM_KEYUP:
			{
				sendKeyEvent ( uiMsg, wParam );

            if(currentTest)
            {
               currentTest->onKeyboardEvent(KE_UP, (int)wParam);
            }
			}
			break;
		case WM_LBUTTONDOWN:
         {
            if(currentTest)
            {
               currentTest->onTouchEvent(TE_UP, Vector2i(GET_X_LPARAM ( lParam ), GET_Y_LPARAM ( lParam )));
            }

            lButtonPressed = true;
         }
         break;
		case WM_LBUTTONUP:
		/*case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:*/
			{
            if(currentTest)
            {
               currentTest->onTouchEvent(TE_UP, Vector2i(GET_X_LPARAM ( lParam ), GET_Y_LPARAM ( lParam )));
            }

            lButtonPressed = false;
			}
			break;
      case WM_MOUSEMOVE:
         {
            if(lButtonPressed)
            {
               if(currentTest)
               {
                  currentTest->onTouchEvent(TE_MOVE, Vector2i(GET_X_LPARAM ( lParam ), GET_Y_LPARAM ( lParam )));
               }
            }
         }
         break;
		case WM_SIZE:
				return 0;
	}

	return DefWindowProc ( hWnd, uiMsg, wParam, lParam );
}

void createWindow()
{
   WNDCLASS wc;
	RECT wRect;
	HINSTANCE hInstance;
	std::string application_name = director.getAppName();
   Vector2f viewSize = director.getViewSize();

	if ( !SystemParametersInfo ( SPI_GETWORKAREA, 0, &wRect, 0 ) )
	{
		wRect.left = 0L;
		wRect.right = (LONG)viewSize.y;
		wRect.top = 0L;
		wRect.bottom = (LONG)viewSize.x;
	}
	else
	{
		wRect.left = 0L;
		wRect.right = (LONG)(wRect.bottom / (viewSize.x / viewSize.y));
		wRect.top = 0L;
	}

	wRect.right = (LONG)viewSize.x;
   wRect.bottom = (LONG)viewSize.y;

	hInstance = GetModuleHandle ( NULL );

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)windowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon ( NULL, IDI_WINLOGO );
	wc.hCursor = LoadCursor ( NULL, IDC_ARROW );
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OGLES";

	RegisterClass(&wc);

	AdjustWindowRectEx ( &wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );

	windowHandle = CreateWindowEx ( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE
							 , "OGLES"
							 , application_name.c_str ()
							 , WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
							 , 0
							 , 0
							 , wRect.right - wRect.left
							 , wRect.bottom - wRect.top
							 , NULL
							 , NULL
							 , hInstance
							 , NULL);

	ShowWindow ( (HWND)windowHandle, SW_SHOW );
}

int _tmain(int argc, _TCHAR* argv[])
{
   director.setAppName("Learn OpenGL");
	createWindow ();

   //main loop
   bool bDone = false;
	MSG sMessage;
   double currentTime = GetTickCount() * 0.001f;

#ifdef GLES30
   Win32GLES30RenderContext rc;
#else
   Win32GLRenderContext rc;
#endif

   rc.init ( &windowHandle );

   bool textureBufferSupported =
      rc.isExtensionSupported("GL_EXT_texture_buffer_object") ||
      rc.isExtensionSupported("GL_EXT_texture_buffer");
   logMsg("Texture buffer supported: %s", textureBufferSupported ? "yes" : "no");

   bool stencilSamplingSupported =
      rc.isExtensionSupported("ARB_depth_texture") &&
      rc.isExtensionSupported("EXT_packed_depth_stencil");
   logMsg("Stencil sampling supported: %s", stencilSamplingSupported ? "yes" : "no");

   changeTest(currentTestType);

   glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while ( !bDone )
	{
		if ( PeekMessage ( &sMessage, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( sMessage.message == WM_QUIT )
			{
				bDone = 1;
			}
			else
			{
				//TranslateMessage ( &sMessage );
				DispatchMessage ( &sMessage );
			}
		}

      director.update();
      double t = GetTickCount() * 0.001f;
      double timeDelta = t - currentTime;
      static double fpsUpdateTime = 0;
      fpsUpdateTime += timeDelta;
      static int framesCount = 0;
      framesCount++;
      currentTime = t;
      director.setTimeDelta((float)timeDelta);
      director.setTime((float)t);

      if(fpsUpdateTime > 1.0)
      {
         char windowText[255];
         strcpy(windowText, director.getAppName().c_str());
         sprintf(windowText, "%s FPS: %i", director.getAppName().c_str(), framesCount);
         SetWindowText(windowHandle, windowText);

         fpsUpdateTime -= 1.0;
         framesCount = 0;
      }

      glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      if(currentTest)
      {
         currentTest->run();
      }

      rc.swapBuffers ();
      Sleep(10);
	}

   if(currentTest)
   {
      currentTest->deinit();
   }

   rc.destroySurface ();
   rc.destroy ();

	return 0;
}

