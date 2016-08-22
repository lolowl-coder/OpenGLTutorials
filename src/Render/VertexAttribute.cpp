#include "Smart/Base/Director.h"
#include "Smart/Render/Types.h"
#include "Smart/Render/Capabilities.h"
#include "Smart/Macros/Macros.h"

#include "Log/Log.h"

namespace Smart
{

	void VertexAttribute::bind ( GLuint vertex_size ) const
	{
		//LOG ( "Binding attribute: " << m_index << ", " << m_name << ", " << m_size );
		glVertexAttribPointer ( m_index,
								m_size,
								data_type[m_dataType],
								GL_FALSE,
								vertex_size,
								m_offset );
		CHECK_ERROR ( "VertexAttribPointer: " + m_name );

		glEnableVertexAttribArray ( m_index );
		CHECK_ERROR ( "EnableVertexAttribArray: " + m_name );
	}

}