#include "item.h"


Item::Item(SDL_Renderer *renderer, string dirPath, string audioPath, int i, float x, float y){

	if ( i == 0){
		itemPath = dirPath + "key.png";
		texture = IMG_LoadTexture(renderer, itemPath.c_str());
		itemType = i;
		Pickup = Mix_LoadWAV((audioPath + "pickup.wav").c_str());
	} else if (i == 1) {
		itemPath = dirPath + "generator.png";
		texture = IMG_LoadTexture(renderer, itemPath.c_str());
		itemType = i;
		Pickup = Mix_LoadWAV((audioPath + "pickup.wav").c_str());
	} else if (i == 2) {
		itemPath = dirPath + "healthKit.png";
		texture = IMG_LoadTexture(renderer, itemPath.c_str());
		itemType = i;
		Pickup = Mix_LoadWAV((audioPath + "pickup.wav").c_str());
	} else if (i == 3) {
		itemPath = dirPath + "ammoBox.png";
		texture = IMG_LoadTexture(renderer, itemPath.c_str());
		itemType = i;
		Pickup = Mix_LoadWAV((audioPath + "reload.wav").c_str());
	} else if (i == 4) {
		itemPath = dirPath + "lvl1Gate.png";
		texture = IMG_LoadTexture(renderer, itemPath.c_str());
		itemType = i;
		Pickup = Mix_LoadWAV((audioPath + "pickup.wav").c_str());
	} else if (i == 5) {
		itemPath = dirPath + "lvl2Gate.png";
		texture = IMG_LoadTexture(renderer, itemPath.c_str());
		itemType = i;
		Pickup = Mix_LoadWAV((audioPath + "pickup.wav").c_str());
	}

	


	active = false;

	posRect.x = x;
	posRect.y = y;

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	pos_X = x;
	pos_Y = y;


}

void Item::Draw(SDL_Renderer *renderer){

	SDL_RenderCopy(renderer, texture, NULL, &posRect);

}

void Item::Reset(){

	posRect.x = -500;

	pos_X = posRect.x;

	active = false;

	Mix_PlayChannel(-1, Pickup, 0);

}

void Item::Drop(int x, int y){

	if (active == false){

		active = true;
		posRect.y = y;
		posRect.x = x;
	}

}

void Item::MoveToHUD(int x, int y){

			posRect.y = y;
			posRect.x = x;
			pos_X = posRect.x;
			pos_Y = posRect.y;


}
