#include "Object.h"
#include <cmath>
#include <cwchar>

void Object::Accelerate(Axis axis, double a)
{
	if (axis == Axis::Vertical && (vertical_vel < 20.0))
		vertical_vel += a;
	else if (axis == Axis::Horizontal && (horizontal_vel < 20.0))
		horizontal_vel += a;
}

void Object::Decelerate(Axis axis, double a)
{
	if (axis == Axis::Vertical && (vertical_vel > -20.0))
		vertical_vel -= a;
	else if (axis == Axis::Horizontal && horizontal_vel > -20.0)
		horizontal_vel -= a;
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
	GetClientRect(parent_window, &rect);
	if ((x+width) > rect.right) {
		x = rect.right - width;
		horizontal_vel = -horizontal_vel;
	}
	if ((y+height) > rect.bottom) {
		y = rect.bottom - height;
		vertical_vel = -vertical_vel;
	}
}

void Object::Move()
{
	Refresh();
	if (!IsMovingVertically() && vertical_vel != 0) { // Vertical friction
		if (vertical_vel >= friction)
			Decelerate(Axis::Vertical, friction);
		else if (vertical_vel <= -friction)
			Accelerate(Axis::Vertical, friction);
		
		if (abs(vertical_vel) < friction) vertical_vel = 0;
	}
	if (!IsMovingHorizontally() && horizontal_vel != 0) { // Horizontal friction
		if (horizontal_vel >= friction)
			Decelerate(Axis::Horizontal, friction);
		else if (horizontal_vel <= -friction)
			Accelerate(Axis::Horizontal, friction);
			
		if (abs(horizontal_vel) < friction) horizontal_vel = 0;
	}
	
	x += horizontal_vel;
	y += vertical_vel;
	
	Rebound();
	Refresh();
}

void Object::MoveWithMouse(int mouse_x, int mouse_y)
{
	POINT cursor = {};
	GetCursorPos(&cursor);
	ScreenToClient(parent_window, &cursor);
	
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
	swprintf(str3, L"%.2f, %.2f", newx, newy);
	HDC hdc = GetDC(parent_window);
	SetTextColor(hdc, RGB(255, 0, 0));
	
	TextOut(hdc, 10, 10, str2, wcslen(str2));
	TextOut(hdc, 10, 40, str3, wcslen(str3));
	
	ReleaseDC(parent_window, hdc);
	
	horizontal_vel = newx;
	vertical_vel = newy;	
	
	Rebound();
	Refresh();
}

void Object::Refresh()
{
	const RECT rect = {x, y, x + width, y + height};
	InvalidateRect(parent_window, &rect, true);
}

bool IsMovingHorizontally()
{	
	return ((GetKeyState(VK_LEFT) < 0) || (GetKeyState(VK_RIGHT) < 0));
}

bool IsMovingVertically()
{	
	return ((GetKeyState(VK_UP) < 0) || (GetKeyState(VK_DOWN) < 0));
}