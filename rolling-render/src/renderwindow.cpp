/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#include "renderwindow.h"

#include <tchar.h>
#include <assert.h>

#include "texture.h"
#include "line.h"

#define WIDTH 800
#define HEIGHT 600

namespace rrender {

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_ERASEBKGND:
			return TRUE;
		default:
			break;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	RenderWindow::RenderWindow()
	{
	}

	RenderWindow::~RenderWindow()
	{
		if (hwnd)
		{
			DestroyWindow(hwnd);
			hwnd = nullptr;
		}
	}

	void RenderWindow::Init()
	{
		RegisterWindowClass();
		CreateWindowInstance();
		ShowWindow(hwnd, SW_SHOW);

		CreateBitmap();
	}

	void RenderWindow::Render()
	{
		HDC hdc = GetDC(hwnd);

		Texture texture(WIDTH, HEIGHT);
		heshen::LineTool::DrawLine(&texture, { 0, 0 }, { WIDTH - 1, HEIGHT - 101 });
		heshen::LineTool::DrawLine(&texture, { WIDTH, 0 }, { 0, HEIGHT - 101 });

		UINT* data = pbmp;
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				*data = texture.Get(x, y);
				data++;
			}
		}

		BitBlt(hdc, 0, 0, WIDTH, HEIGHT, memhdc, 0, 0, SRCCOPY);

		ReleaseDC(hwnd, hdc);
	}

	void RenderWindow::Update()
	{
		ProcessInput();
	}

	void RenderWindow::ProcessInput()
	{
		MSG msg = { 0 };
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) {
				shouldClose = true;
				exitCode = static_cast<int>(msg.wParam);
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	bool RenderWindow::WindowShouldClose()
	{
		return shouldClose;
	}

	void RenderWindow::RegisterWindowClass()
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszClassName = _T("HeShenWindowClass");
		RegisterClassEx(&wc);
	}

	void RenderWindow::UnRegisterWindowClass()
	{
		if (GetModuleHandle(NULL))
		{
			UnregisterClass(_T("HeShenWindowClass"), GetModuleHandle(NULL));
		}
	}

	void RenderWindow::CreateWindowInstance()
	{
		hwnd = CreateWindowEx(
			0,
			_T("HeShenWindowClass"),
			_T("Hello, HeShen!"),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			WIDTH, HEIGHT,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL);
	}

	void RenderWindow::ReleaseWindowInstance()
	{
		if (hwnd)
		{
			DestroyWindow(hwnd);
			hwnd = nullptr;
		}
	}

	void RenderWindow::CreateBitmap()
	{
		HDC hdc = GetDC(hwnd);
		memhdc = CreateCompatibleDC(hdc);

		BITMAPINFOHEADER header = { 0 };
		header.biSize = sizeof(BITMAPINFOHEADER);
		header.biWidth = WIDTH;
		header.biHeight = HEIGHT;
		header.biPlanes = 1;
		header.biBitCount = 32;
		header.biCompression = BI_RGB;

		PVOID pvBits = NULL;
		hbmp = CreateDIBSection(NULL, (PBITMAPINFO)&header,
			DIB_RGB_COLORS, &pvBits, NULL, 0);
		
		if (hbmp) {
			holdbmp = (HBITMAP)SelectObject(memhdc, hbmp);
		}

		pbmp = static_cast<UINT*>(pvBits);

		ReleaseDC(hwnd, hdc);
	}

	void RenderWindow::ReleaseBitmap()
	{
		if (holdbmp && memhdc) {
			SelectObject(memhdc, holdbmp);
			holdbmp = nullptr;
		}
		if (hbmp) {
			DeleteObject(hbmp);
			hbmp = nullptr;
		}
		if (memhdc) {
			DeleteDC(memhdc);
			memhdc = nullptr;
		}
	}
};