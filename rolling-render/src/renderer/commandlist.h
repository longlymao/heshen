/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "shader/shaderdef.h"
#include "shader/shaderbuffer.h"

namespace rolling {
	struct ShaderCommand {
		PrimitiveType primitiveType;
		ShaderBuffer buffer;
	};

	class CommandList {

	public:
		CommandList() = default;
		~CommandList() = default;
		CommandList(const CommandList&) = default;
		CommandList(CommandList&&) = default;
		CommandList& operator=(const CommandList&) = default;
		CommandList& operator=(CommandList&&) = default;

		template<PrimitiveType T>
		void AddCommand(const ShaderBuffer&& buffer) {
			ShaderCommand command;
			command.primitiveType = T;
			command.buffer = buffer;
			m_CommandList.push_back(command);
		}
		void AddCommand(const ShaderCommand&& command) {
			m_CommandList.push_back(command);
		}

		void Clear() {
			m_CommandList.clear();
		}

		std::vector<ShaderCommand> m_CommandList;
	};

};