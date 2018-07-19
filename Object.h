#ifndef OBJECT_H
#define OBJECT_H

#include "windows.h"

enum class Axis {Vertical, Horizontal};
bool IsMovingHorizontally();
bool IsMovingVertically();

class Object
{
public:
	Object(double x = 0, double y = 0, int w = 0, int h = 0, int c = 0x00FF0088)
		: x(x), y(y), width(w), height(h), color(c), parent_window(nullptr),
			horizontal_vel(0), vertical_vel(0) {}
	void Accelerate(Axis, double a = 1.0);
	void Decelerate(Axis, double a = 1.0);
	void Move();
	void MoveWithMouse(int x, int y);
	void Rebound();
	void Refresh();
	
	constexpr static double friction = 0.15;
	double x;
	double y;
	int width;
	int height;
	double horizontal_vel;
	double vertical_vel;
	int color;
	HWND parent_window;
};

#endif