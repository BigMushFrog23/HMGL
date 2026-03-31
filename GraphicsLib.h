#pragma once
#include <Windows.h>
#include <stdint.h>

// hardware abstraction layer
// talk to windows so the rest of the engine doesn't have to.

class Timer
{
private:
    int64_t m_frequency;
    int64_t m_lastTime;

    // FPS timer
    float m_fpsTimer = 0.0f;
    int m_frameCount = 0;
    int m_currentFPS = 0;

public:
    Timer()
    {
        // get CPU's last time frequency
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        m_frequency = freq.QuadPart;

        // initialize the first tick
        LARGE_INTEGER start;
        QueryPerformanceCounter(&start); // Measures CPU cycles instead of GetTickCount. (dt = ticks passed / frequency)
        m_lastTime = start.QuadPart;
    }

    float GetDeltaTime()
    {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        // calculate how many ticks passed since the last call
        int64_t timePassed = currentTime.QuadPart - m_lastTime;

        // update m_lastTime for the NEXT frame
        m_lastTime = currentTime.QuadPart;

        // convert ticks to seconds (float)
        float dt = (float)timePassed / (float)m_frequency;

        // get FPS
        m_fpsTimer += dt;
        m_frameCount++;

        if (m_fpsTimer >= 0.5f) // update every half second
        {
            m_currentFPS = (int)(m_frameCount / m_fpsTimer);
            m_fpsTimer = 0.0f;
            m_frameCount = 0;
        }

        return dt;
    }

    int GetFPS() const { return m_currentFPS; }
};

// Colors

const uint32_t COLOR_DARKGRAY = 0xFF111111;
const uint32_t COLOR_RED = 0xFFFF0000;
const uint32_t COLOR_GREEN = 0xFF00FF00;
const uint32_t COLOR_BLUE = 0xFF0000FF;
const uint32_t COLOR_TRANSPARENT = 0x00000000;

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

// FPS
void DisplayFPS(int fps);