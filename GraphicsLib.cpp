#include "GraphicsLib.h"
#include <string>
#include <cstdlib> // used for malloc/free

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
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // initialize windows cursor in case.
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

    size_t pixelsSizeInBytes = g_buffer.width * g_buffer.height * sizeof(uint32_t); // get amount of pixels, then multiply by the size of a 32-bit integer ( 4 bytes ) and get the total pixels converted to bytes.
    g_buffer.pixels = (uint32_t*)malloc(pixelsSizeInBytes); // translate memory into raw numbers (uint32_t*) and allocate the amount needed for the pixels.

    if (g_buffer.pixels == NULL)
    {
        return false; // don't run this if we can't get memory.
    }

    // setup BITMAPINFO for StretchDIBits
    // we essentially tell the RAM that we just allocated a 32 bit-image.
    g_buffer.bitmap_info.bmiHeader.biSize = sizeof(g_buffer.bitmap_info.bmiHeader);
    g_buffer.bitmap_info.bmiHeader.biWidth = g_buffer.width;
    g_buffer.bitmap_info.bmiHeader.biHeight = -g_buffer.height; // the negative height means coordinate (0,0) is at the top-left of the screen
    g_buffer.bitmap_info.bmiHeader.biPlanes = 1;
    g_buffer.bitmap_info.bmiHeader.biBitCount = 32;
    g_buffer.bitmap_info.bmiHeader.biCompression = BI_RGB;

    g_hdc = GetDC(g_hwnd); // get the canvas for the window.
    g_running = true;

    return true;
}

void DisplayFPS(int fps)
{
    static int lastFPS = -1;

    // only update if the FPS has actually changed and our window exists
    if (fps != lastFPS && g_hwnd != NULL)
    {
        lastFPS = fps;
        std::wstring title = L"HMGL Engine | FPS: " + std::to_wstring(fps);
        SetWindowTextW(g_hwnd, title.c_str()); // we use .c_str() to translate the wstring to a c_str, since the windows api was written before wstring existed, it doesn't know what it is.
    }
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
    free(g_buffer.pixels);
    ReleaseDC(g_hwnd, g_hdc);
    DestroyWindow(g_hwnd);
}

// we get the total pixels, then go through each one, and set it to the color (black)
// fast, but can probably be optimized further (to look at)
void ClearScreen(uint32_t color)
{
    int totalPixels = g_buffer.width * g_buffer.height;

    for (int i = 0; i < totalPixels; ++i)
    {
        g_buffer.pixels[i] = color;
    }
}

void PutPixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < g_buffer.width && y >= 0 && y < g_buffer.height) // check if we are inside the window / grid of pixels.
    {
        g_buffer.pixels[y * g_buffer.width + x] = color; // act as if the 2D array is a 1D array, allowing prefetch and batches.
    }
}