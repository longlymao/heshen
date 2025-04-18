/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <cstdint>
#include <numbers>

namespace rrender {
	struct Color {
		enum BaseColor {
			RED = 0xFF0000,
			GREEN = 0x00FF00,
			BLUE = 0x0000FF,
			WHITE = 0xFFFFFF,
			BLACK = 0x000000,
			YELLOW = 0xFFFF00,
			CYAN = 0x00FFFF,
			MAGENTA = 0xFF00FF
		};

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		Color() : r(0), g(0), b(0), a(255) {}
		Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
			: r(red), g(green), b(blue), a(alpha) {
		}
		Color(uint32_t color) {
			r = (color >> 16) & 0xFF;
			g = (color >> 8) & 0xFF;
			b = color & 0xFF;
			a = (color >> 24) & 0xFF;
		}

		uint32_t ToARGB() const {
			return (a << 24) | (r << 16) | (g << 8) | b;
		}
	};
};