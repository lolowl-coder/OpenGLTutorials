#pragma once

#include "Render/Types.h"

#include <GLES2/gl2.h>

#include <string>
#include <vector>

namespace Smart
{

	class VertexAttribute
	{
	public:
		GLuint m_index;
		std::string m_name;
		DataType m_dataType;
		GLint m_size;
		void* m_offset;

		VertexAttribute ( GLuint index
						, const std::string& name
						, DataType data_type
						, GLint size
						, void* offset )
		: m_index ( index )
		, m_name ( name )
		, m_dataType ( data_type )
		, m_size ( size )
		, m_offset ( offset )
		{
		}

		void bind ( GLuint vertex_size ) const;
	};

	typedef std::vector<VertexAttribute> VertexAttributes;

}