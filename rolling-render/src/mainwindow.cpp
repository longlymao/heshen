/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#include "mainwindow.h"

#include <tchar.h>

#include "texture.h"
#include "line.h"

#define WIDTH 800
#define HEIGHT 600

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
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

namespace rrender {
	MainWindow::MainWindow()
		: hwnd(nullptr)
	{
		RegisterWindowClass();
		CreateWindowInstance();
	}

	MainWindow::~MainWindow()
	{
		if (hwnd)
		{
			DestroyWindow(hwnd);
			hwnd = nullptr;
		}
	}

	void MainWindow::Init()
	{
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);
	}

	void MainWindow::Render()
	{
	}

	void MainWindow::Update()
	{
		HDC winhdc = GetDC(hwnd);
		HDC memhdc = CreateCompatibleDC(winhdc);

		RECT rcWindow;
		GetClientRect(hwnd, &rcWindow);

		BITMAPINFOHEADER header = { 0 };

		header.biSize = sizeof(BITMAPINFOHEADER);
		header.biWidth = WIDTH;
		header.biHeight = HEIGHT;
		header.biPlanes = 1;
		header.biBitCount = 32;
		header.biCompression = BI_RGB;

		PVOID pvBits = NULL;
		HBITMAP bmp = CreateDIBSection(NULL, (PBITMAPINFO)&header,
			DIB_RGB_COLORS, &pvBits, NULL, 0);

		if (bmp == NULL)
		{
			DeleteDC(memhdc);
			ReleaseDC(hwnd, winhdc);
			return;
		}

		Texture texture(WIDTH, HEIGHT);
		heshen::LineTool::DrawLine(&texture, { 0, 0 }, { WIDTH - 1, HEIGHT - 101 });
		heshen::LineTool::DrawLine(&texture, { WIDTH, 0 }, { 0, HEIGHT - 101 });

		HGDIOBJ oldbmp = SelectObject(memhdc, bmp);
		UINT* data = static_cast<UINT*>(pvBits);
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				*data = texture.Get(x, y);
				data++;
			}
		}

		BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		POINT srcpos = { 0, 0 };
		POINT ptwinpos = { rcWindow.left, rcWindow.top };

		SIZE sizewindow = { WIDTH, HEIGHT };
		auto ret = UpdateLayeredWindow(hwnd, winhdc, &ptwinpos, &sizewindow, memhdc, &srcpos, 0, &blend, ULW_ALPHA);

		if (ret == 0)
		{
			auto err = GetLastError();
		}
		SelectObject(memhdc, oldbmp);
		DeleteObject(bmp);

		DeleteDC(memhdc);
		ReleaseDC(hwnd, winhdc);
	}

	void MainWindow::RegisterWindowClass()
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

	void MainWindow::CreateWindowInstance()
	{
		hwnd = CreateWindowEx(
			WS_EX_LAYERED,
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
};