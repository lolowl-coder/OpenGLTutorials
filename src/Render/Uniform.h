#pragma once

#include <GLES3/gl3.h>

#include <string>

namespace Smart
{

	class Uniform
	{
	public:
		Uniform ()
		: m_index ( -1 )
		//, m_size ( 0 )
		//, m_type ( GL_NONE )
		{
		}

		Uniform ( GLint index, const std::string& name, GLint size, GLenum type )
		{
			m_index = index;
			//m_name = name;
			//m_size = size;
			//m_type = type;
		}

		GLint m_index;
		//std::string m_name;
		//GLint m_size;
		//GLenum m_type;
	};

}