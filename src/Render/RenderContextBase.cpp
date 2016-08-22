#include "Platform/Log.h"
#include "Render/RenderContextBase.h"

void checkGLError(const std::string& pre_msg)
{
   GLenum error_code = glGetError();
   if(error_code != GL_NO_ERROR)
   {
      switch(error_code)
      {
      case GL_INVALID_ENUM:
         logMsg("[%s] GL_INVALID_ENUM - An unacceptable value has been specified for an enumerated argument.  The offending function has been ignored.", pre_msg.c_str());
         break;
      case GL_INVALID_VALUE:
         logMsg("[%s] GL_INVALID_VALUE - A numeric argument is out of range.  The offending function has been ignored.", pre_msg.c_str());
         break;
      case GL_INVALID_OPERATION:
         logMsg("[%s] GL_INVALID_OPERATION - The specified operation is not allowed in the current state.  The offending function has been ignored.", pre_msg.c_str());
         break;
      case GL_OUT_OF_MEMORY:
         logMsg("[%s] GL_OUT_OF_MEMORY - There is not enough memory left to execute the function.  The state of OpenGL has been left undefined.", pre_msg.c_str());
         break;
      default:
         logMsg("[%s] Unknown GL error.  This should never happen.", pre_msg.c_str());
         break;
      }
   }
}

bool RenderContextBase::isExtensionSupported(const std::string& name)
{
   GLint n = 0;
   glGetIntegerv(GL_NUM_EXTENSIONS, &n);
   for(GLint i=0; i < n; i++)
   {
      const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
      if (!strcmp(name.c_str(), extension))
      {
         return true;
      }
   }

   return false;
}