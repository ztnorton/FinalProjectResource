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

class Zombie {
public:

	bool active;
	bool killed;

	Mix_Chunk *death;

	// Hold path to texture
	string zombiePath;

	// Hold image
	SDL_Texture *zombie;

	// Texture dimensions and position XYWH
	SDL_Rect zombieRect;

	// Get angle between tank and players position
	float x, y, tankangle;

	// Sound effect
	//Mix_Chunk *death;

	// speed
	float speed;

	// health
	int health;

	// Used to rotate
	SDL_Point center;

	// Float for the Z's position to avoid precision loss
	float posZ_X, posZ_Y;

	// Creation
	Zombie(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y);

	void Update(float deltaTime, SDL_Rect tankRect);

	void Draw(SDL_Renderer *renderer);

	void Reset();

	void RemoveHealth();

	void zombieMoveX(float tankSpeed, float deltaTime);

	void zombieMoveY(float tankSpeed, float deltaTime);


};
