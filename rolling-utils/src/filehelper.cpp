/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#include "filehelper.h"

namespace rolling {
	size_t GetRemainingSize(std::ifstream& stream)
	{
		if (!stream) {
			return 0;
		}
		auto pos = stream.tellg();
		stream.seekg(0, std::ios::end);
		auto size = stream.tellg();
		stream.seekg(pos);
		return size - pos;
	}
};