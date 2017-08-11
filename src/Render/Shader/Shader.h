#pragma once

#include "Render/GLHeaders.h"

#include "vmath.h"

#include <string>
#include <vector>
#include <map>
#include <fstream>

class Shader
{
public:
   Shader ();
   void deinit ();
   bool load ( const std::string& vsFileName, const std::string& fsFileName );
   bool load ( const std::string& fileName, GLenum shaderType );
   bool link ();
   void bind ();
   void uniform1 ( int location, const int count, const float* value );
   void uniform1i ( int location, const int count, const int* value );
   void uniform2 ( int location, const int count, const Vector2f* value );
   void uniform2i ( int location, const int count, const Vector2i* value );
   void uniform3 ( int location, const int count, const Vector3f* value );
   void uniform4 ( int location, const int count, const Vector4f* value );
   void uniformMatrix4 ( int location, const int count, const Matrix4f* value );

   GLint uniformLocation ( const std::string& name );

private:
   bool loadSource ( std::ifstream& file, std::string& src );
   bool compileShader ( const std::string& src, GLuint& shader_id, GLenum shader_type );
   virtual void initUniforms ();

private:
   std::string mName;
   GLuint mProgramId;
   GLuint mVertexShaderId;
   GLuint mFragmentShaderId;
   bool mLinked;
};
