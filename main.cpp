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

static Object object(75, 75, 100, 100);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Draw();
void Update();

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{	
	PWSTR class_name = L"MainWindowClass";
	WNDCLASSEX wnd_class = {};
	wnd_class.cbSize = sizeof(WNDCLASSEX);
	wnd_class.hInstance = hInstance;
	wnd_class.lpfnWndProc = WindowProc;
	wnd_class.lpszClassName = class_name;
	wnd_class.hbrBackground = CreateSolidBrush(0x00333333);
	
	if (RegisterClassEx(&wnd_class) == 0)
	{
		std::cout << "Error al registrar clase MainWindowClass.\n";
		return 1;
	}
	
	object.parent_window = CreateWindowEx(0, class_name, L"Juego", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	if (object.parent_window == nullptr)
	{
		std::cout << "Error al crear ventana parent_window\n";
		return 1;
	}
		
	SetTimer(object.parent_window, 0, USER_TIMER_MINIMUM, nullptr);
	
	MSG msg = {};
	bool running = true;
	
	while (running) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Update();
			Draw();
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
		case VK_UP:
			object.Decelerate(Axis::Vertical);
		break;
		case VK_DOWN:
			object.Accelerate(Axis::Vertical);
		break;
		case VK_RIGHT:
			object.Accelerate(Axis::Horizontal);
		break;
		case VK_LEFT:
			object.Decelerate(Axis::Horizontal);
		break;
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
	case WM_MOUSEMOVE:
		if (wParam == MK_LBUTTON) {
			//object.MoveWithMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
	break;
	case WM_PAINT: {
		PAINTSTRUCT ps = {};
		HDC hdc = BeginPaint(hwnd, &ps);
		
		// Draw object
		HBRUSH brush = CreateSolidBrush(object.color);
		SelectObject(hdc, brush);
		const RECT rect = {object.x, object.y, object.x + object.width, object.y + object.height};
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
		
		EndPaint(hwnd, &ps);
		}
	break;
	case WM_TIMER: {
		/*POINT cursor = {};
		GetCursorPos(&cursor);
		ScreenToClient(hwnd, &cursor);
		wchar_t str[100];
		swprintf(str, L"pos: (%.2f, %.2f); vel: (%.2f/%.2f); mouse: (%i, %i)",
			object.x, object.y, object.horizontal_vel, object.vertical_vel, cursor.x, cursor.y);
		SetWindowText(hwnd, str);
			
		object.Move();*/
		}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void Update()
{
	const int FPS = 30;
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);
	QueryPerformanceCounter(&t2);
	
	/*wchar_t str[100];
	swprintf(str, L"freq: %lli; t1: %lli; t2: %lli; Dt: %llf", freq.QuadPart, t1.QuadPart, t2.QuadPart, (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart);
	SetWindowText(object.parent_window, str);
	*/
	object.Move();
}

void Draw()
{
	object.Refresh();
}