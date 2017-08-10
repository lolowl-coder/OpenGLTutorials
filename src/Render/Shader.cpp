#include "Platform/Log.h"
#include "Render/RenderContext.h"
#include "Render/Shader.h"
#include "Platform/Log.h"

Shader::Shader ()
: mProgramId(0)
, mLinked(false)
{
}

void Shader::deinit()
{
   if(mProgramId > 0)
   {
      glDeleteProgram(mProgramId);
      mProgramId = 0;
   }
}

bool Shader::loadSource(std::ifstream& file, std::string& src)
{
   std::string one_src_line;

   while(std::getline(file, one_src_line))
   {
      src += one_src_line + "\n";
   }

   return true;
}

bool Shader::compileShader(const std::string& src, GLuint& shader_id, GLenum shader_type)
{
   std::string log;
   GLint log_length;
   GLint compile_result;
   const char* src_ptr = src.c_str();
   std::string shader_type_str;
   GLint length;

   switch(shader_type)
   {
   case GL_VERTEX_SHADER:
      shader_type_str = "vertex";
      break;
   case GL_FRAGMENT_SHADER:
      shader_type_str = "fragment";
   case GL_COMPUTE_SHADER:
      shader_type_str = "compute";
      break;
   }

   if(src.empty())
   {
      logMsg("  source is empty");

      return false;
   }

   shader_id = glCreateShader(shader_type);

   length = static_cast<GLint>(src.length());

   glShaderSource(shader_id, 1, (const char**)&src_ptr, &length);
   checkGLError("glShaderSource");

   glCompileShader(shader_id);
   checkGLError("glCompileShader");

   glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result);
   if(!compile_result)
      logMsg("  Can't compile %s shader %s", shader_type_str.c_str(), mName.c_str());

   glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

   if(log_length > 1 && !compile_result)
   {
      log.resize(log_length);
      glGetShaderInfoLog(shader_id, log_length, 0, (char*)log.data ());
      logMsg("  Compilation log: %s", log.c_str());
   }

   if(!compile_result)
   {
      glDeleteShader(shader_id);

      return false;
   }

   return true;
}

void Shader::initUniforms()
{
   //LOG ( "  Initializing uniforms" );

   /*GLint uniforms_count;
   const int buffer_size = 256;
   char* uniform_name[buffer_size];
   std::string name;
   GLint size;
   GLenum type;
   GLint location;

   LOCK_SHADER;

   glGetProgramiv ( m_programId, GL_ACTIVE_UNIFORMS, &uniforms_count );
   for ( int i = 0; i < uniforms_count; i++ )
   {
   glGetActiveUniform ( m_programId, i, buffer_size, 0, &size
   , &type, (char*)&uniform_name[0] );
   CHECK_ERROR ( "Get active uniform" );
   location = glGetUniformLocation ( m_programId, (char*)&uniform_name[0] );
   CHECK_ERROR ( "Get uniform location" );
   name.assign ( (char*)&uniform_name[0] );
   //LOG ( "  Uniform: " << name << " location: " << location << " size: " << size );
   m_uniforms[name] = Uniform ( location, name, size, type );
   }*/

   //LOG ( "  Uniforms initialized" );
}

bool Shader::load(const std::string& vsFileName, const std::string& fsFileName)
{
   std::string vsSrc;
   std::string fsSrc;

   std::ifstream vsFile(vsFileName.c_str());

   if(!vsFile.is_open())
   {
      logMsg("Can't open shader source: %s", vsFileName.c_str());
      return false;
   }

   if(!loadSource(vsFile, vsSrc))
      return false;

   std::ifstream fsFile(fsFileName.c_str());

   if(!fsFile.is_open())
   {
      logMsg("Can't open shader source: %s", fsFileName.c_str());
      return false;
   }

   if(!loadSource(fsFile, fsSrc))
      return false;

   GLuint vertexShaderId;
   GLuint fragmentShaderId;

   if(!compileShader(vsSrc, vertexShaderId, GL_VERTEX_SHADER))
      return false;

   if(!compileShader(fsSrc, fragmentShaderId, GL_FRAGMENT_SHADER))
      return false;

   mProgramId = glCreateProgram();

   glAttachShader(mProgramId, vertexShaderId);
   checkGLError("attach vertex shader");
   glAttachShader(mProgramId, fragmentShaderId);
   checkGLError("attach fragment shader");

   link();

   glDeleteShader(vertexShaderId);
   checkGLError("delete vertex shader");
   glDeleteShader(fragmentShaderId);
   checkGLError("delete fragment shader");

   return true;
}

bool Shader::load(const std::string& fileName, GLenum shaderType)
{
   std::string src;

   std::ifstream f(fileName.c_str());

   if(!f.is_open())
   {
      logMsg("Can't open shader source: %s", fileName.c_str());
      return false;
   }

   if(!loadSource(f, src))
      return false;

   GLuint shaderId;

   if(!compileShader(src, shaderId, shaderType))
      return false;

   mProgramId = glCreateProgram();

   glAttachShader(mProgramId, shaderId);

   link();

   glDeleteShader(shaderId);

   return true;
}

bool Shader::link()
{
   GLint link_status;
   GLint log_length;
   std::string log;

   if(mLinked)
      return true;

   glLinkProgram(mProgramId);

   glGetProgramiv(mProgramId, GL_LINK_STATUS, &link_status);

   if(!link_status)
      logMsg("Can't link shader program");

   glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &log_length);

   if(log_length > 1 && !link_status)
   {
      log.resize(log_length);
      glGetProgramInfoLog(mProgramId, log_length, 0, (char*)log.data ());
      logMsg(log.c_str());
   }

   if(!link_status)
   {
      glDeleteProgram(mProgramId);

      return false;
   }

   initUniforms();

   mLinked = true;

   return true;
}

void Shader::bind()
{
   glUseProgram(mProgramId);
   checkGLError("Shader::bind");
}

GLint Shader::uniformLocation(const std::string& name)
{
   GLint result = glGetUniformLocation(mProgramId, name.c_str());

   if(result < 0)
   {
      logMsg("Uniform %s not found", name.c_str());
   }

   checkGLError("Shader::uniformLocation");

   return result;
}

void Shader::uniform1(int location, const int count, const float* value)
{
   if(location < 0)
      return;

   glUniform1fv(location, count, &value[0]);
   checkGLError("Set uniform 1");
}

void Shader::uniform1i(int location, const int count, const int* value)
{
   if(location < 0)
      return;

   glUniform1iv(location, count, &value[0]);
   checkGLError("Set int uniform 1");
}

void Shader::uniform2(int location, const int count, const Vector2f* value)
{
   if(location < 0)
      return;

   glUniform2fv(location, count, &value[0].x);
   checkGLError("Set uniform 2");
}

void Shader::uniform2i(int location, const int count, const Vector2i* value)
{
   if(location < 0)
      return;

   glUniform2iv(location, count, &value[0].x);
   checkGLError("Set int uniform 2");
}

void Shader::uniform3(int location, const int count, const Vector3f* value)
{
   if(location < 0)
      return;

   glUniform3fv(location, count, &value[0].x);
   checkGLError("Set uniform 3");
}

void Shader::uniform4(int location, const int count, const Vector4f* value)
{
   if(location < 0)
      return;

   glUniform4fv(location, count, &value[0].x);
   checkGLError("Set uniform 4");
}

void Shader::uniformMatrix4(int location, const int count, const Matrix4f* matrix)
{
   if(location < 0)
      return;

   glUniformMatrix4fv(location, count, false, &matrix[0].data[0]);
   checkGLError("Set uniform matrix 4x4");
}