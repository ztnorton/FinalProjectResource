#include <iostream>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <time.h>
#include "player.h"


using namespace std;

// DeltaTime var initialization - FPS independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>

// Get current working directory
string currentWorkingDirectory(getcwd(NULL, 0));

// Create a string linking to the images folder
string images_dir = currentWorkingDirectory + "/FinalProjectResource/image/";

// Create a string to link to the audio folder
string audio_dir = currentWorkingDirectory + "/FinalProjectResource/audio/";
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include <direct.h>
#define getcwd _getcwd

string currentWorkingDirectory(getcwd(NULL, 0));
string images_dir = currentWorkingDirectory + "\\FinalProjectResource\\image\\";
string audio_dir = currentWorkingDirectory + "\\FinalProjectResource\\audio\\";
#endif


#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include <unistd.h>

string currentWorkingDirectory(getcwd(NULL, 0));
string images_dir = currentWorkingDirectory + "/FinalProjectResource/image/";
string audio_dir = currentWorkingDirectory + "/FinalProjectResource/audio/";
#endif

// ******** MAIN START **********
int main(int argc, char* argv[]) {

	// **** Creat SDL Window - START ****
	// Start SDL2
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = NULL;

	SDL_Renderer *renderer = NULL;

	// Create an application window with the following settings:
	window = SDL_CreateWindow("CELL BLOCK Z", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_SHOWN);

	// Create an SDL RenderTarget
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	// **** Create SDL Window - END ****


	// **** Variable Creation - START ****
	// Main Loop flag
	bool quit = false;

	// Event Handler
	SDL_Event e;

	// **** Turn on Game Controller Events ****
	SDL_GameControllerEventState(SDL_ENABLE);

	// **** Set up a Game Controller variable for player1 ****
	SDL_GameController* gGameControllerO = NULL;

	// **** Open Game Controller for player1 ****
	gGameControllerO = SDL_GameControllerOpen(0);

	// **** Open Audio Channel ****
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// **** Create Players - START ****
	Player player = Player(renderer, images_dir.c_str(), audio_dir.c_str(), 512.0f, 384.0f);

	// **** Background Texture - START ****
	SDL_Texture *bkgd = IMG_LoadTexture(renderer,
			(images_dir + "mainMenu.png").c_str());

	SDL_Rect bkgdRect;

	bkgdRect.x = 0;
	bkgdRect.y = 0;
	bkgdRect.w = 1024;
	bkgdRect.h = 768;

	float X_pos = 0.0f;
	float Y_pos = 0.0f;

	// **** MAIN LOOP GAME START ****

	while (!quit) {

		// Create deltaTime
		thisTime = SDL_GetTicks();
		deltaTime = (float)(thisTime - lastTime) / 1000;
		lastTime = thisTime;

		// Handle Events on Queue - Key press and such - START ****
		while (SDL_PollEvent(&e) != 0) {

			// User Requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			switch (e.type) {

			case SDL_CONTROLLERBUTTONDOWN:

				if (e.cdevice.which == 0) {

					if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {

						player.OnControllerButton(e.cbutton);
						break;
					}
				}
				break;

			case SDL_CONTROLLERAXISMOTION:

				//tank1.OnControllerAxis(e.caxis);
				break;

			}

		} // END POLL EVENT

		// Get x and Y values from game controller
		const Sint16 Xvalue = SDL_GameControllerGetAxis(gGameControllerO,
				SDL_CONTROLLER_AXIS_LEFTX);
		const Sint16 Yvalue = SDL_GameControllerGetAxis(gGameControllerO,
				SDL_CONTROLLER_AXIS_LEFTY);

		// Pass values to method
		player.OnControllerAxis(Xvalue, Yvalue);

		// **** UPDATE PROCESS ****
		// Update player
		player.Update(deltaTime);

		// **** DRAW PROCESS ****
		//Clear the SDL RenderTarget
		SDL_RenderClear(renderer);

		// Draw the main menu
		SDL_RenderCopy(renderer, bkgd, NULL, &bkgdRect);

		// Player Draw
		player.Draw(renderer);

		// Present screen render
		SDL_RenderPresent(renderer);

	}

	// Close and Destroy Window
	SDL_DestroyWindow(window);

	// Clean Up
	SDL_Quit();

	return 0;
}
