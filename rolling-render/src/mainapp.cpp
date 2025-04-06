/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#include "mainapp.h"

namespace rrender {
	MainApp::MainApp()
		: m_FrameRate(0)
		, m_FrameInterval({ 0 })
		, m_Frequency({ 0 })
	{
		mainWindow = std::make_unique<rrender::MainWindow>();
	}

	MainApp::~MainApp()
	{
		mainWindow.reset();
	}

	void MainApp::Init()
	{
        InitFrequencyAndFrameRate();
		mainWindow->Init();
	}

	void MainApp::Render()
	{
		mainWindow->Render();
	}

	void MainApp::Update()
	{
		mainWindow->Update();
	}

	void MainApp::MainLoop()
	{
        int retCode = 0;
        bool shouldClose = false;

        LARGE_INTEGER nLast;
        LARGE_INTEGER nNow;

        QueryPerformanceCounter(&nLast);

        LONGLONG interval = 0LL;
        LONG waitMS = 0L;

        while (!shouldClose)
        {
            QueryPerformanceCounter(&nNow);
            interval = nNow.QuadPart - nLast.QuadPart;

            if (interval >= m_FrameInterval.QuadPart)
            {
                nLast.QuadPart = nNow.QuadPart;
				Update();
            }
            else
            {
                waitMS = static_cast<LONG>((m_FrameInterval.QuadPart - interval) * 1000LL / m_Frequency.QuadPart - 1L);
                if (waitMS > 1L)
                    Sleep(waitMS);
            }
        }
	}

    void MainApp::SetFrameRate(int frameRate)
    {
        m_FrameRate = frameRate;
        m_FrameInterval.QuadPart = (LONGLONG)(1.0f / m_FrameRate * m_Frequency.QuadPart);
    }

    void MainApp::InitFrequencyAndFrameRate()
    {
        QueryPerformanceFrequency(&m_Frequency);
		SetFrameRate(60);
    }
}