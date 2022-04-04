#include "Render/RenderContext.h"
#include "Tests/FBO/FBOTest.h"

static GLuint g_pcProgramObject; // position, color shader
static GLuint g_pctProgramObject; // position, color, texture shader
static GLint g_textureLocation;

static GLuint g_fbo0;
static GLuint g_fbo1;
static GLuint g_fboColorTexture;
static GLuint g_fboDepthTexture;

static bool g_evenFrame = false;
static int g_displayable_width = 0;
static int g_displayable_height = 0;

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

    //rror
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
        "out vec4 frag_color;\n"
        "void main(){\n"
        "   frag_color = color;\n"
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

    IPL_TRACE_ERR_C("Creating pc shader");
    g_pcProgramObject = createShaderProgram(vPcShaderStr, fPcShaderStr);
    IPL_TRACE_ERR_C("Creating pct shader");
    g_pctProgramObject = createShaderProgram(vPctShaderStr, fPctShaderStr);
    g_textureLocation = glGetUniformLocation(g_pctProgramObject, "s_texture");

    if(g_pcProgramObject == 0 || g_pctProgramObject == 0)
    {
        return 0;
    }

    return 1;
}

int CreateFBO()
{
    // Create fbo
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    if(checkGLError("Error on fbo generation") != GL_NO_ERROR)
    {
       return 0;
    }

    // Create render target
    glGenTextures(1, &g_fboColorTexture);
    glBindTexture(GL_TEXTURE_2D, g_fboColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_displayable_width, g_displayable_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_fboColorTexture, 0);

    // Create depth texture
    glGenTextures(1, &g_fboDepthTexture);
    glBindTexture(GL_TEXTURE_2D, g_fboDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, g_displayable_width, g_displayable_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, g_fboDepthTexture, 0);

    if(checkGLError("Error on depth attach to fbo: ") != GL_NO_ERROR)
    {
        return 0;
    }

    // Allow render to texture
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        IPL_TRACE_ERR_C("Frame buffer not created");
        return 0;
    }

    return fbo;
}

int initDraw()
{
    if(initShaders() == 0)
    {
        return 0;
    }

    g_fbo0 = CreateFBO();
    if(g_fbo0 == 0)
    {
       return 0;
    }

    g_fbo1 = CreateFBO();
    if(g_fbo1 == 0)
    {
       return 0;
    }

    glDisable(GL_CULL_FACE);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 1;
}

void RenderTexturedQuad()
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

void RenderTriangles()
{
   //the vertex data
   static GLfloat vVertices1[] =
   {
      -0.5f, 0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f, 0.0f, -0.5f, 1.0f
   };

   //the vertex data
   static GLfloat vVertices2[] =
   {
      0.5f, 0.5f, 0.0f, 1.0f,
      -0.5f, 0.0f, -0.5f, 1.0f,
      0.5f, -0.5f, 0.0f, 1.0f
   };

   //the vertex colors
   static GLfloat vColors[] =
   {
      1.0f, 0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f, 1.0f
   };

   glEnable(GL_DEPTH_TEST);

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

   // Load the vertex data
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vVertices2);
   checkGLError("Render triangles: vertex pointer 0 - triangle 1");

   //draw a single triangle
   glDrawArrays(GL_TRIANGLES, 0, 3);
   checkGLError("Render triangles: draw triangle 1");

   glDisableVertexAttribArray(0);
   checkGLError("Render triangles: glDisableVertexAttribArray(0)");
   glDisableVertexAttribArray(1);
   checkGLError("Render triangles: glDisableVertexAttribArray(1)");
}

//renders a single frame
void draw()
{
   GLuint& fbo = g_evenFrame ? g_fbo0 : g_fbo1;
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

   glBindFramebuffer(GL_FRAMEBUFFER, fbo);
   checkGLError("Bind offscreen fbo");

   // Set the viewport
   glViewport(0, 0, g_displayable_width, g_displayable_height);

   // Clear the color buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   RenderTriangles();

   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   checkGLError("Bind onscreen fbo");
   glViewport(0, 0, g_displayable_width, g_displayable_height);

   glBindTexture(GL_TEXTURE_2D, g_fboDepthTexture);
   checkGLError("Bind fbo color texture");

   glDisable(GL_DEPTH_TEST);

   RenderTexturedQuad();

   g_evenFrame = !g_evenFrame;
}

void cleanupDraw()
{
   if(g_pcProgramObject)
   {
      glDeleteProgram(g_pcProgramObject);
      g_pcProgramObject = 0;
   }

   if(g_pctProgramObject)
   {
      glDeleteProgram(g_pctProgramObject);
      g_pctProgramObject = 0;
   }

   if(g_fbo0)
   {
      glDeleteFramebuffers(1, &g_fbo0);
      g_fbo0 = 0;
   }

   if(g_fbo1)
   {
      glDeleteFramebuffers(1, &g_fbo1);
      g_fbo1 = 0;
   }

   if(g_fboColorTexture)
   {
      glDeleteTextures(1, &g_fboColorTexture);
      g_fboColorTexture = 0;
   }

   if(g_fboDepthTexture)
   {
      glDeleteTextures(1, &g_fboDepthTexture);
      g_fboDepthTexture = 0;
   }
}

FBOTest::FBOTest(Director& director, const std::string& name)
: Test(director, name)
{

}

void FBOTest::init()
{
   Vector2f viewSize = mDirector.getViewSize();
   g_displayable_width = viewSize.x;
   g_displayable_height = viewSize.y;

   initDraw();
}

void FBOTest::deinit()
{
   cleanupDraw();
}

void FBOTest::run()
{
   draw();
}
