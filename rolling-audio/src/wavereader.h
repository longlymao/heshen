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

	struct WavFmt {
		constexpr static size_t baseSize = 16;
		constexpr static uint16_t standardExtendedSize = 22;

		uint16_t formatTag = 0;
		uint16_t channels = 0;
		uint32_t sampleRate = 0;
		uint32_t avgBytesPerSec = 0;
		uint16_t blockAlign = 0;
		uint16_t bitsPerSample = 0;

		uint16_t extendedSize = 0;
		uint16_t validBitsPerSample = 0;
		uint32_t channelMask = 0;
		char subFormat[16] = { 0 };

		void Reset() {
			memset(this, 0, sizeof(WavFmt));
		}
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

	private:
		WavFmt fmt;
		std::vector<char> rawData;
	};
};