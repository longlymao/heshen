/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <memory>
#include "mainwindow.h"

namespace rrender
{
	class MainApp {
	public:
		MainApp(); 
		~MainApp();

		void Init();
		void Render();
		void Update();

		void MainLoop();

		void SetFrameRate(int frameRate);

	private:
		void InitFrequencyAndFrameRate();

	private:
		std::unique_ptr<MainWindow> mainWindow;

		int m_FrameRate;
		LARGE_INTEGER m_FrameInterval;
		LARGE_INTEGER m_Frequency;
	};
}