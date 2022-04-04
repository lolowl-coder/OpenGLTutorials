#include "Render/RenderContext.h"
#include "Tests/MSAA/MSAATest.h"

#include <chrono>

#define USE_MSAA

namespace
{

   static GLuint g_pcProgramObject; // position, color shader
   static GLuint g_pctProgramObject; // position, color, texture shader
   static GLint g_textureLocation;

   static GLuint g_msaaFBO;
   static GLuint g_renderBuffer0;
   static GLuint g_renderBuffer1;

   static GLuint g_downsampledFBO;
   static GLuint g_fboColorTexture0;
   static GLuint g_fboColorTexture1;

   static int g_msaaSamplesCount = 4;

   static int g_displayable_width = 0;
   static int g_displayable_height = 0;

   static std::chrono::time_point<std::chrono::steady_clock> m_starttime;

   #    define IPL_TRACE_ERR_C(...)   do { printf("[ERROR] %s -- ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); } while(0)
   #    define IPL_TRACE_ERR_CV(...)  do { printf("[ERROR] %s -- ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); } while(0)
   #    define IPL_TRACE_TRACE_C(...)   do { printf("[TRACE] %s -- ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); } while(0)
   #    define IPL_TRACE_TRACE_CV(...)  do { printf("[TRACE] %s -- ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); } while(0)
   #    define IPL_TRACE_DEBUG_C(...)   do { printf("[DEBUG] %s -- ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); } while(0)
   #    define IPL_TRACE_DEBUG_CV(...)  do { printf("[DEBUG] %s -- ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); } while(0)
   #    define IPL_TRACE_FATAL_C(...)   do { printf("[FATAL] %s -- ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); } while(0)
   #    define IPL_TRACE_FATAL_CV(...)  do { printf("[FATAL] %s -- ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); } while(0)
   #    define IPL_TRACE_INFO_C(...)  do { printf("[INFO] %s -- ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); } while(0)
   #    define IPL_TRACE_INFO_CV(...) do { printf("[INFO] %s -- ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); } while(0)
   #    define IPL_TRACE_LVL_C(...)  do { printf("[LVL] %s -- ", __FUNCTION__); printf("\n"); } while(0)
   #    define IPL_TRACE_LVL_CV(...) do { printf("[LVL] %s -- ", __FUNCTION__); printf("\n"); } while(0)


   GLuint loadShader(GLenum type, const char *shaderSrc)
   {
         GLuint shader = 0;
         GLint compiled;

         // Create the shader object
         shader = glCreateShader(type);

         if(shader == 0) {
            IPL_TRACE_ERR_C("failed to create shader object");
            return 0;
         }

         // Load the shader source
         glShaderSource(shader, 1, &shaderSrc, NULL);

         // Compile the shader
         glCompileShader(shader);

         // Check the compile status
         glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

         //if compile status is not ok...
         if(!compiled)
         {
            GLint infoLen = 0;

            //...retrieve some more infos about the issue
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            IPL_TRACE_ERR_C("Error compiling shader");

            if(infoLen > 1)
            {
               char* infoLog = (char*)malloc(sizeof(char) * infoLen);

               glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
               IPL_TRACE_ERR_CV("Error compiling shader:\n%s\n", infoLog);

               free(infoLog);
            }

            //...and leave
            return 0;
         }

         return shader;
   }

   GLuint createShaderProgram(const char* vs, const char* fs)
   {
         GLuint vertexShader;
         GLuint fragmentShader;
         GLuint programObject = 0;
         GLint linked;

         // Load the vertex/fragment shaders
         vertexShader = loadShader(GL_VERTEX_SHADER, vs);
         fragmentShader = loadShader(GL_FRAGMENT_SHADER, fs);

         // Create the program object
         programObject = glCreateProgram();

         // Check error
         if(programObject == 0)
         {
            IPL_TRACE_ERR_CV("Error creating shader object (%i)", glGetError());
            return 0;
         }

         //attach the two shaders
         glAttachShader(programObject, vertexShader);
         glAttachShader(programObject, fragmentShader);

         // Bind vPosition to attribute 0
         glBindAttribLocation(programObject, 0, "vPosition");
         // Bind color to attribute 1
         glBindAttribLocation(programObject, 1, "vColor");
         // Bind tc to attribute 2
         glBindAttribLocation(programObject, 2, "vTC");

         // Link the program
         glLinkProgram(programObject);

         checkGLError("Link shader");

         // Check the link status
         glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

         //if some linker error occured...
         if(!linked)
         {
            GLint infoLen = 0;

            glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
            IPL_TRACE_ERR_C("Error linking shader");

            //...retrieve some more infos about the issue
            if(infoLen > 1)
            {
               char *infoLog = (char*)malloc(sizeof(char) * infoLen);

               glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
               IPL_TRACE_ERR_CV("Error linking program:\n%s\n", infoLog);

               free(infoLog);
            }
         }

         glDeleteShader(vertexShader);
         vertexShader = 0;
         glDeleteShader(fragmentShader);
         fragmentShader = 0;

         if(!linked)
         {
            glDeleteProgram(programObject);
            programObject = 0;
         }

         return programObject;
   }

