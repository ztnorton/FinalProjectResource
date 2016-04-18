#include"zombie.h"

Zombie::Zombie(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y){

	active = false;

	//death = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	string basePath = filePath + "zombie.png";

	zombie = IMG_LoadTexture(renderer, basePath.c_str());

	// SDL_Rect for X Y image
	zombieRect.x = x;
	zombieRect.y = y;

	// Get W and H of Texture
	int w,h;
	SDL_QueryTexture(zombie, NULL, NULL, &w, &h);
	zombieRect.w = w;
	zombieRect.h = h;

	posZ_X = zombieRect.x;
	posZ_Y = zombieRect.y;

	speed = 50;

	center.x = 26;
	center.y = 25;

	health = 8;

}

void Zombie::Update(float deltaTime, SDL_Rect playerRect){

	double distancex = (zombieRect.x - playerRect.x) * (zombieRect.x - playerRect.x);
	double distancey = (zombieRect.y - playerRect.y) * (zombieRect.y - playerRect.y);

	double calcdistance = sqrt(distancex + distancey);

	if (calcdistance <= 450){
		active = true;
	}else{
		active = false;
	}

	if (active){

		// Get angle between zombie and player
		x = (playerRect.x + (playerRect.w / 2)) - (zombieRect.x + (zombieRect.w / 2));
		y = (playerRect.y + (playerRect.h / 2)) - (zombieRect.y + (zombieRect.h / 2));
		tankangle = atan2(y, x) * 180 / 3.14;

		float radians = (tankangle * 3.14) / 180;

		// Get X and Y values to move
		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		// Update floats for precision loss
		posZ_X += (move_x) * deltaTime;
		posZ_Y += (move_y) * deltaTime;

		// Update zombie's position for precision loss
		zombieRect.x = (int) (posZ_X + 0.5f);
		zombieRect.y = (int) (posZ_Y + 0.5f);
	}

}

void Zombie::RemoveHealth(){

	health = health - 1;

	if (health <= 0){
		Reset();
	}
}


void Zombie::Reset(){

	int kX = (rand() % 1024);

	int kY = (rand() % 768);

	zombieRect.x = kX;

	posZ_X = zombieRect.x;

	zombieRect.y = kY;

	posZ_Y = zombieRect.y;

	health = 6;

	active = false;
}


void Zombie::zombieMoveX(float tankSpeed, float deltaTime){

	posZ_X += (tankSpeed) * deltaTime;

	zombieRect.x = (int)(posZ_X + .05f);
}


void Zombie::zombieMoveY(float tankSpeed, float deltaTime){

	posZ_Y += (tankSpeed) * deltaTime;

	zombieRect.y = (int)(posZ_Y + .05f);
}

void Zombie::Draw(SDL_Renderer *renderer){

	SDL_RenderCopyEx(renderer, zombie, NULL, &zombieRect, tankangle, &center, SDL_FLIP_NONE);
}
