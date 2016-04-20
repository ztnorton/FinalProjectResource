#include "bullet.h"

PlayerBullet::PlayerBullet(SDL_Renderer *renderer, string filePath, float x, float y, int dirX, int dirY){

	active = false;

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

	if (active) {

			// Gives us radians
			float radians = (playerangle * 3.14) / 180;

			// Get, need X and Y to move
			float move_x = speed * cos(radians);
			float move_y = speed * sin(radians);

			// Update floats for precision
			pos_X += (move_x) * deltaTime;
			pos_Y += (move_y) * deltaTime;

			//Update bullet position with code to account for precision loss
			posRect.x = (int) (pos_X + .05f);
			posRect.y = (int) (pos_Y + .05f);

			//check to see fit he bullet is off the top of the screen
			//and deactivate and move off screen
			if ((posRect.y < (0 - posRect.h)) || (posRect.y > 768)
					|| (posRect.x < (0 - posRect.w)) || (posRect.x > 1024)) {
				Reset();
			}
		}

}

void PlayerBullet::Draw(SDL_Renderer *renderer){

	SDL_RenderCopy(renderer, texture, NULL, &posRect);

}

void PlayerBullet::Reset(){

	posRect.x = -1000;
	pos_X = posRect.x;
	active = false;
}
