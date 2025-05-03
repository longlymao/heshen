/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#include "renderwindow.h"

#include <tchar.h>
#include <assert.h>

#include "app/input/inputmanager.h"

#include <WinUser.h>

namespace rolling {
	static RenderWindow* g_MainWindow = nullptr;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		1 == VK_CONTROL;
		switch (uMsg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_ERASEBKGND:
			return TRUE;
		case WM_SIZE: {
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;
			if (g_MainWindow) {
				g_MainWindow->SetWindowRect(width, height);
			}
			return 0;
		}
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			KeyCode code = InputManager::TransWin32KeyToKeyCode(wParam);
			KeyEvent event = InputManager::TransWin32KeyEventToKeyEvent(uMsg);
			InputManager::GetInstance().ProcessWin32Event(code, event);
		}
		return 0;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		{
			KeyEvent event = InputManager::TransWin32KeyEventToKeyEvent(uMsg);
			POINT pt = { 0 };
			GetCursorPos(&pt);
			InputManager::GetInstance().HandleInputEvent(event, KeyCode::NONE, pt.x, pt.y);

		}
		return 0;
		case WM_MOUSEMOVE:
		{
			POINT pt = { LOWORD(lParam), HIWORD(lParam) };
			InputManager::GetInstance().HandleInputEvent(KeyEvent::MOUSE_MOVE, KeyCode::NONE, pt.x, pt.y);
		}
		return 0;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		{
			KeyEvent event = InputManager::TransWin32KeyEventToKeyEvent(uMsg);
			KeyCode code = KeyCode::NONE;
			switch (uMsg) {
			case WM_LBUTTONDOWN: code = KeyCode::MouseLeft; break;
			case WM_LBUTTONUP: code = KeyCode::MouseLeft; break;
			case WM_RBUTTONDOWN: code = KeyCode::MouseRight; break;
			case WM_RBUTTONUP: code = KeyCode::MouseRight; break;
			case WM_MBUTTONDOWN: code = KeyCode::MouseMiddle; break;
			case WM_MBUTTONUP: code = KeyCode::MouseMiddle; break;
			default:
				break;
			}
			POINT pt = { LOWORD(lParam), HIWORD(lParam) };
			InputManager::GetInstance().HandleInputEvent(event, code, pt.x, pt.y);
		}
		return 0;
		default:
			break;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	RenderWindow::RenderWindow()
	{
		g_MainWindow = this;
	}

	RenderWindow::~RenderWindow()
	{
		if (hwnd)
		{
			DestroyWindow(hwnd);
			hwnd = nullptr;
		}

		g_MainWindow = nullptr;
	}

	void RenderWindow::Init()
	{
		RegisterWindowClass();
		CreateWindowInstance();
		ShowWindow(hwnd, SW_SHOW);

		CreateBitmap();
	}

	void RenderWindow::Render(const rolling::Image<unsigned int>& image)
	{
		if (rectDirty) {
			ReleaseBitmap();
			CreateBitmap();
			rectDirty = false;
		}

		HDC hdc = GetDC(hwnd);

		UINT* data = pbmp;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				*data = image.Get(x, y);
				data++;
			}
		}

		BitBlt(hdc, 0, 0, width, height, memhdc, 0, 0, SRCCOPY);

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

		RECT clientRect;
		if (GetClientRect(hwnd, &clientRect)) {
			SetWindowRect(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
		}
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
		header.biWidth = width;
		header.biHeight = height;
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
		pbmp = nullptr;

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