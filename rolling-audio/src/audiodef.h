/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

namespace raudio {

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

};