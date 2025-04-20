/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once
#include <vector>

namespace rolling {
	class ShaderBuffer {
	public:
		ShaderBuffer() = default;
		~ShaderBuffer() = default;

		ShaderBuffer(const ShaderBuffer&) = default;
		ShaderBuffer(ShaderBuffer&&) = default;
		ShaderBuffer& operator=(const ShaderBuffer&) = default;
		ShaderBuffer& operator=(ShaderBuffer&&) = default;

		template <typename T>
		void AddData(const T& data) {
			m_Data.resize(m_Data.size() + sizeof(T));
			memcpy(m_Data.data() + m_Data.size() - sizeof(T), &data, sizeof(T));
		}

		std::vector<char> m_Data;
	};
}