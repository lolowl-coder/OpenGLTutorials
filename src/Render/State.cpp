#include "Smart/Base/Director.h"
#include "Smart/Base/State.h"
#include "Smart/Render/Capabilities.h"

namespace Smart
{

	State::State ()
	: m_blend ( false )
	, m_depthTest ( false )
	, m_depthMask ( true )
	, m_texture ( 0 )
	, m_activeTexture ( -1 )
	, m_shader ( 0 )
	, m_clearColor ( Vector4f ( 0, 0, 0, 1) )
	, m_blendSFactor ( MAX_BLEND_FACTOR )
	, m_blendDFactor ( MAX_BLEND_FACTOR )
	, m_blendEquation ( MAX_BLEND_EQUATION )
	, m_faceCull ( false )
	, m_frontFace ( FRONT )
	{

	}

	void State::blend ( bool value )
	{
		if ( m_blend == value )
			return;

		m_blend = value;

		if ( m_blend )
			glEnable ( GL_BLEND );
		else
			glDisable ( GL_BLEND );

		//CHECK_ERROR ( "Set blending" );
	}

	void State::depthTest ( bool value )
	{
		DIRECTOR;

		if ( director->view ()->renderContext ()->depthBits () == 0 )
			return;

		if ( m_depthTest == value )
			return;

		m_depthTest = value;

		if ( m_depthTest )
			glEnable ( GL_DEPTH_TEST );
		else
			glDisable ( GL_DEPTH_TEST );

		//CHECK_ERROR ( "Set depth test" );
	}

	void State::depthMask ( bool value )
	{
		if ( m_depthMask == value )
			return;

		m_depthMask = value;

		glDepthMask ( m_depthMask );
	}

	bool State::viewport ( const Vector2i& lt, const Vector2i& rb )
	{
		if ( m_viewportLT == lt && m_viewportRB == rb || rb.x == 0 || rb.y == 0 )
			return false;

		m_viewportLT = lt;
		m_viewportRB = rb;

		glViewport ( (int)m_viewportLT.x, (int)m_viewportLT.y
					, (int)m_viewportRB.x, (int)m_viewportRB.y );

		//CHECK_ERROR ( "Set viewport" );

		return true;
	}

	void State::clearColor ( const Vector4f& value )
	{
		if ( m_clearColor == value )
			return;

		m_clearColor == value;

		glClearColor ( m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a );
	}

	void State::bindTexture ( GLuint texture, int unit )
	{
		if ( m_activeTexture != unit )
		{
			m_activeTexture = unit;
			glActiveTexture ( active_texture[unit] );
			CHECK_ERROR ( "Active texture" );

			m_texture = texture;
			glBindTexture ( GL_TEXTURE_2D, m_texture );
			CHECK_ERROR ( "Bind texture" );
			//LOG ( "Bind texture: " << m_texture );

			return;
		}

		if ( m_texture == texture )
			return;

		m_texture = texture;
		glBindTexture ( GL_TEXTURE_2D, m_texture );
		CHECK_ERROR ( "Bind texture" );
	}

	void State::resetBoundTexture ()
	{
		m_texture = 0;
	}

	bool State::bindShader ( GLuint shader )
	{
		if ( m_shader == shader )
			return false;

		m_shader = shader;

		glUseProgram ( m_shader );

		//LOG ( "Bind shader: " << m_shader );

		CHECK_ERROR ( "Bind shader" );

		return true;
	}

	void State::resetBoundShader ()
	{
		m_shader = 0;
	}

	void State::blendFunc ( BlendFactor sfactor, BlendFactor dfactor )
	{
		if ( m_blendSFactor == sfactor && m_blendDFactor == dfactor )
			return;

		m_blendSFactor = sfactor;
		m_blendDFactor = dfactor;

		glBlendFunc ( blend_factor[m_blendSFactor], blend_factor[m_blendDFactor] );
	}

	void State::blendEquation ( BlendEquation value )
	{
		if ( m_blendEquation == value )
			return;

		m_blendEquation = value;
		glBlendEquation ( blend_equation[value] );
	}

	void State::faceCull ( bool value )
	{
		if ( m_faceCull == value )
			return;

		m_faceCull = value;

		if ( value )
			glEnable ( GL_CULL_FACE );
		else
			glDisable ( GL_CULL_FACE );
	}

	void State::frontFace ( FaceType value )
	{
		if ( m_frontFace == value )
			return;

		m_frontFace = value;

		glFrontFace ( face_type[value] );
	}

}
