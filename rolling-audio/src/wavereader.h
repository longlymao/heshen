/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "audiodef.h"
#include <cstdint>
#include <filesystem>
#include <fstream>

namespace raudio {
	enum class ChunkType {
		RIFF,
		FMT,
		DATA,
		UNKNOWN,
	};

	struct RiffHeader {
		char id[4] = { 0 };
		uint32_t size = 0;
		char format[4] = { 0 };
	};

	struct ChunkHeader {
		char id[4] = { 0 };
		uint32_t size = 0;
	};

	ChunkType GetChunkType(const ChunkHeader& chunk);

	class WaveReader
	{
	public:
		void Load(const std::filesystem::path& path);

	private:
		bool ReadRiff(std::ifstream& stream);
		bool ReadFmt(std::ifstream& stream, const ChunkHeader& chunk);
		bool ReadData(std::ifstream& stream, const ChunkHeader& chunk);
		bool ReadChunk(std::ifstream& stream, ChunkHeader& chunk);
	};
};