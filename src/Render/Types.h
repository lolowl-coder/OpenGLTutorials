#pragma once

namespace Smart
{

	enum BlendFactor
	{
		ONE
	  , ZERO
	  , ONE_MINUS_SRC_ALPHA
	  , SRC_ALPHA
	  , SRC_COLOR
	  , DST_ALPHA
	  , DST_COLOR
	  , MAX_BLEND_FACTOR
	};

	enum BlendEquation
	{
		ADD
	  , SUB
	  , REVERSE_SUBTRACT
	  , MAX_BLEND_EQUATION
	};

	enum DataType
	{
		FLOAT
	  , SHORT
	  , UNSIGNED_SHORT
	  , INT
	  , UNSIGNED_INT
	  , BYTE
	  , UNSIGNED_BYTE
	  , MAX_DATA_TYPE
	};

	enum FaceType
	{
		FRONT
	  , BACK
	};

}