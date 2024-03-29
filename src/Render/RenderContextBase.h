#pragma once

#include "Render/GLHeaders.h"
#include <string>

GLenum checkGLError ( const std::string& pre_msg );

class RenderContextBase
{
public:
   virtual bool isExtensionSupported(const std::string& name);
};