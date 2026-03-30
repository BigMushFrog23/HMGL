#include "Shape.h"
#include "GraphicsLib.h"

Square::Square(int x, int y, int size, uint32_t color) : Shape(x, y, color), m_size(size) {}

void Square::Draw()
{
    for (int y_size = 0; y_size < m_size; y_size++)
    {
        for (int x_size = 0; x_size < m_size; x_size++)
        {
            PutPixel(m_x + x_size, m_y + y_size, m_color);
        }
    }
}