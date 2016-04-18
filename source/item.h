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

class Item {
public:

	bool active, pickedUp;

	string itemPath;

	SDL_Texture *texture;

	SDL_Rect posRect;

	int itemType;

	float pos_X, pos_Y;

	Item(SDL_Renderer *renderer, string dirPath, int i, float x, float y);

	void Reset();

	void Drop(int x, int y);

	void Draw(SDL_Renderer *renderer);

};
