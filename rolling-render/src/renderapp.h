/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <memory>
#include "renderwindow.h"

#include "scene/scene.h"

namespace rrender
{
	class RenderApp {
	public:
		RenderApp();
		~RenderApp();

		void Init();
		void Render();
		void Update();

		void MainLoop();

		void SetFrameRate(int frameRate);

		Scene& GetScene();

	private:
		void InitFrequencyAndFrameRate();

	private:
		std::unique_ptr<RenderWindow> m_Window;

		int m_FrameRate;
		LARGE_INTEGER m_FrameInterval;
		LARGE_INTEGER m_Frequency;

		Scene m_Scene;
	};
}