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
#include "button.h"


using namespace std;

// DeltaTime var initialization - FPS independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;


//**** Joystick

//Analog joy stick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

// joy stick directions variables
float xDir, yDir;

// cursor float vars for movement
float pos_X, pos_Y;

// Create SDL Rectangle for the 2 player graphic
SDL_Rect cursorPos, activePos;

// Variable for cursor speed
int cursorSpeed = 400;

void moveCursor(const SDL_ControllerAxisEvent event) {
	//check Joystick 0 - first player
	if (event.which == 0) {

		//check X Axis
		if (event.axis == 0) {

			if (event.value < -JOYSTICK_DEAD_ZONE) {
				xDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE) {
				xDir = 1.0f;
			}
			else {
				xDir = 0.0f;
			}
		}

		//check Y Axis
		if (event.axis == 1) {

			if (event.value < -JOYSTICK_DEAD_ZONE) {
				yDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE) {
				yDir = 1.0f;
			}
			else {
				yDir = 0.0f;
			}
		}
	}
}

void UpdateCursor(float deltaTime) {

	//Update Cursor
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	//assign to SDL_Rect ints X and Y
	cursorPos.x = (int)(pos_X + 0.5f);
	cursorPos.y = (int)(pos_Y + 0.5f);

	//update active position of cursor - collision box
	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	// off the screen in X
	if (cursorPos.x < 0) {
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}
	if (cursorPos.x > 1024 - cursorPos.w) {
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;
	}

	// off the screen in Y
	if (cursorPos.y < 0) {
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}
	if (cursorPos.y > 768 - cursorPos.h) {
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;
	}
}

