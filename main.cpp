#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define WINVER _WIN32_WINNT_WIN7

#include <iostream>
#include <cwchar>
#include <cmath>
#include "windows.h"
#include "windowsx.h"
#include "Object.h"
#include "Game.h"

Game game{};
Object object{75, 75, 100, 100};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MSG msg = {};
	LARGE_INTEGER freq, t;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t);
	double last_time = t.QuadPart * 1000.0 / freq.QuadPart;

	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			QueryPerformanceCounter(&t);
			double current_time = t.QuadPart * 1000.0 / freq.QuadPart;
			
			game.time = current_time - last_time;
			game.Update();
			game.Render();
			last_time = current_time;
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'R':
			object.x = 75.0;
			object.y = 75.0;
			object.horizontal_vel = 0;
			object.vertical_vel = 0;
		break;
		case VK_ESCAPE:
			DestroyWindow(hwnd);
		break;
		}
	break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}