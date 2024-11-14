#include <stdio.h>
#include <SDL2/SDL.h>

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

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

    SDL_SetHint(SDL_HINT_JOYSTICK_MFI, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_IOKIT, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_XBOX, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_XBOX_360, "0");
	SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT, "0");
	SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT_CORRELATE_XINPUT, "0");

    printf("Waiting for controllers...\n");

    SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");

    SDL_Event e;
    int running = 1;
    while (running)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        int numJoysticks = SDL_NumJoysticks();
        printf("Connected controllers: %d\n", numJoysticks);

        for (int i = 0; i < numJoysticks; i++)
        {
            SDL_GameController* gGameController = SDL_GameControllerOpen(i);
            if (gGameController != NULL)
            {
                printf("Controller %i: %s, address: %p\n", i, SDL_GameControllerName(gGameController), (void*)gGameController);
            // Get the joystick associated with this game controller
            SDL_Joystick* joystick = SDL_GameControllerGetJoystick(gGameController);
            
            // Get the GUID for the joystick
            SDL_JoystickGUID guid = SDL_JoystickGetGUID(joystick);
            char guidString[33]; // GUID is 16 bytes, so string representation will be 32 characters + null terminator
            SDL_JoystickGetGUIDString(guid, guidString, sizeof(guidString));
            
            printf("Controller %d: %s\n", i, SDL_GameControllerName(gGameController));
            printf("Joystick GUID: %s\n", guidString); // Print the GUID in string format
                if (SDL_GameControllerHasRumble(gGameController))
                {
                    SDL_GameControllerRumble(gGameController, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 500);
                    SDL_Delay(500);
                    SDL_GameControllerRumble(gGameController, 0, 0, 0);
                    printf("Rumble on controller %d\n", i);

                    // Print the joystick mode hints after each rumble
                    const char* mfiHint = SDL_GetHint(SDL_HINT_JOYSTICK_MFI);
                    const char* iokitHint = SDL_GetHint(SDL_HINT_JOYSTICK_IOKIT);
                    const char* hidHint = SDL_GetHint(SDL_HINT_JOYSTICK_HIDAPI);
					const char* xboxhidHint = SDL_GetHint(SDL_HINT_JOYSTICK_HIDAPI_XBOX);
					const char* xbox360hidHint = SDL_GetHint(SDL_HINT_JOYSTICK_HIDAPI_XBOX_360);

                    printf("SDL Joystick Modes:\n");
                    printf("MFI: %s\n", mfiHint ? mfiHint : "Not Set");
                    printf("IOKIT: %s\n", iokitHint ? iokitHint : "Not Set");
                    printf("HIDAPI: %s\n\n", hidHint ? hidHint : "Not Set");
					printf("XBOXHIDAPI: %s\n\n", xboxhidHint ? xboxhidHint : "Not Set");
					printf("XBOX360HIDAPI: %s\n\n", xbox360hidHint ? xbox360hidHint : "Not Set");
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

        SDL_Delay(2000);
    }

    printf("Exiting program...\n");

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
