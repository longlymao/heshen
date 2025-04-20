/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <cstdint>

namespace rolling {

	enum class AudioChannel {
		MONO = 1,
		STEREO = 2,
	};

	enum class AudioSampleRate {
		SR_8000 = 8000,
		SR_11025 = 11025,
		SR_22050 = 22050,
		SR_44100 = 44100,
		SR_48000 = 48000,
	};

	enum class AudioSampleFormat {
		PCM_8 = 8,
		PCM_16 = 16,
		PCM_32 = 32,
	};

	enum RAU_WAVE_FORMAT : uint16_t {
		RAU_WAVE_FORMAT_PCM = 0x1,
		RAU_WAVE_FORMAT_ADPCM = 0x2,
		RAU_WAVE_FORMAT_IEEE_FLOAT = 0x3,
		RAU_WAVE_FORMAT_ALAW = 0x6,
		RAU_WAVE_FORMAT_MULAW = 0x7,
		RAU_WAVE_FORMAT_DVI_ADPCM = 0x11,
		RAU_WAVE_FORMAT_EXTENSIBLE = 0xFFFE,
	};
};