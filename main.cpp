#include "GraphicsLib.h"

// wWinMain is essentially window's version of int main()
// _In_ means windows will pass information IN here, whereas _In_opt is optional
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    // open the Window using the new graphics library.
    if (!OpenWindow(L"HomeMadeGraphicsLibrary", 1280, 720))
    {
        return -1;
    }

    // "game" loop
    while (IsWindowRunning())
    {
        // Drawing code --START--

        // Clearing screen
        ClearScreen(COLOR_DARKGRAY);

        // Test square
        Square(100, 100, 200, COLOR_RED);

        // Drawing code --END--

        // Put pixels on screen AND handle window messages
        UpdateWindow();
    }

    // Clean up memory before closing
    CloseWindow();

    return 0;
}