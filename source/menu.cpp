#include "menu.h"


Menu::Menu(SDL_Renderer *renderer, string dirPath, string filePath, float x, float y){


	//create the SDL surface to hold texture file
	menuPath = dirPath + filePath;

	// create a SDL texture - title
	texture = IMG_LoadTexture(renderer, menuPath.c_str());

	posRect.x = x;
	posRect.y = y;

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	pos_X = x;
	pos_Y = y;


}

void Menu::Draw(SDL_Renderer *renderer){

	SDL_RenderCopy(renderer, texture, NULL, &posRect);


}