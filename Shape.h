#pragma once
#include <stdint.h>

class Shape
{
protected:
	int m_x;
	int m_y;
	uint32_t m_color;

public:
	
	Shape(int x, int y, uint32_t color) : m_x(x), m_y(y), m_color(color) {}

	// virtual means that Shape might be replaced by a child class later, so use the child's functions.
	virtual ~Shape() = default; // ~Shape() = default is a destructor, it is run before the object is deleted to clean up. default writes a basic empty destructor for it.
	virtual void Draw() = 0; // Pure virtual, Shape has no Draw code, so it will be forced to instead be a child class that has Draw code.

};

class Square : public Shape
{
private:
	int m_size;

public:
	Square(int x, int y, int size, uint32_t color);
	void Draw() override; // Use this Class' Draw function instead of the parent Shape, replacing the virtual Draw.
};