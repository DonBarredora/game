#ifndef GAME_H
#define GAME_H

#include "windows.h"

enum class Axis {Vertical, Horizontal};

class Game
{
public:
	Game();
	~Game();
	void Render();
	static bool TheresHorizontalInput();
	static bool TheresVerticalInput();
	void Update();
	
	static double acceleration;
	static constexpr double friction = 0.004;
	static HWND main_window;
	static HBITMAP hbmp;
	static HDC memory_DC;
	static double time;
};

#endif