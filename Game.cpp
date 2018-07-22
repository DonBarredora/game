#include "Game.h"
#include "Object.h"
#include <iostream>

extern Object object;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

double Game::acceleration = 0.02;
HWND Game::main_window = nullptr;
HDC Game::memory_DC = nullptr;
double Game::time = 0;
HBITMAP Game::hbmp = nullptr;

Game::Game()
{
	// Create main window
	PCWSTR class_name = L"MainWindowClass";
	WNDCLASSEX wnd_class = {};
	wnd_class.cbSize = sizeof(WNDCLASSEX);
	wnd_class.hInstance = GetModuleHandle(nullptr);
	wnd_class.lpfnWndProc = WindowProc;
	wnd_class.lpszClassName = class_name;
	wnd_class.hbrBackground = CreateSolidBrush(RGB(0, 255, 0));
	
	if (RegisterClassEx(&wnd_class) == 0) {
		std::cout << "Error al registrar clase MainWindowClass.\n";
		MessageBox(nullptr, L"Error al registrar clase MainWindowClass.", L"Error", MB_OK);
	}
	
	const int window_width = 800;
	const int window_height = 600;
	
	main_window = CreateWindowEx(0, class_name, L"Juego", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 				CW_USEDEFAULT, CW_USEDEFAULT, window_width,
			window_height, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	
	if (main_window == nullptr) {
		std::cout << "Error al crear ventana principal.\n";
		MessageBox(nullptr, L"Error al crear ventana principal.", L"Error", MB_OK);
	}
	
	// Create memory DC to store scene from window DC
	HDC window_DC = GetDC(main_window);
	memory_DC = CreateCompatibleDC(window_DC);
	hbmp = CreateCompatibleBitmap(window_DC, window_width, window_height);
	SelectObject(memory_DC, hbmp);
	BitBlt(memory_DC, 0, 0, window_width, window_height, window_DC, 0, 0, SRCCOPY);
	ReleaseDC(main_window, window_DC);
}

Game::~Game()
{
	DeleteObject((HGDIOBJ)hbmp);
	DeleteDC(memory_DC);
}

bool Game::TheresHorizontalInput()
{	
	return ((GetAsyncKeyState(VK_UP) < 0) || (GetAsyncKeyState(VK_DOWN) < 0));
}

bool Game::TheresVerticalInput()
{	
	return ((GetAsyncKeyState(VK_LEFT) < 0) || (GetAsyncKeyState(VK_RIGHT) < 0));
}

void Game::Render()
{
	// Paint background
	RECT rect = {};
	GetClientRect(main_window, &rect);
	FillRect(memory_DC, &rect, (HBRUSH)(BLACK_BRUSH));
	
	// Draw object
	object.Draw();
	
	// Blit from buffer to screen
	HDC window_DC = GetDC(main_window);
	BitBlt(window_DC, 0, 0, rect.right-rect.left, rect.bottom-rect.top, memory_DC, 0, 0, SRCCOPY);
	ReleaseDC(main_window, window_DC);
}

void Game::Update()
{
	object.Accelerate();
	object.Friction();
	object.Move();
	/*
	wchar_t window_title[200];
	swprintf(window_title, L"pos: (%.2f, %.2f); vel: (%.2f/%.2f); t: %f",
	object.x, object.y, object.horizontal_vel, object.vertical_vel, time);
	SetWindowText(main_window, window_title);*/
}