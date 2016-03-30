#include <string>
#include <iostream>
#include <time.h>
using namespace std;

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#endif

class PlayerBullet {
public:

	// Bool for the state of the bullet
	bool active;

	// Bullet Texture
	SDL_Texture *texture;

	// Bullet Rect for postition and size
	SDL_Rect posRect;

	// Movement direction values
	float xDir, yDir;

	// Bullet Speed
	float speed;

	//Bullet float positions to prevent percision loss
	float pos_X, pos_Y;

	// Player angle passed in for direction
	float playerangle;

	// Bullet creation method. Requires rederer, path to image, X pos, and Y pos
	PlayerBullet(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY);

	// Bullet Update - requires DeltaTime
	void Update(float deltaTime);

	// Bullet Draw - requires Renderer
	void Draw(SDL_Renderer *renderer);

	// Bullet Reset
	void Reset();





};
