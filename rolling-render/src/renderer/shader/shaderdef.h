/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once


namespace rolling {
	enum class ShaderType
	{
		VERTEX,
		FRAGMENT,
		GEOMETRY,
		TESS_CONTROL,
		TESS_EVALUATION,
		COMPUTE,
	};

	enum class PrimitiveType
	{
		POINT,
		LINE,
		TRIANGLE,
		QUAD,
		CIRCLE,
		SQUARE,
	};
}