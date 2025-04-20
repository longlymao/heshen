/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <cmath>
#include <vector>

namespace rolling {
	template<typename T>
	class Rect
	{
	public:
		Rect()
			: x(0), y(0), width(0), height(0)
		{
		}
		Rect(T x, T y, T width, T height)
			: x(x), y(y), width(width), height(height)
		{
		}
		Rect(const Rect& other)
			: x(other.x), y(other.y), width(other.width), height(other.height)
		{
		}
		Rect& operator=(const Rect& other)
		{
			if (this != &other)
			{
				x = other.x;
				y = other.y;
				width = other.width;
				height = other.height;
			}
			return *this;
		}
		T GetWidth() const
		{
			return width;
		}
		T GetHeight() const
		{
			return height;
		}
	private:
		T x, y, width, height;
	};

	constexpr double PSNR_MAX = 100;

	template<typename T>
	class Image
	{
		std::vector<T> data;
		int width = 0;
		int height = 0;
	public:
		Image() 
		{
		}
		Image(int width, int height)
			: width(width), height(height)
		{
			data.resize(width * height);
		}
		~Image()
		{
		}
		Image(const Image& other)
			: width(other.width), height(other.height)
		{
			data = other.data;
		}
		Image& operator=(const Image& other)
		{
			if (this != &other)
			{
				width = other.width;
				height = other.height;
				data = other.data;
			}
			return *this;
		}
		T Get(int x, int y) const
		{
			return data[y * width + x];
		}
		void Set(int x, int y, T value)
		{
			if (x >= 0 && x < width && y >= 0 && y < height) {
				data[y * width + x] = value;
			}
		}
		int GetWidth() const
		{
			return width;
		}
		int GetHeight() const
		{
			return height;
		}
	};

	template<typename T>
	double psnr(const Image<T>& img1, const Image<T>& img2)
	{
		double mse = 0;
		for (int y = 0; y < img1.GetHeight(); y++)
		{
			for (int x = 0; x < img1.GetWidth(); x++)
			{
				double diff = img1.Get(x, y) - img2.Get(x, y);
				mse += diff * diff;
			}
		}
		mse /= img1.GetWidth() * img1.GetHeight();
		return mse ? 10 * log10(255 * 255 / mse) : PSNR_MAX;
	}
};