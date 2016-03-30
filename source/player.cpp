#include "player.h"


// Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y){

	active = true;

	speed = 200.0f;

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
	for (int i = 0; i < 10; i++) {
		PlayerBullet tmpBullet(renderer, bulletPath, -1000, -1000, 0, 0);
		bulletList.push_back(tmpBullet);
	}

}

void Player::Update(float deltaTime){

	// Check for gamepad input
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

	//		//Update the players bullets
	//		for (int i = 0; i < bulletList.size(); i++) {
	//			if (bulletList[i].active) {
	//				bulletList[i].Update(deltaTime);
	//			}
	//		}

}

void Player::Draw(SDL_Renderer *renderer){

	//	// Draw Bullets
	//	for (int i = 0; i < bulletList.size(); i++) {
	//		if (bulletList[i].active) {
	//			bulletList[i].Draw(renderer);
	//		}
	//	}

	// Draw the player texture using the vars texture and posRect
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, playerangle, &center, SDL_FLIP_NONE);


}

void Player::Reset(){


}

void Player::OnControllerAxis(Sint16 X, Sint16 Y){

	Xvalue = X;
	Yvalue = Y;

	if (!(Xvalue < -JOYSTICK_DEAD_ZONE) && !(Xvalue > JOYSTICK_DEAD_ZONE)) {
		Xvalue = 0.0f;
	}
	if (!(Yvalue < -JOYSTICK_DEAD_ZONE) && !(Yvalue > JOYSTICK_DEAD_ZONE)) {
		Yvalue = 0.0f;
	}

}

void Player::OnControllerButton(const SDL_ControllerButtonEvent event){

	cout << "fire" << endl;

	//if josytick is 0
			if (event.which == 0) {
				//if A button
				if (event.button == 0) {

					//createBullet
					//CreateBullet();
				}
			}

}