/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <Windows.h>

namespace rrender {

	class MainWindow
	{
	public:
		MainWindow();
		~MainWindow();
		void Init();
		void Render();
		void Update();

	private:
		void RegisterWindowClass();
		void CreateWindowInstance();

	private:
		HWND hwnd;
	};

};