#include "player.h"


// Analog joy stick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y) {

	back = IMG_LoadTexture(renderer, (filePath + "HPback.png").c_str());
	mid = IMG_LoadTexture(renderer, (filePath + "HPmid.png").c_str());
	front = IMG_LoadTexture(renderer, (filePath + "HPfront.png").c_str());

	backR.x = midR.x = frontR.x = 10;
	backR.y = midR.y = frontR.y = 10;
	backR.w = midR.w = frontR.w = 324;
	backR.h = midR.h = frontR.h = 42;

	aMid = IMG_LoadTexture(renderer, (filePath + "ammoMid.png").c_str());
	aFront = IMG_LoadTexture(renderer, (filePath + "ammoFront.png").c_str());

	aMidR.x = 796; aFrontR.x = 781;
	aMidR.y = 12;  aFrontR.y = 10;
	aMidR.w = 172; aFrontR.w = 190;
	aMidR.h = 53;  aFrontR.h = 57;

	key1 = key2 = gen1 = gen2 = gen3 = gen4 = false;

	// Player Health
	playerHealth = 100.0f;
	maxHealth = 100.0f;
	ammo = 30;
	maxAmmo = 30;

	active = true;

	speed = 200.0f;

	xangle = 0; yangle = 0; playerangle = 0; oldAngle = 0; Xvalue = 0; Yvalue = 0;

	fire = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	playerPath = filePath + "player.png";

	texture = IMG_LoadTexture(renderer, playerPath.c_str());

	posRect.x = x;
	posRect.y = y;

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	pos_X = x;
	pos_Y = y;

	yDir = 0;
	xDir = 0;

	// Set initial values so the player can shoot
	xDirOld = 1;
	yDirOld = 0;

	// Center of the image Rectangle
	center.x = posRect.w / 2;
	center.y = posRect.h / 2;

	string bulletPath;

	bulletPath = filePath + "bullet.png";

	// Create Bullet Pool
	for (int i = 0; i < 20; i++) {
		PlayerBullet tmpBullet(renderer, bulletPath, -1000, -1000, 0, 0);
		bulletList.push_back(tmpBullet);
	}

}

void Player::Update(float deltaTime) {

	// Check for game pad input
	if (Xvalue != 0 || Yvalue != 0) {
		// Get the angle between the tank and turret
		playerangle = atan2(Yvalue, Xvalue) * 180 / 3.14;
		// Set this as the old angle and dir so the player can shoot when stopped
		oldAngle = playerangle;

		// Gives us radians
		float radians = (playerangle * 3.14) / 180;

		// Get, need X and Y to move
		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		// Update floats for precision
		pos_X += (move_x)* deltaTime;
		pos_Y += (move_y)* deltaTime;

		// Update player position with code to account for precision loss
		posRect.x = (int)(pos_X + 0.5f);
		posRect.y = (int)(pos_Y + 0.5f);

	}
	else {
		playerangle = oldAngle;
	}

	// Check to prevent tank from leaving the screen
	if (posRect.x < 0) {
		posRect.x = 0;
		pos_X = posRect.x;
	}
	if (posRect.x > 1024 - posRect.w) {
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}
	if (posRect.y < 0) {
		posRect.y = 0;
		pos_Y = posRect.y;
	}
	if (posRect.y > 768 - posRect.h) {
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}

	//Update the players bullets
	for (int i = 0; i < bulletList.size(); i++) {
		if (bulletList[i].active) {
			bulletList[i].Update(deltaTime);
		}
	}

}

void Player::Draw(SDL_Renderer *renderer) {

	// Draw Bullets
	for (int i = 0; i < bulletList.size(); i++) {
		if (bulletList[i].active) {
			bulletList[i].Draw(renderer);
		}
	}

	// Draw the player texture using the vars texture and posRect
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, playerangle, &center, SDL_FLIP_NONE);

	// Draw Health bar
	SDL_RenderCopy(renderer, back, NULL, &backR);
	SDL_RenderCopy(renderer, mid, NULL, &midR);
	SDL_RenderCopy(renderer, front, NULL, &frontR);

	SDL_RenderCopy(renderer, aFront, NULL, &aFrontR);
	SDL_RenderCopy(renderer, aMid, NULL, &aMidR);
}

void Player::OnControllerAxis(Sint16 X, Sint16 Y) {

	Xvalue = X;
	Yvalue = Y;

	if (!(Xvalue < -JOYSTICK_DEAD_ZONE) && !(Xvalue > JOYSTICK_DEAD_ZONE)) {
		Xvalue = 0.0f;
	}
	if (!(Yvalue < -JOYSTICK_DEAD_ZONE) && !(Yvalue > JOYSTICK_DEAD_ZONE)) {
		Yvalue = 0.0f;
	}

}

void Player::OnControllerButton(const SDL_ControllerButtonEvent event) {

	//if josytick is 0
	if (event.which == 0) {
		//if A button
		if (event.button == 0) {

			//createBullet
			CreateBullet();
		}
	}

}

void  Player::CreateBullet() {

	if (ammo > 0) {

		ammo -= 1;

		// If there is a active bullet to fire
		for (int i = 0; i < bulletList.size(); i++) {

			// If the bullet is not active
			if (bulletList[i].active == false) {

				// Play the Fire sound - plays fine through levels, must pause for QUIT
				Mix_PlayChannel(-1, fire, 0);

				aMidR.w = ammo / maxAmmo * 172;

				// Set bullet to active
				bulletList[i].active = true;

				// Use the same math in the x position to get the bullet close to the center of the player using player width
				bulletList[i].posRect.x = (posRect.x + (posRect.w / 2));
				bulletList[i].posRect.y = (posRect.y + (posRect.h / 2));

				// Finishing aligning to the players center using the texture width
				bulletList[i].posRect.x = (bulletList[i].posRect.x
						- (bulletList[i].posRect.w / 2));
				bulletList[i].posRect.y = (bulletList[i].posRect.y
						- (bulletList[i].posRect.h / 2));

				// Set the x and y position of the bullets float position
				bulletList[i].pos_X = bulletList[i].posRect.x;
				bulletList[i].pos_Y = bulletList[i].posRect.y;

				// IF the tank is moving, fire in that Direction
				if (Xvalue != 0 || Yvalue != 0) {
					// Set the x and y pos of the bullet's float pos
					bulletList[i].playerangle = playerangle;

				}
				else {
					// If the tank isnt moving, fire the direction currently facing & Set the x and y pos of the bullet's float pos
					bulletList[i].playerangle = oldAngle;

				}

				// Once bullet is found, break out of loop
				break;
			}
		}
	}
}

void Player::Reset() {

	key1 = key2 = gen1 = gen2 = gen3 = gen4 = false;

	playerHealth = 100.0f;

	ammo = 30;

	midR.w = playerHealth / maxHealth * 324;
	aMidR.w = ammo / maxAmmo * 172;

	posRect.x = 950.0f;
	posRect.y = 400.0f;

	pos_X = posRect.x;
	pos_Y = posRect.y;

}

void Player::eZombieHit() {

	playerHealth -= .003f;

	midR.w = playerHealth / maxHealth * 324;
}

void Player::eBulletHit() {

	playerHealth -= 5;

	midR.w = playerHealth / maxHealth * 324;
}

void Player::GiveHealth() {

	playerHealth = 100 ;

	midR.w = playerHealth / maxHealth * 324;
}

void Player::GiveAmmo() {

	ammo = 30;

	aMidR.w = ammo / maxAmmo * 172;

}