   int initShaders()
   {
         //the vertex shader source
         char vPcShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec4 vPosition;\n"
            "in vec4 vColor;\n"
            "out vec4 color;\n"
            "void main(){\n"
            "   color = vColor;\n"
            "   gl_Position = vPosition;\n"
            "}\n";

         //the fragment shader source
         char fPcShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec4 color;\n"
            "layout(location = 0) out vec4 frag_color0;\n"
            "layout(location = 1) out vec4 frag_color1;\n"
            "void main(){\n"
            "   frag_color0 = color;\n"
            "   frag_color1 = vec4(1.0, 0.0, 0.0, 1.0);\n"
            "}\n";

         char vPctShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec4 vPosition;\n"
            "in vec4 vColor;\n"
            "in vec2 vTC;\n"
            "out vec4 color;\n"
            "out vec2 tc;\n"
            "void main(){\n"
            "   color = vColor;\n"
            "	tc = vTC;\n"
            "   gl_Position = vPosition;\n"
            "}\n";

         //the fragment shader source
         char fPctShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "uniform sampler2D s_texture;"
            "in vec4 color;\n"
            "in vec2 tc;\n"
            "out vec4 frag_color;\n"
            "void main(){\n"
            "   frag_color = texture(s_texture, tc) * color;\n"
            "}\n";

         IPL_TRACE_INFO_C("Creating pc shader");
         g_pcProgramObject = createShaderProgram(vPcShaderStr, fPcShaderStr);
         IPL_TRACE_INFO_C("Creating pct shader");
         g_pctProgramObject = createShaderProgram(vPctShaderStr, fPctShaderStr);
         g_textureLocation = glGetUniformLocation(g_pctProgramObject, "s_texture");

         if(g_pcProgramObject == 0 || g_pctProgramObject == 0)
         {
            return 0;
         }

         return 1;
   }

