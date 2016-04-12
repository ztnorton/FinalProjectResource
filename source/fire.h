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

class Fire {
public:


	SDL_Texture *texture;

	SDL_Rect posRect;

	SDL_Rect drawRect;

	// Width and Height of each frame plus W and H of entire image
	long int frameWidth, frameHeight, textureWidth, textureHeight;

	// Float values to track time until next frame of animation
	float frameCounter;

	Fire(SDL_Renderer *renderer, string filePath, int i, float x, float y);

	void Update(float deltaTime);

	void Draw(SDL_Renderer *renderer);

};
