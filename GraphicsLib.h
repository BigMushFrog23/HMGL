#pragma once
#include <Windows.h>
#include <stdint.h>

// Colors

const uint32_t COLOR_DARKGRAY = 0x111111;
const uint32_t COLOR_RED = 0xFF0000;
const uint32_t COLOR_GREEN = 0x00FF00;
const uint32_t COLOR_BLUE = 0x0000FF;

// struct defaults to public whereas class defaults to private.
// a buffer is a big chunk of memory essentially.
struct GraphicsBuffer
{
    int width; // width of drawing area
    int height; // height of drawing area
    uint32_t* pixels; // This is raw memory, we use uint32_t so it's exactly 32 bits (4 bytes). One pixel is 4 bytes (Red, Green, Blue, Alpha(opacity)) so this is perfect.
    BITMAPINFO bitmap_info; // Tell the OS that this is a 32-bit RGB image.
};

// global buffer for now to make things easier to start with,
// we make it extern so all files share the exact same memory buffer, no copies will be done.
extern GraphicsBuffer g_buffer;

// Function declarations
void ClearScreen(uint32_t color);
void PutPixel(int x, int y, uint32_t color);

// WINDOW API
bool OpenWindow(const wchar_t* title, int width, int height);
bool IsWindowRunning();
void UpdateWindow();
void CloseWindow();