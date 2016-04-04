#include <iostream>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <time.h>
#include "player.h"
#include "menu.h"


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
	Player player = Player(renderer, images_dir.c_str(), audio_dir.c_str(), 950.0f, 384.0f);

	// **** MainMenu Textures - START ****
	Menu MainMenu = Menu(renderer, images_dir.c_str(), "mainMenu.png", 0.0f, 0.0f);
	Menu Level1 = Menu(renderer, images_dir.c_str(), "level1.png", 0.0f, 0.0f);
	Menu Level2 = Menu(renderer, images_dir.c_str(), "level2.png", 0.0f, 0.0f);
	Menu Instructions = Menu(renderer, images_dir.c_str(), "instructions.png", 0.0f, 0.0f);
	Menu Backstory = Menu(renderer, images_dir.c_str(), "backstory.png", 0.0f, 0.0f);
	Menu Win = Menu(renderer, images_dir.c_str(), "win.png", 0.0f, 0.0f);
	Menu Lose = Menu(renderer, images_dir.c_str(), "win.png", 0.0f, 0.0f);

	// **** Set Up Game State Enums
	enum GameState {
		MENU, INSTRUCTIONS, BACKSTORY, LEVEL1, LEVEL2, WIN, LOSE
	};

	GameState gameState = MENU;

	// bools for menu switching
	bool menu= false; bool instructions= false; bool backstory= false; bool level1= false; bool level2= false; bool win= false; bool lose= false; bool quit = false;



	// **** MAIN LOOP GAME START ****

	while (!quit) {

		switch (gameState) {
		// *********************************************************************************************************
		case MENU:
			menu = true;
			cout << "The Game State is Menu..." << endl;
			cout << "Press A Button for level 1..." << endl;
			cout << "Press B Button for Instructions..." << endl;
			cout << "Press X Button for BackStory..." << endl;

			while (menu) {

				// Create deltaTime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// Handle Events on Queue - Key press and such - START ****
				while (SDL_PollEvent(&e) != 0) {

					// User Requests quit
					if (e.type == SDL_QUIT) {
						quit = true;
						menu = false;
						break;
					}

					switch (e.type) {

					case SDL_CONTROLLERBUTTONDOWN:

						if (e.cdevice.which == 0) {
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {

								menu = false;
								gameState = LEVEL1;
							}
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_B) {

								menu = false;
								gameState = INSTRUCTIONS;
							}
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_X) {

								menu = false;
								gameState = BACKSTORY;
							}
						}
						break;

					}

				} // END POLL EVENT

				// **** DRAW PROCESS ****
				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				MainMenu.Draw(renderer);

				// Present screen render
				SDL_RenderPresent(renderer);
			}
			break; //end main menu case

		// *********************************************************************************************************
		case LEVEL1:
			level1 = true;
			cout << "The Game State is LEVEL 1..." << endl;
			cout << "Press A Button for firing ..." << endl;
			cout << "Press X Button for LEVEL 2 ..." << endl;

			while (level1){

				//set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				while (SDL_PollEvent(&e) != 0) {

					//check to see if the SDL Window is closed - player clicks X in Window
					if (e.type == SDL_QUIT) {
						quit = true;
						level1 = false;
						break;

					}
					switch (e.type) {

					case SDL_CONTROLLERBUTTONDOWN:

						if (e.cdevice.which == 0) {
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {

								player.OnControllerButton(e.cbutton);
								break;
							}
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_X) {

								level1 = false;
								gameState = LEVEL2;
							}

						}
						break;

					}
				}

				// Get x and Y values from game controller
				const Sint16 Xvalue = SDL_GameControllerGetAxis(gGameControllerO,
						SDL_CONTROLLER_AXIS_LEFTX);
				const Sint16 Yvalue = SDL_GameControllerGetAxis(gGameControllerO,
						SDL_CONTROLLER_AXIS_LEFTY);

				// Pass values to method
				player.OnControllerAxis(Xvalue, Yvalue);

				// **** Update
				// Update player
				player.Update(deltaTime);

				// **** Draw

				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				Level1.Draw(renderer);

				// Player Draw
				player.Draw(renderer);

				// Present screen render
				SDL_RenderPresent(renderer);
			}
			break; //end level1 case

		// *********************************************************************************************************
		case LEVEL2:
			level2 = true;
			cout << "The Game State is LEVEL 2..." << endl;
			cout << "Press A Button for firing ..." << endl;

			while (level2){

				//set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				while (SDL_PollEvent(&e) != 0) {

					//check to see if the SDL Window is closed - player clicks X in Window
					if (e.type == SDL_QUIT) {
						quit = true;
						level2 = false;
						break;

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

					}
				}

				// Get x and Y values from game controller
				const Sint16 Xvalue = SDL_GameControllerGetAxis(gGameControllerO,
						SDL_CONTROLLER_AXIS_LEFTX);
				const Sint16 Yvalue = SDL_GameControllerGetAxis(gGameControllerO,
						SDL_CONTROLLER_AXIS_LEFTY);

				// Pass values to method
				player.OnControllerAxis(Xvalue, Yvalue);

				// **** Update
				// Update player
				player.Update(deltaTime);

				// **** Draw
				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				Level2.Draw(renderer);

				// Player Draw
				player.Draw(renderer);

				// Present screen render
				SDL_RenderPresent(renderer);
			}
			break; //end level2 case

		// *********************************************************************************************************
		case INSTRUCTIONS:
			instructions = true;
			cout << "The Game State is INSTRUCTIONS..." << endl;
			cout << "Press A Button for MENU ..." << endl;
			cout << "Press B Button for WIN ..." << endl;
			cout << "Press X Button for LOSE ..." << endl;

			while (instructions){


				//set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				while (SDL_PollEvent(&e) != 0) {

					//check to see if the SDL Window is closed - player clicks X in Window
					if (e.type == SDL_QUIT) {
						quit = true;
						instructions = false;
						break;

					}
					switch (e.type) {

					case SDL_CONTROLLERBUTTONDOWN:

						if (e.cdevice.which == 0) {
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {

								instructions = false;
								gameState = MENU;
							}
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_B) {

								instructions = false;
								gameState = WIN;
							}
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_X) {

								instructions = false;
								gameState = LOSE;
							}
						}
						break;
					}
				}

				// **** DRAW PROCESS ****
				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				Instructions.Draw(renderer);

				// Present screen render
				SDL_RenderPresent(renderer);


			}
			break; //end Instructions case

		// *********************************************************************************************************
		case BACKSTORY:
			backstory = true;
			cout << "The Game State is Backstory..." << endl;
			cout << "Press A Button for main menu ..." << endl;

			while (backstory){

				//set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				while (SDL_PollEvent(&e) != 0) {

					//check to see if the SDL Window is closed - player clicks X in Window
					if (e.type == SDL_QUIT) {
						quit = true;
						backstory = false;
						break;

					}
					switch (e.type) {

					case SDL_CONTROLLERBUTTONDOWN:

						if (e.cdevice.which == 0) {
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {

								backstory = false;
								gameState = MENU;
							}
						}
						break;
					}
				}

				// **** DRAW PROCESS ****
				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				Backstory.Draw(renderer);

				// Present screen render
				SDL_RenderPresent(renderer);


			}
			break; //end back story case

		// *********************************************************************************************************
		case WIN:
			win = true;
			cout << "The Game State is WIN..." << endl;
			cout << "Press A Button for main menu ..." << endl;

			while (win){

				//set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				while (SDL_PollEvent(&e) != 0) {

					//check to see if the SDL Window is closed - player clicks X in Window
					if (e.type == SDL_QUIT) {
						quit = true;
						win = false;
						break;

					}
					switch (e.type) {

					case SDL_CONTROLLERBUTTONDOWN:

						if (e.cdevice.which == 0) {
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {

								win = false;
								gameState = MENU;
							}
						}
						break;
					}
				}

				// **** DRAW PROCESS ****
				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				Win.Draw(renderer);

				// Present screen render
				SDL_RenderPresent(renderer);
			}
			break; //end win case

		// *********************************************************************************************************
		case LOSE:
			lose = true;
			cout << "The Game State is LOSE..." << endl;
			cout << "Press A Button for main menu ..." << endl;

			while (lose){

				//set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				while (SDL_PollEvent(&e) != 0) {

					//check to see if the SDL Window is closed - player clicks X in Window
					if (e.type == SDL_QUIT) {
						quit = true;
						lose = false;
						break;

					}
					switch (e.type) {

					case SDL_CONTROLLERBUTTONDOWN:

						if (e.cdevice.which == 0) {
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {

								lose = false;
								gameState = MENU;
							}
						}
						break;
					}
				}

				// **** DRAW PROCESS ****
				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				Lose.Draw(renderer);

				// Present screen render
				SDL_RenderPresent(renderer);
			}
			break; //end lose case

		}//end gamestate
	}//end while !quit loop

	// Close and Destroy Window
	SDL_DestroyWindow(window);

	// Clean Up
	SDL_Quit();

	return 0;
}
