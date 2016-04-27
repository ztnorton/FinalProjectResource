// needed includes for cross platform development
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

// needed includes
#include <string>
#include <iostream>
using namespace std;

class Explode {

public:

	// Bool for active state of explosion
	bool active;

	// Explosion Texture
	SDL_Texture *texture;

	// Explosion Rect for postition and size
	SDL_Rect posRect;

	// Explosion rectangle for position and size
	SDL_Rect drawRect;

	// Width and Height of each fram plus W and H of entire image
	int frameWidth, frameHeight, textureWidth, textureHeight;

	// Float values to track time until next frame of animation
	float frameCounter;

	// Explode creation method. Requires rederer, path to image, X pos, and Y pos
	Explode(SDL_Renderer *renderer, string filePath, float x, float y);

	// Explode Update - requires DeltaTime
	void Update(float deltaTime);

	void makeExplosion(int x, int y);

	// Explode Draw - requires Renderer
	void Draw(SDL_Renderer *renderer);

	// Explode Reset
	void Reset();

};