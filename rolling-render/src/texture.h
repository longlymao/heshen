/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

class Texture
{
public:
	Texture(int width, int height);
	~Texture();

	int GetWidth();
	int GetHeight();
	unsigned int* GetData();

	void Set(int x, int y, int color);
	int Get(int x, int y);

private:
	int width;
	int height;
	unsigned int* data;
};