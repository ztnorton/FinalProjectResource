#include "turretbullet.h"

// Bullet creation method
TurretBullet::TurretBullet(SDL_Renderer *renderer, string filePath, float x,
		float y) {

	// Set the bullet initial state
	active = false;

	// Set bullet speed
	speed = 500.0;

	// Create texture
	texture = IMG_LoadTexture(renderer, filePath.c_str());

	// set the width and height of the bullet's rectangle
	// by querying the texture itself
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	// set the x and y positions of the bullet rectangle
	// finishing aligning to the player center using the texture width
	posRect.x = x;
	posRect.y = y;

	// set the float postion values of the texure for percision loss
	pos_X = x;
	pos_Y = y;

}

void TurretBullet::Start(SDL_Rect playerPos, SDL_Rect turretPos) {

	player.x = playerPos.x;
	player.y = playerPos.y;

	turret.x = turretPos.x;
	turret.y = turretPos.y;

	distance = sqrt(
			(player.x - turret.x) * (player.x - turret.x)
					+ (player.y - turret.y) * (player.y - turret.y));
}

void TurretBullet::Reset() {

	// Reset the X postion off the screen
	posRect.x = -1000;

	// Update the pos_X for precsion
	pos_X = posRect.x;

	// Deactivate the bullet
	active = false;
}

void TurretBullet::Draw(SDL_Renderer *renderer) {
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

void TurretBullet::Update(float deltaTime) {

	// Get direction values to move bullet with
	float vX = ((player.x - 25) - turret.x) / distance;
	float vY = ((player.y - 25) - turret.y) / distance;

	//Adjust position floats based on speed direction (-1 for up), and delta Time
	pos_X += (speed * vX * deltaTime);
	pos_Y += (speed * vY * deltaTime);

	//Update bullet position with code to account for percision loss
	posRect.x = (int) (pos_X + .05f);
	posRect.y = (int) (pos_Y + .05f);

	//check to see fit he bullet is off the top of the screen
	//and deactivate and move off screen
	if ((posRect.y < (0 - posRect.h)) || (posRect.y > 768)
			|| (posRect.x < (0 - posRect.w)) || (posRect.x > 1024)) {
		Reset();
	}

}
