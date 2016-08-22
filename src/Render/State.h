#pragma once

#include "Smart/Render/Shader/Shader.h"
#include "Smart/Render/Types.h"
#include "Smart/Resource/Texture.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace Smart
{

	class State
	{
	private:
		bool m_blend;
		bool m_depthTest;
		bool m_depthMask;
		GLuint m_texture;
		GLuint m_activeTexture;
		GLuint m_shader;
		Vector2f m_viewportLT;
		Vector2f m_viewportRB;
		Vector4f m_clearColor;
		BlendFactor m_blendSFactor;
		BlendFactor m_blendDFactor;
		BlendEquation m_blendEquation;
		bool m_faceCull;
		FaceType m_frontFace;

	public:
		State ();

		void blend ( bool value );

		void depthTest ( bool value );

		void depthMask ( bool value );

		bool viewport ( const Vector2i& lt, const Vector2i& rb );

		void clearColor ( const Vector4f& value );

		void bindTexture ( GLuint texture, int unit );
		void resetBoundTexture ();

		bool bindShader ( GLuint shader );
		void resetBoundShader ();

		void blendFunc ( BlendFactor sfactor, BlendFactor dfactor );
		void blendEquation ( BlendEquation value );

		void faceCull ( bool value );
		void frontFace ( FaceType value );
	};

}