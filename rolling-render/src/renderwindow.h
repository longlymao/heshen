/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <Windows.h>

namespace rrender {

	class RenderWindow
	{
	public:
		RenderWindow();
		~RenderWindow();

		void Init();
		void Render();
		void Update();

		bool WindowShouldClose();

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

		bool shouldClose = false;
		int exitCode = 0;

		int width = 800;
		int height = 600;
	};

};