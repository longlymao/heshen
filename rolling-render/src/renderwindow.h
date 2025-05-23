/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

#if defined(near)
#undef near
#endif

#if defined(far)
#undef far
#endif

#include "image.h"

#define WIDTH 800 + 16
#define HEIGHT 600 + 40

namespace rolling {

	class RenderWindow
	{
	public:
		RenderWindow();
		~RenderWindow();

		void Init();
		void Render(const rolling::Image<unsigned int>& image);
		void Update();

		bool WindowShouldClose();

		rolling::Rect<int> GetWindowRect() const
		{
			return rolling::Rect<int>(0, 0, width, height);
		}
		void SetWindowRect(int w, int h) {
			width = w;
			height = h;
			rectDirty = true;
		}

	private:
		void RegisterWindowClass();
		void UnRegisterWindowClass();
		void CreateWindowInstance();
		void ReleaseWindowInstance();

		void CreateBitmap();
		void ReleaseBitmap();

		void ProcessInput();

	private:
		HWND hwnd = nullptr;
		HDC memhdc = nullptr;
		HBITMAP holdbmp = nullptr;
		HBITMAP hbmp = nullptr;
		UINT* pbmp = nullptr;

		bool rectDirty = true;;

		bool shouldClose = false;
		int exitCode = 0;

		int width = WIDTH;
		int height = HEIGHT;
	};

};