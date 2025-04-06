/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "texture.h"

namespace heshen
{
	struct Pos
	{
		int x;
		int y;
	};


	class LineTool
	{
	public:
		static void DrawLine(Texture* texture, Pos start, Pos end);
	};
}