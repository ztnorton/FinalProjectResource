#include "bullet.h"

PlayerBullet::PlayerBullet(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY){

	active = true;

	speed = 900.0f;

	texture = IMG_LoadTexture(renderer, filePath.c_str());

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	posRect.x = x - (posRect.w / 2);
	posRect.y = y;

	xDir = dirX;
	yDir = dirY;

}

void PlayerBullet::Update(float deltaTime){

}

void PlayerBullet::Draw(SDL_Renderer *renderer){

}

void PlayerBullet::Reset(){

}