   GLuint createAndAttachRenderBuffer(GLenum attachment)
   {
      GLuint renderBuffer = 0;
      glGenRenderbuffers(1, &renderBuffer);
      glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
      if(checkGLError("createAndAttachRenderBuffer(), glBindRenderbuffer") != GL_NO_ERROR)
      {
         renderBuffer = 0;
      }
      GLenum internalFormat = attachment == GL_DEPTH_ATTACHMENT ? GL_DEPTH_COMPONENT24 : GL_RGBA8;
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, g_msaaSamplesCount, internalFormat, g_displayable_width, g_displayable_height);
      if(checkGLError("createAndAttachRenderBuffer(), glRenderbufferStorageMultisample") != GL_NO_ERROR)
      {
         renderBuffer = 0;
      }
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer);
      if(checkGLError("createAndAttachRenderBuffer(), glFramebufferRenderbuffer") != GL_NO_ERROR)
      {
         renderBuffer = 0;
      }

      return renderBuffer;
   }

   GLuint genFBO()
   {
      GLuint fbo = 0;
      glGenFramebuffers(1, &fbo);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);
      if(checkGLError("Error on fbo generation") != GL_NO_ERROR)
      {
         return 0;
      }

      return fbo;
   }

   GLuint createMSAAFBO()
   {
      // Create MSAA fbo
      GLuint fbo = genFBO();
      if(fbo == 0)
      {
         return 0;
      }

      // setup MSAA color attachments
      g_renderBuffer0 = createAndAttachRenderBuffer(GL_COLOR_ATTACHMENT0);
      if(g_renderBuffer0 == 0)
      {
         return 0;
      }

      g_renderBuffer1 = createAndAttachRenderBuffer(GL_COLOR_ATTACHMENT1);
      if(g_renderBuffer1 == 0)
      {
         return 0;
      }

      GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
      glDrawBuffers(2, DrawBuffers);

      GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if(status != GL_FRAMEBUFFER_COMPLETE)
      {
         IPL_TRACE_ERR_C("MSAA FBO not created. Status: %#x", status);
         return 0;
      }

      return fbo;
   }

   GLuint createAndAttachTexture(GLenum attachment)
   {
      GLuint texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      GLenum internalFormat = attachment == GL_DEPTH_ATTACHMENT ? GL_DEPTH_COMPONENT24 : GL_RGBA8;
      GLenum format = attachment == GL_DEPTH_ATTACHMENT ? GL_DEPTH_COMPONENT : GL_RGBA;
      glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, g_displayable_width, g_displayable_height, 0, format, GL_UNSIGNED_BYTE, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
      if(checkGLError("Error on texture attachment setup") != GL_NO_ERROR)
      {
         return 0;
      }

      return texture;
   }

   GLuint createDownsampledFBO()
   {
      // Create MSAA fbo
      GLuint fbo = genFBO();
      if(fbo == 0)
      {
         return 0;
      }

      g_fboColorTexture0 = createAndAttachTexture(GL_COLOR_ATTACHMENT0);
      if(g_fboColorTexture0 == 0)
      {
         return 0;
      }

      g_fboColorTexture1 = createAndAttachTexture(GL_COLOR_ATTACHMENT1);
      if(g_fboColorTexture1 == 0)
      {
         return 0;
      }

      GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
      glDrawBuffers(2, DrawBuffers);

      GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if(status != GL_FRAMEBUFFER_COMPLETE)
      {
         IPL_TRACE_ERR_C("Frame buffer not created. Status: %#x", status);
         return 0;
      }

      return fbo;
   }

   int initDraw()
   {
      m_starttime = std::chrono::steady_clock::now();

      if(initShaders() == 0)
      {
         return 0;
      }

      g_msaaFBO = createMSAAFBO();
      if(g_msaaFBO == 0)
      {
         return 0;
      }

      g_downsampledFBO = createDownsampledFBO();
      if(g_downsampledFBO == 0)
      {
         return 0;
      }

      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);

      return 1;
   }

   void renderTexturedQuad(GLuint texture)
   {
      static const GLfloat quadVertices[] =
      {
         -1.0f, -1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f,
         -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f, 1.0f,
         -1.0f,  1.0f, 0.0f, 1.0f
      };
      static const GLfloat quadColors[] =
      {
         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f, 1.0f
      };
      static const GLfloat quadTexCoords[] =
      {
         0.0f, 0.0f,
         1.0f, 0.0f,
         0.0f, 1.0f,
         1.0f, 0.0f,
         1.0f, 1.0f,
         0.0f, 1.0f
      };

      glBindTexture(GL_TEXTURE_2D, texture);

      // Use the program object
      glUseProgram(g_pctProgramObject);

      glUniform1i(g_textureLocation, 0);

      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, quadVertices);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, quadColors);
      glEnableVertexAttribArray(1);

      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, quadTexCoords);
      glEnableVertexAttribArray(2);

      //draw a single triangle
      glDrawArrays(GL_TRIANGLES, 0, 6);

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glDisableVertexAttribArray(2);
   }

   void renderTriangle()
   {
      //the vertex data
      static GLfloat vVertices1[] =
      {
         -0.5f, 0.5f, 0.0f, 1.0f,
         -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, 0.0f, -0.5f, 1.0f
      };

      //the vertex colors
      static GLfloat vColors[] =
      {
         1.0f, 0.0f, 0.0f, 1.0f,
         0.0f, 1.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f, 1.0f
      };

      glBindTexture(GL_TEXTURE_2D, 0);

      // Use the program object
      glUseProgram(g_pcProgramObject);
      checkGLError("Render triangles: use shader program");
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      checkGLError("Render triangles: bind 0 vbo");
      // Load the vertex data
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vVertices1);
      checkGLError("Render triangles: vertex pointer 0 - triangle 0");
      glEnableVertexAttribArray(0);
      checkGLError("Render triangles: glEnableVertexAttribArray(0)");

      glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vColors);
      checkGLError("Render triangles: vertex pointer 1");
      glEnableVertexAttribArray(1);
      checkGLError("Render triangles: glEnableVertexAttribArray(1)");

      //draw a single triangle
      glDrawArrays(GL_TRIANGLES, 0, 3);
      checkGLError("Render triangles: draw triangle 0");

      glDisableVertexAttribArray(0);
      checkGLError("Render triangles: glDisableVertexAttribArray(0)");
      glDisableVertexAttribArray(1);
      checkGLError("Render triangles: glDisableVertexAttribArray(1)");
   }

   void blitBuffer(GLenum attachment)
   {
      char msg[35];
      glReadBuffer(attachment);
      sprintf(msg, "Set read buffer. Attachment %d", attachment - GL_COLOR_ATTACHMENT0);
      checkGLError(msg);
      sprintf(msg, "Set draw buffers. Attachment %d", attachment - GL_COLOR_ATTACHMENT0);
      glDrawBuffers(1, &attachment);
      checkGLError(msg);

      glBlitFramebuffer(
         // src area covers whole FBO
         0, 0, g_displayable_width, g_displayable_height,
         // dst area cover whole FBO
         0, 0,g_displayable_width, g_displayable_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

      sprintf(msg, "glBlitFramebuffer. Attachment %d", attachment - GL_COLOR_ATTACHMENT0);
      checkGLError(msg);
   }

   //renders a single frame
   void draw()
   {
      #if defined(USE_MSAA)
         glBindFramebuffer(GL_FRAMEBUFFER, g_msaaFBO);
         checkGLError("Bind offscreen fbo");
         GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
         glDrawBuffers(2, DrawBuffers);
         checkGLError("Set draw buffers");

         glViewport(0, 0, g_displayable_width, g_displayable_height);
         glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

         renderTriangle();

         glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g_downsampledFBO);
         checkGLError("Bind downsampled FBO as draw buffer.");

         glBindFramebuffer(GL_READ_FRAMEBUFFER, g_msaaFBO);
         checkGLError("Bind msaa FBO as read buffer.");

         blitBuffer(GL_COLOR_ATTACHMENT0);
         blitBuffer(GL_COLOR_ATTACHMENT1);
      #else
         glBindFramebuffer(GL_FRAMEBUFFER, g_downsampledFBO);
         checkGLError("Bind offscreen fbo");
         GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
         glDrawBuffers(2, DrawBuffers);
         checkGLError("Set draw buffers");

         glViewport(0, 0, g_displayable_width, g_displayable_height);
         glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

         renderTriangle();
      #endif

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      checkGLError("Bind onscreen fbo");

      const GLenum drawBuffer = GL_BACK;
      glDrawBuffers(1, &drawBuffer);
      checkGLError("Set draw buffers onscreen");

      glReadBuffer(GL_BACK);
      checkGLError("Set read buffer onscreen");

      glClear(GL_COLOR_BUFFER_BIT);
      glViewport(0, 0, g_displayable_width, g_displayable_height);

      unsigned int t = ((std::chrono::steady_clock::now() - m_starttime).count()) * 1000 * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
      GLuint colorTexture = ((t % 2000) / 2000.0f) > 0.5 ? g_fboColorTexture0 : g_fboColorTexture1;
      renderTexturedQuad(colorTexture);
   }

   void deleteShader(GLuint &shader)
   {
      if(shader != 0)
      {
         glDeleteProgram(shader);
         shader = 0;
      }
   }

   void deleteFBO(GLuint &fbo)
   {
      if(fbo != 0)
      {
         glDeleteFramebuffers(1, &fbo);
         fbo = 0;
      }
   }

   void deleteRenderBuffer(GLuint &renderBuffer)
   {
      if(renderBuffer != 0)
      {
         glDeleteRenderbuffers(1, &renderBuffer);
         renderBuffer = 0;
      }
   }

   void deleteTexture(GLuint &texture)
   {
      if(texture != 0)
      {
         glDeleteTextures(1, &texture);
         texture = 0;
      }
   }

   void cleanupDraw()
   {
      deleteShader(g_pcProgramObject);
      deleteShader(g_pctProgramObject);

      deleteFBO(g_msaaFBO);
      deleteRenderBuffer(g_fboColorTexture0);
      deleteRenderBuffer(g_fboColorTexture1);

      deleteFBO(g_downsampledFBO);
      deleteTexture(g_fboColorTexture0);
      deleteTexture(g_fboColorTexture1);
   }

}

MSAATest::MSAATest(Director& director, const std::string& name)
: Test(director, name)
{

}

void MSAATest::init()
{
   Vector2f viewSize = mDirector.getViewSize();
   g_displayable_width = viewSize.x;
   g_displayable_height = viewSize.y;

   initDraw();
}

void MSAATest::deinit()
{
   cleanupDraw();
}

void MSAATest::run()
{
   draw();
}
