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

	virtual ~Shape() = default;
	virtual void Draw() = 0; // Pure virtual

};

class Square : public Shape
{
private:
	int m_size;

public:
	Square(int x, int y, int size, uint32_t color);
	void Draw() override;
};