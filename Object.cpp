#include "Object.h"
#include "Game.h" 
#include <cmath>
#include <cwchar>

// |MAX_VEL| = 20

void Object::Accelerate()
{
	if (GetAsyncKeyState(VK_UP) < 0) {
		vertical_vel = vertical_vel - Game::acceleration * Game::time;
	}
	if (GetAsyncKeyState(VK_DOWN) < 0) {
		vertical_vel = vertical_vel + Game::acceleration * Game::time;
	}
	if (GetAsyncKeyState(VK_LEFT) < 0) {
		horizontal_vel = horizontal_vel - Game::acceleration * Game::time;
	}
	if (GetAsyncKeyState(VK_RIGHT) < 0) {
		horizontal_vel = horizontal_vel + Game::acceleration * Game::time;
	}
	
	if (vertical_vel > 20) vertical_vel = 20;
	if (vertical_vel < -20) vertical_vel = -20;
	if (horizontal_vel > 20) horizontal_vel = 20;
	if (horizontal_vel < -20) horizontal_vel = -20;
}

void Object::Draw()
{
	HBRUSH brush = CreateSolidBrush(color);
	HPEN pen = CreatePen(PS_NULL, 1, RGB(0,0,0));
	SelectObject(Game::memory_DC, pen);
	SelectObject(Game::memory_DC, brush);
	Rectangle(Game::memory_DC, x, y, x + width+1, y + height+1);
	DeleteObject((HGDIOBJ)brush);
	DeleteObject((HGDIOBJ)pen);
}

void Object::Friction()
{
	if (!Game::TheresVerticalInput() && vertical_vel != 0) { // Vertical friction
		if (vertical_vel >= Game::friction)
			vertical_vel -= Game::friction;
		else if (vertical_vel <= -Game::friction)
			vertical_vel += Game::friction;
		
		if (abs(vertical_vel) < Game::friction) vertical_vel = 0;
	}
	if (!Game::TheresHorizontalInput() && horizontal_vel != 0) { // Horizontal friction
		if (horizontal_vel >= Game::friction)
			horizontal_vel -= Game::friction;
		else if (horizontal_vel <= -Game::friction)
			horizontal_vel += Game::friction;
			
		if (abs(horizontal_vel) < Game::friction) horizontal_vel = 0;
	}
}

void Object::Move()
{
	if (Game::TheresHorizontalInput() || Game::TheresVerticalInput()) {
		x = x + horizontal_vel * Game::time + Game::acceleration * 0.5 * (Game::time * Game::time);
		y = y + vertical_vel * Game::time + Game::acceleration * 0.5 * (Game::time * Game::time);
	}
	else {
		x = x + horizontal_vel * Game::time;
		y = y + vertical_vel * Game::time;
	}
	Rebound();
}

void Object::MoveWithMouse(int mouse_x, int mouse_y)
{
	POINT cursor = {};
	GetCursorPos(&cursor);
	ScreenToClient(Game::main_window, &cursor);
	
	double dx =  cursor.x - (x);
	double dy =  cursor.y - (y);
	
	double newx = dx;
	double newy = dy;
	
	if (abs(newx) > 20.0) newx /= 20.0;
	if (abs(newy) > 20.0) newy /= 20.0;
	if (newx > 20) newx = 20;
	if (newy > 20) newy = 20;
	if (newx < -20) newx = -20;
	if (newy < -20) newy = -20;
	
	wchar_t str2[100];
	swprintf(str2, L"%.2f, %.2f", dx, dy);
	wchar_t str3[100];
	
	horizontal_vel = newx;
	vertical_vel = newy;	
	
	Rebound();
}

void Object::Rebound()
{
	if (x < 0) {
		x = 0;
		horizontal_vel = -horizontal_vel;
	}
	if (y < 0) {
		y = 0;
		vertical_vel = -vertical_vel;
	}
	RECT rect = {};
	GetClientRect(Game::main_window, &rect);
	if ((x+width) > rect.right) {
		x = rect.right - width;
		horizontal_vel = -horizontal_vel;
	}
	if ((y+height) > rect.bottom) {
		y = rect.bottom - height;
		vertical_vel = -vertical_vel;
	}
}