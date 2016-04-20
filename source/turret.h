#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <stdint.h>
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

#include "turretbullet.h"

class Turret {
public:

	// Is turret alive?
	bool active;
	bool killed;

	int health;

	// Var to hold bullets
	vector<TurretBullet> bulletList;

	// String to hold texture path
	string turretPath;

	// Turret's texture for base and barrel
	SDL_Texture *tBase;
	SDL_Texture *tBarrel;
	SDL_Texture *tRoof;

	// Turret base and Barrel SDL_Rect for the X,Y,W, and H for Texture
	SDL_Rect baseRect;
	SDL_Rect barrelRect;
	SDL_Rect roofRect;

	// Vars used to get angle between the turrets position and tank's position
	float x, y, turretangle;

	// Point that will be used to rotate turret - relative to the turret barrel's RECT
	SDL_Point center;

	// Floats for fireTime and fireRate
	float fireTime = 0.0f;
	float fireRate = 1000.0f; // one second

	// Float for the turret's pos to avoid precision loss
	float posB_X, posB_Y, posT_X, posT_Y, posR_X, posR_Y;

	// Audio sound effect - CHUNK
	Mix_Chunk *fire;

	// Turrets creation method using passed in values for renderer, player number, path to texture starting position X, starting position Y
	Turret(SDL_Renderer *renderer, string filePath, string audioPath, float x,
			float y);

	void Update(float deltaTime, SDL_Rect tankRect);

	void Draw(SDL_Renderer *renderer);

	void Reset();

	void levelReset(float x, float y);

	// Create a Bullet
	void CreateBullet(SDL_Rect target);

	void RemoveHealth();

	void TankMoveX(float tankSpeed, float deltaTime);

	void TankMoveY(float tankSpeed, float deltaTime);



};
