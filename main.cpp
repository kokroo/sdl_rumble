#include <stdio.h>
#include <SDL2/SDL.h>

int main(void)
{
    // Initialize SDL with video and joystick
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create an SDL window (required for macOS gamepad detection)
    SDL_Window* window = SDL_CreateWindow("SDL Gamepad Test",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN);

    if (!window)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_SetHint(SDL_HINT_JOYSTICK_MFI, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_IOKIT, "1");

    printf("Waiting for controllers...\n");

    // Load custom controller mappings (if any)
    SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");

    // Event loop
    SDL_Event e;
    int running = 1;
    while (running)
    {
        // Process events
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        // Check for connected controllers
        int numJoysticks = SDL_NumJoysticks();
        printf("Connected controllers: %d\n", numJoysticks);

        for (int i = 0; i < numJoysticks; i++)
        {
            SDL_GameController* gGameController = SDL_GameControllerOpen(i);
            if (gGameController != NULL)
            {
                printf("Controller %i: %s, address: %p\n", i, SDL_GameControllerName(gGameController), (void*)gGameController);

                // Check for rumble capability and initiate rumble
                if (SDL_GameControllerHasRumble(gGameController))
                {
                    SDL_GameControllerRumble(gGameController, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 500);
                    SDL_Delay(500); // Optional delay to show rumble effect
                    SDL_GameControllerRumble(gGameController, 0, 0, 0);
                    printf("Rumble on controller %d\n\n", i);
                }
                else
                {
                    printf("Rumble not supported on controller %d\n\n", i);
                }

                SDL_GameControllerClose(gGameController);
            }
            else
            {
                printf("No controller connected: %s\n", SDL_GetError());
            }
        }

        SDL_Delay(1000); // Delay for periodic checking
    }

    printf("Exiting program...\n");

    // Clean up and quit
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
