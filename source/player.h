#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
using namespace std;

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#endif

#include "bullet.h"

class Player {
public:

	// Textures for health bar
	//SDL_Texture *back, *mid, *front;

	// Player's SDL_Rect for the X,Y,W, and H for Texture
	//SDL_Rect backR, midR, frontR;

	// Player health
	//float playerHealth, maxHealth;

	// Is the player active - still alive?
	bool active;

	// Var to hold the list of bullets
	vector<PlayerBullet> bulletList;

	// String to hold path to player texture
	string playerPath;

	// Players texture to hold image
	SDL_Texture *texture;

	// Player's SDL_Rect for the X,Y,W, and H for Texture
	SDL_Rect posRect;

	// Floats to hold the joystick axis (-1 or 0 or 1)
	float xDir, xDirOld;
	float yDir, yDirOld;

	// Float for player speed
	float speed;

	// Float for the player's position to avoid precision loss
	float pos_X, pos_Y;

	// Vars used to get angle between the player's position and joystick position
	float xangle, yangle, playerangle;

	// Point that will be used to rotate turret - relative to the player barrel's RECT
	SDL_Point center;

	// Keep the last angle so the player can fire in correct direction
	float oldAngle;

	// Audio sound effect - CHUNK
	Mix_Chunk *fire;

	// Add X and Y values
	Sint16 Xvalue, Yvalue;

	// Players creation method using passed in values for renderer, player number, path to texture starting postion X, starting postition Y
	Player(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y);

	// Method to allow the player to move via joystick axis
	void OnControllerAxis(Sint16 X, Sint16 Y);

	// Method to allow the player to interact via joystick buttons
	void OnControllerButton(const SDL_ControllerButtonEvent event);

	// Update player using the passed in deltaTime
	void Update(float deltaTime);

	// Draw the player main's passed in renderer
	void Draw(SDL_Renderer *renderer);

	// Reset Player
	void Reset();

	// Player hit by Zombie
	void eZombieHit();

	// Player hit by Guard Tower bullet
	void eBulletHit();

private:

	void CreateBullet();

};
