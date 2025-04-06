/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#include "texture.h"
#include <memory.h>

Texture::Texture(int width, int height):
	width(width),
	height(height),
	data(nullptr)
{
	int size = width * height;
	if (size)
	{
		data = new unsigned int[size];
		memset(data, 0xFFFFFFFF, size * sizeof(int));
	}
}

Texture::~Texture()
{
	if (data)
	{
		delete data;
		data = nullptr;
	}
}

int Texture::GetWidth()
{
	return width;
}

int Texture::GetHeight()
{
	return height;
}

unsigned int* Texture::GetData()
{
	return data;
}

void Texture::Set(int x, int y, int color)
{
	data[y * width + x] = color;
}

int Texture::Get(int x, int y)
{
	return data[y * width + x];
}
