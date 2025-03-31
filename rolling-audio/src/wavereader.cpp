/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#include "wavereader.h"
#include "filehelper.h"

namespace raudio {
	ChunkType GetChunkType(const ChunkHeader& chunk)
	{
		if (chunk.id[0] == 'R' && chunk.id[1] == 'I' && chunk.id[2] == 'F' && chunk.id[3] == 'F') return ChunkType::RIFF;
		if (chunk.id[0] == 'f' && chunk.id[1] == 'm' && chunk.id[2] == 't' && chunk.id[3] == ' ') return ChunkType::FMT;
		if (chunk.id[0] == 'd' && chunk.id[1] == 'a' && chunk.id[2] == 't' && chunk.id[3] == 'a') return ChunkType::DATA;
		return ChunkType::UNKNOWN;
	}

	void WaveReader::Load(const std::filesystem::path& path)
	{
		if (std::ifstream file{ path, std::ios::binary }) {
			if (!ReadRiff(file)) return;
			ChunkHeader chunk;
			while (ReadChunk(file, chunk)) {
				ChunkType type = GetChunkType(chunk);
				switch (type) {
				case ChunkType::FMT:
					ReadFmt(file, chunk);
					break;
				case ChunkType::DATA:
					ReadData(file, chunk);
					break;
				default:
					file.seekg(chunk.size, std::ios::cur);
					break;
				}
			};
		}
	}

	bool WaveReader::ReadRiff(std::ifstream& stream)
	{
		RiffHeader riff;
		if (!stream.read(reinterpret_cast<char*>(&riff), sizeof(riff))) {
			return false;
		}
		if (riff.id[0] != 'R' || riff.id[1] != 'I' || riff.id[2] != 'F' || riff.id[3] != 'F') {
			return false;
		}
		if (riff.format[0] != 'W' || riff.format[1] != 'A' || riff.format[2] != 'V' || riff.format[3] != 'E') {
			return false;
		}
		if (rutils::GetRemainingSize(stream) + sizeof(riff.format) != riff.size) {
			return false;
		}
		return true;
	}

	bool raudio::WaveReader::ReadFmt(std::ifstream& stream, const ChunkHeader& chunk)
	{
		stream.seekg(chunk.size, std::ios::cur);
		return true;
	}

	bool raudio::WaveReader::ReadData(std::ifstream& stream, const ChunkHeader& chunk)
	{
		stream.seekg(chunk.size, std::ios::cur);
		return true;
	}

	bool raudio::WaveReader::ReadChunk(std::ifstream& stream, ChunkHeader& chunk)
	{
		if (!stream.read(reinterpret_cast<char*>(&chunk), sizeof(chunk))) {
			return false;
		}
		if (rutils::GetRemainingSize(stream) < chunk.size) {
			return false;
		}
		return true;
	}
};