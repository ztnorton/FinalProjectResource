#include <string>
#include <iostream>
#include <math.h>
#include <time.h>
using namespace std;

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#endif

class TurretBullet {
public:

	bool active;

	// Bullet Texture
	SDL_Texture *texture;

	// Bullet Rect for postition and size
	SDL_Rect posRect;

	// Bullet Speed
	float speed;

	//Bullet float positions to prevent percision loss
	float pos_X, pos_Y;

	// Poistion for the turret and player
	SDL_Point turret, player;

	// Distance between the 2
	float distance;

	// Bullet creation method. Requires rederer, path to image, X pos, and Y pos
	TurretBullet(SDL_Renderer *renderer, string filePath, float x, float y);

	// Bullet Update - requires DeltaTime
	void Update(float deltaTime);

	// Bullet Draw - requires Renderer
	void Draw(SDL_Renderer *renderer);

	// Bullet Reset
	void Reset();

	// Bullet Start
	void Start(SDL_Rect playerPos, SDL_Rect turretPos);

};
