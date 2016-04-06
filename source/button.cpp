#include "button.h"


Button::Button(SDL_Renderer *renderer, string dirPath, string filePath, float x, float y){

	//create the SDL surface to hold texture file
	buttonPath = dirPath + filePath;

	// create a SDL texture
	texture = IMG_LoadTexture(renderer, buttonPath.c_str());

	posRect.x = x;
	posRect.y = y;

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	pos_X = x;
	pos_Y = y;

}

void Button::Draw(SDL_Renderer *renderer, SDL_Rect positionRect){

	SDL_RenderCopy(renderer, texture, NULL, &positionRect);


}