// variables for all Menu button over
bool startGameOver = false, backstoryOver = false, instructionsOver = false,
		quitOver = false, menuOver = false, playAgainOver = false, continueOver = false;


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

	// **** Create SDL Window - START ****
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

	// **** Variable Creation

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

	// **** Main Menu Textures
	Menu MainMenu = Menu(renderer, images_dir.c_str(), "mainMenu.png", 0.0f, 0.0f);
	Menu Level1 = Menu(renderer, images_dir.c_str(), "level1.png", 0.0f, 0.0f);
	Menu Level2 = Menu(renderer, images_dir.c_str(), "level2.png", 0.0f, 0.0f);
	Menu Instructions = Menu(renderer, images_dir.c_str(), "instBackstory.png", 0.0f, 0.0f);
	Menu Backstory = Menu(renderer, images_dir.c_str(), "instBackstory.png", 0.0f, 0.0f);
	Menu Win = Menu(renderer, images_dir.c_str(), "winLose.png", 0.0f, 0.0f);
	Menu Lose = Menu(renderer, images_dir.c_str(), "winLose.png", 0.0f, 0.0f);

	// **** Set Up Game State Enumerators
	enum GameState {
		MENU, INSTRUCTIONS, BACKSTORY, LEVEL1, LEVEL2, WIN, LOSE
	};

	GameState gameState = MENU;

	// bools for menu switching
	bool menu= false; bool instructions= false; bool backstory= false; bool level1= false; bool level2= false; bool win= false; bool lose= false; bool quit = false;

	bool level1won = false;


	//**** Create Cursor
	// create a SDL texture - cursor
	SDL_Texture *cursor;

	//create the SDL surface to hold texture file
	cursor = IMG_LoadTexture(renderer, (images_dir + "cursor.png").c_str());

	// create the SDL_Rectangle for the texture's position and size - x,y,w,h
	//SDL_Rect cursorPos, activePos;
	cursorPos.x = 0;
	cursorPos.y = 0;

	int w, h;
	SDL_QueryTexture(cursor, NULL, NULL, &w, &h);
	cursorPos.w = w;
	cursorPos.h = h;

	// set the X, Y, W, and H for the Rectangle - cursor collision
	activePos.x = 5;
	activePos.y = 5;
	activePos.w = 5;
	activePos.h = 5;

	// **** Create Buttons and selection
	Button selected (renderer, images_dir.c_str(), "selected.png", 750.0f, 500.0f);

	Button startN (renderer, images_dir.c_str(), "startN.png", 386.0f, 419.0f);
	Button instructionsN (renderer, images_dir.c_str(), "instructionsN.png", 386.0f, 478.0f);
	Button backstoryN (renderer, images_dir.c_str(), "backstoryN.png", 386.0f, 537.0f);

	Button quitN (renderer, images_dir.c_str(), "quitN.png", 386.0f, 598.0f);

	Button mainN (renderer, images_dir.c_str(), "mainN.png", 750.0f, 15.0f);

	Button playAgainN (renderer, images_dir.c_str(), "playN.png", 750.0f, 55.0f);
	Button continueN (renderer, images_dir.c_str(), "continueN.png", 750.0f, 55.0f);

	// **** MAIN LOOP GAME START ****

	while (!quit) {

		switch (gameState) {
		// *********************************************************************************************************
		case MENU:
			menu = true;

			backstoryN.posRect.x = 386.0f;
			backstoryN.posRect.y = 537.0f;

			instructionsN.posRect.x = 386.0f;
			instructionsN.posRect.y = 478.0f;

			startN.posRect.x = 386.0f;
			startN.posRect.y = 419.0f;

			cout << "The Game State is Menu..." << endl;
			cout << "B to WIN..." << endl;
			cout << "X to LOSE..." << endl;


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

								if (startGameOver) {
									menu = false;
									gameState = LEVEL1;
									startGameOver = false;
								}
								if (backstoryOver) {
									menu = false;
									gameState = BACKSTORY;
									backstoryOver = false;
								}
								if (instructionsOver) {
									menu = false;
									gameState = INSTRUCTIONS;
									instructionsOver = false;
								}
								if (quitOver) {
									menu = false;
									quit = true;
									quitOver = false;
								}
							}

							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_B) {
								menu = false;
								gameState = WIN;
							}

							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_X) {
								menu = false;
								gameState = LOSE;
							}

						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(e.caxis);
						break;
					}

				} // END POLL EVENT

				// **** UPDATE
				UpdateCursor(deltaTime);

				// check for collision between cursor active state and buttons
				startGameOver = SDL_HasIntersection(&activePos, &startN.posRect);
				backstoryOver = SDL_HasIntersection(&activePos, &backstoryN.posRect);
				instructionsOver = SDL_HasIntersection(&activePos, &instructionsN.posRect);
				quitOver = SDL_HasIntersection(&activePos, &quitN.posRect);

				// **** DRAW PROCESS ****
				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				MainMenu.Draw(renderer);

				if(startGameOver){
					startN.Draw(renderer, startN.posRect);
					selected.Draw(renderer, startN.posRect);
				}else{
					startN.Draw(renderer, startN.posRect);
				}
				if(backstoryOver){
					backstoryN.Draw(renderer, backstoryN.posRect);
					selected.Draw(renderer, backstoryN.posRect);
				}else{
					backstoryN.Draw(renderer, backstoryN.posRect);
				}
				if(instructionsOver){
					instructionsN.Draw(renderer, instructionsN.posRect);
					selected.Draw(renderer, instructionsN.posRect);
				}else{
					instructionsN.Draw(renderer, instructionsN.posRect);
				}
				if(quitOver){
					quitN.Draw(renderer, quitN.posRect);
					selected.Draw(renderer, quitN.posRect);
				}else{
					quitN.Draw(renderer, quitN.posRect);
				}

				//Draw Cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

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

							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_B) {
								level1 = false;
								level1won = true;
								gameState = WIN;
							}

							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_Y) {
								level1 = false;
								gameState = LOSE;
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
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_B) {
								level2 = false;
								gameState = WIN;
							}

							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_Y) {
								level2 = false;
								gameState = LOSE;
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

			backstoryN.posRect.x = 750.0f;
			backstoryN.posRect.y = 55.0f;

			startN.posRect.x = 750.0f;
			startN.posRect.y = 100.0f;

			cout << "The Game State is INSTRUCTIONS..." << endl;

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


								if (startGameOver) {
									instructions = false;
									gameState = LEVEL1;
									startGameOver = false;
								}
								if (menuOver) {
									instructions = false;
									gameState = MENU;
									menuOver = false;
								}
								if (backstoryOver) {
									instructions = false;
									gameState = BACKSTORY;
									backstoryOver = false;
								}
							}

						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(e.caxis);
						break;
					}
				}

				// **** UPDATE
				UpdateCursor(deltaTime);

				// check for collision between cursor active state and buttons
				startGameOver = SDL_HasIntersection(&activePos, &startN.posRect);
				backstoryOver = SDL_HasIntersection(&activePos, &backstoryN.posRect);
				menuOver = SDL_HasIntersection(&activePos, &mainN.posRect);

				// **** DRAW PROCESS ****
				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				Instructions.Draw(renderer);

				if(startGameOver){
					startN.Draw(renderer, startN.posRect);
					selected.Draw(renderer, startN.posRect);
				}else{
					startN.Draw(renderer, startN.posRect);
				}
				if(backstoryOver){
					backstoryN.Draw(renderer, backstoryN.posRect);
					selected.Draw(renderer, backstoryN.posRect);
				}else{
					backstoryN.Draw(renderer, backstoryN.posRect);
				}
				if(menuOver){
					mainN.Draw(renderer, mainN.posRect);
					selected.Draw(renderer, mainN.posRect);
				}else{
					mainN.Draw(renderer, mainN.posRect);
				}

				//Draw Cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				// Present screen render
				SDL_RenderPresent(renderer);


			}
			break; //end Instructions case

			// *********************************************************************************************************
		case BACKSTORY:
			backstory = true;

			instructionsN.posRect.x = 750.0f;
			instructionsN.posRect.y = 55.0f;

			startN.posRect.x = 750.0f;
			startN.posRect.y = 100.0f;

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


								if (startGameOver) {
									backstory = false;
									gameState = LEVEL1;
									startGameOver = false;
								}
								if (menuOver) {
									backstory = false;
									gameState = MENU;
									menuOver = false;
								}
								if (instructionsOver) {
									backstory = false;
									gameState = INSTRUCTIONS;
									instructionsOver = false;
								}

							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(e.caxis);
						break;
					}
				}

				// **** UPDATE
				UpdateCursor(deltaTime);

				// check for collision between cursor active state and buttons
				startGameOver = SDL_HasIntersection(&activePos, &startN.posRect);
				instructionsOver = SDL_HasIntersection(&activePos, &instructionsN.posRect);
				menuOver = SDL_HasIntersection(&activePos, &mainN.posRect);

				// **** DRAW PROCESS ****
				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				Backstory.Draw(renderer);

				if(startGameOver){
					startN.Draw(renderer, startN.posRect);
					selected.Draw(renderer, startN.posRect);
				}else{
					startN.Draw(renderer, startN.posRect);
				}
				if(instructionsOver){
					instructionsN.Draw(renderer, instructionsN.posRect);
					selected.Draw(renderer, instructionsN.posRect);
				}else{
					instructionsN.Draw(renderer, instructionsN.posRect);
				}
				if(menuOver){
					mainN.Draw(renderer, mainN.posRect);
					selected.Draw(renderer, mainN.posRect);
				}else{
					mainN.Draw(renderer, mainN.posRect);
				}

				//Draw Cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

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

								if (continueOver) {
									win = false;
									gameState = LEVEL2;
									continueOver = false;
								}
								if (menuOver) {
									win = false;
									gameState = MENU;
									menuOver = false;
								}

							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(e.caxis);
						break;
					}
				}

				// **** UPDATE
				UpdateCursor(deltaTime);

				// check for collision between cursor active state and buttons
				continueOver = SDL_HasIntersection(&activePos, &continueN.posRect);
				menuOver = SDL_HasIntersection(&activePos, &mainN.posRect);

				// **** DRAW PROCESS ****
				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				Win.Draw(renderer);

				if(continueOver){
					continueN.Draw(renderer, continueN.posRect);
					selected.Draw(renderer, continueN.posRect);
				}else{
					continueN.Draw(renderer, continueN.posRect);
				}
				if(menuOver){
					mainN.Draw(renderer, mainN.posRect);
					selected.Draw(renderer, mainN.posRect);
				}else{
					mainN.Draw(renderer, mainN.posRect);
				}

				//Draw Cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

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

								if (playAgainOver) {
									lose = false;
									if (level1won == true)
									{
										gameState = LEVEL2;
									}else{
										gameState = LEVEL1;
									}
									playAgainOver = false;
								}
								if (menuOver) {
									lose = false;
									gameState = MENU;
									menuOver = false;
								}

							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(e.caxis);
						break;
					}
				}

				// **** UPDATE
				UpdateCursor(deltaTime);

				// check for collision between cursor active state and buttons
				playAgainOver = SDL_HasIntersection(&activePos, &playAgainN.posRect);
				menuOver = SDL_HasIntersection(&activePos, &mainN.posRect);

				// **** DRAW PROCESS ****
				//Clear the SDL RenderTarget
				SDL_RenderClear(renderer);

				// Draw the main menu
				Lose.Draw(renderer);

				if(playAgainOver){
					playAgainN.Draw(renderer, playAgainN.posRect);
					selected.Draw(renderer, playAgainN.posRect);
				}else{
					playAgainN.Draw(renderer, playAgainN.posRect);
				}
				if(menuOver){
					mainN.Draw(renderer, mainN.posRect);
					selected.Draw(renderer, mainN.posRect);
				}else{
					mainN.Draw(renderer, mainN.posRect);
				}

				//Draw Cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

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
