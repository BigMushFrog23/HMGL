#include "GraphicsLib.h"

GraphicsBuffer g_buffer;

// Internal variables hidden from the user
static HWND g_hwnd; // "Handle to a window" essentially is an ID to keep track of your specific window.
static HDC g_hdc; // "Handle to a context device" allows the user to visualize the content of the window.
static bool g_running = false;

// Window Procedure that handles closing the window.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE: case WM_DESTROY:
    {
        g_running = false; // we stop the loop if the user closes the window.
    } break;
    default: return DefWindowProc(hwnd, uMsg, wParam, lParam); // otherwise just handle it normally.
    }
    return 0;
}

bool OpenWindow(const wchar_t* title, int width, int height)
{
    HINSTANCE hInstance = GetModuleHandle(NULL); // asks windows for the memory address of our currently running program (hInstance)

    // create Window class.
    WNDCLASS wc = {}; // creates a blueprint for the window.
    wc.lpfnWndProc = WindowProc; // we link the event listener function to the blueprint.
    wc.hInstance = hInstance;
    wc.lpszClassName = L"HomeMadeGraphicsLibrary";
    RegisterClass(&wc); // we hand the blueprint over to the windows OS so it remembers it.

    // we ask for a drawing area, windows calculates how big the entire window needs to be to fit the drawing area perfectly.
    RECT r = { 0, 0, width, height };
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
    int window_width = r.right - r.left;
    int window_height = r.bottom - r.top;

    // open the Window using the calculated sizes.
    g_hwnd = CreateWindowEx(0, wc.lpszClassName, title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        window_width, window_height, 0, 0, hInstance, 0);

    if (!g_hwnd) return false; // if it fails to open, abort.

    g_buffer.width = width;
    g_buffer.height = height;

    // we ask the OS for Ram, allocating the memory needed for every pixel.
    g_buffer.pixels = (uint32_t*)VirtualAlloc(0, g_buffer.width * g_buffer.height * sizeof(uint32_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // setup BITMAPINFO for StretchDIBits
    // we essentially tell the RAM that we just allocated a 32 bit-image, and the negative height means coordiante (0,0) is at the top-left of the screen.
    g_buffer.bitmap_info.bmiHeader.biSize = sizeof(g_buffer.bitmap_info.bmiHeader);
    g_buffer.bitmap_info.bmiHeader.biWidth = g_buffer.width;
    g_buffer.bitmap_info.bmiHeader.biHeight = -g_buffer.height; // top-down
    g_buffer.bitmap_info.bmiHeader.biPlanes = 1;
    g_buffer.bitmap_info.bmiHeader.biBitCount = 32;
    g_buffer.bitmap_info.bmiHeader.biCompression = BI_RGB;

    g_hdc = GetDC(g_hwnd); // get the canvas for the window.
    g_running = true;

    return true;
}

bool IsWindowRunning() {
    return g_running;
}

void UpdateWindow()
{
    // handle window events
    MSG msg;
    // check for any events like mouse mouvement or such, otherwise remove the message to not freeze the window.
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // take the c++ array (g_buffer.pixels), look at the translation rules (&g_buffer.bitmap_info) and place the pixels on the window's canvas (g_hdc).
    StretchDIBits(g_hdc, 0, 0, g_buffer.width, g_buffer.height, 0, 0, g_buffer.width, g_buffer.height, g_buffer.pixels, &g_buffer.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
}

// return the RAM to the OS, return the canvas, and destroy the window.
// we do this to not cause a memory leak.
void CloseWindow()
{
    VirtualFree(g_buffer.pixels, 0, MEM_RELEASE); 
    ReleaseDC(g_hwnd, g_hdc);
    DestroyWindow(g_hwnd);
}

// drawing function

void ClearScreen(uint32_t color)
{
    uint32_t* pixel = g_buffer.pixels;
    for (int i = 0; i < g_buffer.width * g_buffer.height; ++i) // while we haven't traversed the window, add +1 to position.
    {
        *pixel++ = color; // slide along the memory instead of recalculating position everytime with pixels[i].
    }
}

void PutPixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < g_buffer.width && y >= 0 && y < g_buffer.height) // check if we are inside the window / grid of pixels.
    {
        g_buffer.pixels[y * g_buffer.width + x] = color; // 1D memory address we access skipping the width amount of pixels to get the correct height, giving us (y * width + x).
    }
}