/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "matrix.h"
#include "image.h"
#include "shaderbuffer.h"
#include "base/color.h"

#include <algorithm>

namespace rolling {
	class LineShaderPipeLine {
	public:
		using ConstantBuffer = struct {
			rolling::Matrix4x4 modelViewProjection;
		};

		using VertexInput = struct {
			rolling::VectorF4 pos1;
			rolling::VectorF4 pos2;
			Color color1;
			Color color2;
		};

		using VertexOutput = struct {
			rolling::VectorF4 pos1;
			rolling::VectorF4 pos2;
			Color color1;
			Color color2;
		};

		using RasterizeInput = VertexOutput;

		using PixelInput = struct {
			Color color;
		};
		using PixelOutput = struct {
			Color color;
		};

		using OutputMergeInput = struct {
			rolling::VectorF4 pos;
			PixelOutput pixelOutput;
		};

		void ProcessBuffer(const ShaderBuffer& buffer, Image<unsigned>& image) {
			size_t curIndex = 0;
			if (buffer.m_Data.size() >= sizeof(ConstantBuffer)) {
				memcpy(&m_ConstantBuffer, buffer.m_Data.data(), sizeof(ConstantBuffer));
				curIndex += sizeof(ConstantBuffer);
			}
			while (curIndex < buffer.m_Data.size()) {
				VertexInput input;
				memcpy(&input, buffer.m_Data.data() + curIndex, sizeof(VertexInput));
				curIndex += sizeof(VertexInput);
				VertexOutput output = VertexShader(input);
				Rasterize(output, image);
			}
		}

		void SetupConstantBuffer(ConstantBuffer& cb) {
			m_ConstantBuffer.modelViewProjection = cb.modelViewProjection;
		}

		VertexOutput VertexShader(const VertexInput& input) {
			VertexOutput output;
			output.pos1 = m_ConstantBuffer.modelViewProjection * input.pos1;
			output.pos2 = m_ConstantBuffer.modelViewProjection * input.pos2;
			output.color1 = input.color1;
			output.color2 = input.color2;
			return output;
		}

		void Rasterize(const RasterizeInput& rasterizeInput, Image<unsigned>& image) {
			auto ToPixel = [&](VectorF4 pos, PixelInput pixelInput) {
					PixelOutput pixelOutput = PixelShader(pixelInput);

					OutputMergeInput outputMergeInput;
					outputMergeInput.pos = pos;
					outputMergeInput.pixelOutput = pixelOutput;
					OutputMerge(outputMergeInput, image);
				};

			int viewportWidth = image.GetWidth();
			int viewportHeight = image.GetHeight();

			auto ViewportTransformation = [&](VectorF4& pos) {
					pos /= pos[3];

					pos[0] = (pos[0] + 1) * 0.5 * viewportWidth;
					pos[1] = (pos[1] + 1) * 0.5 * viewportHeight;
				};

			RasterizeInput input = rasterizeInput;
			ViewportTransformation(input.pos1);
			ViewportTransformation(input.pos2);

			auto BresenhamX = [&](){
					int x1 = input.pos1[0];
					int y1 = input.pos1[1];
					int x2 = input.pos2[0];
					int y2 = input.pos2[1];

					int x = x1;
					int y = y1;

					int dh = y2 - y1;
					int dw = dh > 0 ? x2 - x1 : x1 - x2;
					int yStep = dh > 0 ? 1 : -1;

					int ee = 0;

					ToPixel({ x, y, 0, 1 }, { input.color1 });

					while (x < x2)
					{
						x++;
						if (abs(ee + dh * 2) < abs(dw * 2)) {
							ee += dh * 2;
						}
						else {
							y += yStep;
							ee += 2 * dh - 2 * dw;
						}

						float t = static_cast<float>(x - x1) / (x2 - x1);
						Color color = Color::Lerp(input.color1, input.color2, t);

						ToPixel({ x, y, 0, 1 }, { color });
					}
				};

			auto BresenhamY = [&]() {
					int x1 = input.pos1[0];
					int y1 = input.pos1[1];
					int x2 = input.pos2[0];
					int y2 = input.pos2[1];

					int x = x1;
					int y = y1;

					int dw = x2 - x1;
					int dh = dw > 0 ? y2 - y1 : y1 - y2;
					int xStep = dw > 0 ? 1 : -1;

					int ee = 0;

					ToPixel({ x, y, 0, 1 }, { input.color1 });

					while (y < y2)
					{
						y++;
						if (abs(ee + dw * 2) < abs(dh * 2)) {
							ee += dw * 2;
						}
						else {
							x += xStep;
							ee += 2 * dw - 2 * dh;
						}

						float t = static_cast<float>(y - y1) / (y2 - y1);
						Color color = Color::Lerp(input.color1, input.color2, t);

						ToPixel({ x, y, 0, 1 }, { color });
					}
				};


			if (abs(input.pos2[1] - input.pos1[1]) > abs(input.pos2[0] - input.pos1[0])) {
				if (input.pos1[1] > input.pos2[1]) {
					std::swap(input.pos1, input.pos2);
					std::swap(input.color1, input.color2);
				}
				BresenhamY();
			}
			else {
				if (input.pos1[0] > input.pos2[0]) {
					std::swap(input.pos1, input.pos2);
					std::swap(input.color1, input.color2);
				}
				BresenhamX();
			}
		}

		PixelOutput PixelShader(const PixelInput& input) {
			PixelOutput output;
			output.color = input.color;
			return output;
		}

		void OutputMerge(const OutputMergeInput& input, Image<unsigned>& image) {
			image.Set(input.pos[0], input.pos[1], input.pixelOutput.color.ToARGB());
		}

	private:
		ConstantBuffer m_ConstantBuffer;
	};
};