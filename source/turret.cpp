#include"turret.h"


// Turrets creation method
Turret::Turret(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y) {

	// Activate turret
	active = true;
	killed = false;

	health = 4;

	// Load tank fire sound
	fire = Mix_LoadWAV((audioPath + "fire.wav").c_str());


	// Create file path for texture
	string basePath = filePath + "turretBase.png";

	// Load image to texture
	tBase = IMG_LoadTexture(renderer, basePath.c_str());

	// Create file path for texture
	string barrelPath = filePath + "turretBarrel.png";

	// Load image to texture
	tBarrel = IMG_LoadTexture(renderer, barrelPath.c_str());

	// Create file path for texture
	string roofPath = filePath + "turretRoof.png";

	// Load image to texture
	tRoof = IMG_LoadTexture(renderer, roofPath.c_str());


	// Set the SDL_rect x and y for the base image
	baseRect.x = x;
	baseRect.y = y;

	// Use QueryTexture to get W and H of turretBase texture
	int w, h;
	SDL_QueryTexture(tBase, NULL, NULL, &w, &h);
	baseRect.w = w;
	baseRect.h = h;

	// Set the SDL_rect x and y for the base image
	barrelRect.x = x + 88;
	barrelRect.y = y + 65;


	// Use QueryTexture to get W and H of turretBarrel texture
	SDL_QueryTexture(tBarrel, NULL, NULL, &w, &h);
	barrelRect.w = w;
	barrelRect.h = h;

	// Center of the barrel texture for rotation
	center.x = 0;
	center.y = 0;


	// Use QueryTexture to get W and H of turretBarrel texture
	SDL_QueryTexture(tRoof, NULL, NULL, &w, &h);
	roofRect.w = w;
	roofRect.h = h;

	// Set the SDL_rect x and y for the base image
	roofRect.x = x+16;
	roofRect.y = y+15;

	// String for Bullet
	string bulletPath = filePath + "turretBullet.png";

	// Create Bullet Pool
	for (int i = 0; i < 5; i++) {
		TurretBullet tmpBullet(renderer, bulletPath, 1500, 1500);
		bulletList.push_back(tmpBullet);
	}

	// Random null init
	srand(time(NULL));

	// Update the float values for movement
	posB_X = baseRect.x;
	posB_Y = baseRect.y;

	posT_X = barrelRect.x;
	posT_Y = barrelRect.y;

	posR_X = roofRect.x;
	posR_Y = roofRect.y;

}

// tank moves the turret in X
void Turret::TankMoveX(float tankSpeed, float deltaTime) {

	posB_X += (tankSpeed) * deltaTime;
	posT_X += (tankSpeed) * deltaTime;
	posR_X += (tankSpeed) * deltaTime;


	// Update bullet pos with codeto account for precision loss
	baseRect.x = (int) (posB_X + 0.5f);
	barrelRect.x = (int) (posT_X + 0.5f);
	roofRect.x = (int) (posR_X + 0.5f);


}

// tank moves the turret in Y
void Turret::TankMoveY(float tankSpeed, float deltaTime) {

	posB_Y += (tankSpeed) * deltaTime;
	posT_Y += (tankSpeed) * deltaTime;
	posR_Y += (tankSpeed) * deltaTime;


	// Update bullet pos with codeto account for precision loss
	baseRect.y = (int) (posB_Y + 0.5f);
	barrelRect.y = (int) (posT_Y + 0.5f);
	roofRect.y = (int) (posR_Y + 0.5f);

}

// Draw the player main's passed in renderer
void Turret::Draw(SDL_Renderer *renderer) {

	// Draw Bullets
	for (int i = 0; i < bulletList.size(); i++) {
		if (bulletList[i].active) {
			bulletList[i].Draw(renderer);
		}
	}

	// Draw the base
	SDL_RenderCopy(renderer, tBase, NULL, &baseRect);

	// Draw the barrel texture using the vars texture and posRect
	SDL_RenderCopyEx(renderer, tBarrel, NULL, &barrelRect, turretangle, &center,
			SDL_FLIP_NONE);

	// Draw the base
	SDL_RenderCopy(renderer, tRoof, NULL, &roofRect);

}

// Update player using the passed in deltaTime
void Turret::Update(float deltaTime, SDL_Rect tankRect) {

	// Get the angle between the tank and the turret
	x = (tankRect.x + (tankRect.w / 2)) - (baseRect.x + (baseRect.w / 2));
	y = (tankRect.y + (tankRect.h / 2)) - (baseRect.y + (baseRect.h / 2));
	turretangle = atan2(y, x) * 180 / 3.14;

	if (SDL_GetTicks() > fireTime) {

		if (baseRect.x > 0 && baseRect.x < 1024 && baseRect.y > 0
				&& baseRect.y < 768) {
			CreateBullet(tankRect);
		}

		fireTime = SDL_GetTicks() + (rand() % 5 + 1) * 1000;

	}

	//Update the players bullets
	for (int i = 0; i < bulletList.size(); i++) {
		if (bulletList[i].active) {
			bulletList[i].Update(deltaTime);
		}
	}

}

void Turret::CreateBullet(SDL_Rect target) {

	//If there is a active bullet to fire
	for (int i = 0; i < bulletList.size(); i++) {

		// If the bullet is not active
		if (bulletList[i].active == false) {

			bulletList[i].Start(target, baseRect);

			// Play the Fire sound - plays fine through levels, must pause for QUIT
			Mix_PlayChannel(-1, fire, 0);

			// Set bullet to active
			bulletList[i].active = true;

			//			 Use the same math in the x position to get the bullet close to the center of the player using player width
			bulletList[i].posRect.x = ((baseRect.x + (baseRect.w / 2)) - (bulletList[i].posRect.w / 2));
			bulletList[i].posRect.y = ((baseRect.y + (baseRect.h / 2)) - (bulletList[i].posRect.h / 2));

			// Set the X and Y pos of the bullets floats positions
			bulletList[i].pos_X = bulletList[i].posRect.x;
			bulletList[i].pos_Y = bulletList[i].posRect.y;

			// Bullet is found, break from loop
			break;
		}
	}
}

void Turret::RemoveHealth(){

	health -= 1;

	if (health <= 0){
		killed = true;
	}
}

void Turret::Reset(){

	baseRect.x = -1000;
	barrelRect.x = -1000;
	roofRect.x = -1000;

	baseRect.y = -1000;
	barrelRect.y = -1000;
	roofRect.y = -1000;

	posB_X = baseRect.x;
	posB_Y = baseRect.y;

	posT_X = barrelRect.x;
	posT_Y = barrelRect.y;

	posR_X = roofRect.x;
	posR_Y = roofRect.y;

	active = false;
}

void Turret::levelReset(float x, float y){

	baseRect.x = x;
		barrelRect.x = x + 88;
		roofRect.x = x + 16;

		baseRect.y = y;
		barrelRect.y = y + 65;
		roofRect.y = y + 15;

		posB_X = baseRect.x;
		posB_Y = baseRect.y;

		posT_X = barrelRect.x;
		posT_Y = barrelRect.y;

		posR_X = roofRect.x;
		posR_Y = roofRect.y;

		active = true;
}
