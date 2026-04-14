#include "GraphicsLib.h"
#include "Registry.h"
#include "InputManager.h"
#include "EntityBuilder.h"
#include "InputSystem.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"
#include "RenderSystem.h"
#include <iostream>

// main file
// file that directs everything.

// wWinMain is essentially window's version of int main()
// _In_ means windows will pass information IN here, whereas _In_opt is optional
// Using _In_ or _In_opt_ is optional, just used to be very precise and make the IDE not put a warning.
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    //// Debug Console
    //AllocConsole();
    //FILE* dummy;
    //freopen_s(&dummy, "CONOUT$", "w", stdout);
    ////

    OpenWindow(L"Game Engine Test", 1280, 720);

    Registry registry;
    AssetManager assets;
    InputManager input;
    Timer timer;

    // bind keys
    input.BindKey("MoveUp", 'W');
    input.BindKey("MoveDown", 'S');
    input.BindKey("MoveLeft", 'A');
    input.BindKey("MoveRight", 'D');

    InputSystem inputSystem(registry, input);
    CollisionSystem collisionSystem(registry, assets);
    MovementSystem movementSystem(registry);

    // create the texture of an entity
    assets.CreateColorSquare("Player1Tex", 50, COLOR_RED);
    assets.CreateColorSquare("BoxTex", 50, COLOR_BLUE);
    assets.CreateColorSquare("HitTex", 50, COLOR_YELLOW);

    // create an entity
    // we assign the values needed to this new entity between: position, sprite, kinematic values, and controller id. Then build it.
    Entity player = EntityBuilder(registry, assets)
        .WithTransform(100.0f, 100.0f)
        .WithSprite("Player1Tex")
        .WithKinematics(300.0f)
        .WithController(1)
        .WithCollision(50.0f, 50.0f, false)
        .Build();

    Entity box = EntityBuilder(registry, assets)
        .WithTransform(300.0f, 300.0f)
        .WithSprite("BoxTex")
        .WithCollision(50.0f, 50.0f, true)
        .Build();

    while (IsWindowRunning())
    {
        float deltaTime = timer.GetDeltaTime(); // ensures frame-rate independance.
        DisplayFPS(timer.GetFPS());

        input.Update();
        inputSystem.Update();
        collisionSystem.Update();
        movementSystem.Update(deltaTime);
        

        ClearScreen(COLOR_DARKGRAY);
        EngineSystems::Render(registry);
        UpdateWindow(); // swaps the backbuffer to the screen.
    }

    CloseWindow();

    return 0;
}