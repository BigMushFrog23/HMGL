#include "GraphicsLib.h"
#include "Registry.h"
#include "Systems.h"
#include "InputManager.h"
#include "EntityBuilder.h"

// main file
// file that directs everything.

// wWinMain is essentially window's version of int main()
// _In_ means windows will pass information IN here, whereas _In_opt is optional
// Using _In_ or _In_opt_ is optional, just used to be very precise and make the IDE not put a warning.
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    OpenWindow(L"Game Engine Test", 1280, 720);

    Registry registry;
    AssetManager assets;
    InputManager input;
    Timer timer;

    // create the texture of an entity
    assets.CreateColorSquare("HeroTex", 50, COLOR_RED);

    // create an entity
    // we assign the values needed to this new entity between: position, sprite, kinematic values, and controller id. Then build it.
    Entity player = EntityBuilder(registry, assets)
        .WithTransform(100.0f, 100.0f)
        .WithSprite("HeroTex")
        .WithKinematics(300.0f)
        .WithController(1)
        .Build();

    while (IsWindowRunning())
    {
        float deltaTime = timer.GetDeltaTime(); // ensures frame-rate independance.

        DisplayFPS(timer.GetFPS());

        ClearScreen(COLOR_DARKGRAY);
        EngineSystems::Render(registry);
        UpdateWindow(); // swaps the backbuffer to the screen.
    }

    CloseWindow();

    return 0;
}