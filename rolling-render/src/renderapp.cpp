/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#include "renderapp.h"

namespace rolling {
	RenderApp::RenderApp()
		: m_FrameRate(0)
		, m_FrameInterval({ 0 })
		, m_Frequency({ 0 })
	{
		m_Window = std::make_unique<rolling::RenderWindow>();
	}

	RenderApp::~RenderApp()
	{
		m_Window.reset();
	}

	void RenderApp::Init()
	{
		InitFrequencyAndFrameRate();
		m_Window->Init();

		auto rect = m_Window->GetWindowRect();
		m_Scene.SetResolution(rect.GetWidth(), rect.GetHeight());

		auto renderFunc = [&](const rolling::Image<unsigned int>& image) {
			m_Window->Render(image);
			};
		m_Scene.SetRenderFunc(renderFunc);
	}

	void RenderApp::Render()
	{
		m_Scene.Render();
	}

	void RenderApp::Update()
	{
		m_Scene.Update();
		m_Window->Update();
	}

	void RenderApp::MainLoop()
	{
		int retCode = 0;

		LARGE_INTEGER nLast;
		LARGE_INTEGER nNow;

		QueryPerformanceCounter(&nLast);

		LONGLONG interval = 0LL;
		LONG waitMS = 0L;

		while (!m_Window->WindowShouldClose())
		{
			QueryPerformanceCounter(&nNow);
			interval = nNow.QuadPart - nLast.QuadPart;

			if (interval >= m_FrameInterval.QuadPart)
			{
				nLast.QuadPart = nNow.QuadPart;
				Update();
				Render();
			}
			else
			{
				waitMS = static_cast<LONG>((m_FrameInterval.QuadPart - interval) * 1000LL / m_Frequency.QuadPart - 1L);
				if (waitMS > 1L)
					Sleep(waitMS);
			}
		}
	}

	void RenderApp::SetFrameRate(int frameRate)
	{
		m_FrameRate = frameRate;
		m_FrameInterval.QuadPart = (LONGLONG)(1.0f / m_FrameRate * m_Frequency.QuadPart);
	}

	void RenderApp::InitFrequencyAndFrameRate()
	{
		QueryPerformanceFrequency(&m_Frequency);
		SetFrameRate(60);
	}

	Scene& RenderApp::GetScene()
	{
		return m_Scene;
	}
}