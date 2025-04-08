/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "matrix.h"

namespace rrender
{
	class Line {
	public:
		void Render();
	private:
		VectorF3 pos1;
		VectorF3 pos2;
	};
}