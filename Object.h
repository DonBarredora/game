#ifndef OBJECT_H
#define OBJECT_H

#include "Game.h"
#include "windows.h"

class Object
{
public:
	Object(double x = 0, double y = 0, int w = 0, int h = 0, int c = RGB(255, 0, 0))
		: x(x), y(y), width(w), height(h), color(c), horizontal_vel(0),
			vertical_vel(0) {}
	void Accelerate();
	void Draw();
	void Friction();
	void Move();
	void MoveWithMouse(int x, int y);
	void Rebound();
	
	double x;
	double y;
	int width;
	int height;
	double horizontal_vel;
	double vertical_vel;
	int color;
};

#endif