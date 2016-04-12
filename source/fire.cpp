#include"fire.h"

Fire::Fire(SDL_Renderer *renderer, string filePath, int i, float x, float y){

	string flamePath = "o";

	if (int i = 0) {
		 flamePath = filePath + "flame_0Test.png";
	}
	else {
		 flamePath = filePath + "flame_0Big.png";
	}

	// Initialize frame counter
	frameCounter = 1.0f;

	// create a SDL texture - title
	texture = IMG_LoadTexture(renderer, flamePath.c_str());

	if (int i = 0) {

		textureWidth = 4080;
		textureHeight = 2040;

		// Setting the individual image size
		posRect.w = 340;
		posRect.h = 340;

		// Frame W and H of each animation image
		frameWidth = 340;
		frameHeight = 340;

	}
	else {

		textureWidth = 5304;
		textureHeight = 2652;

		// Setting the individual image size
		posRect.w = 442;
		posRect.h = 442;

		// Frame W and H of each animation image
		frameWidth = 442;
		frameHeight = 442;

	}
	
	//set the x and y positions of the explosion rectangle
	posRect.x = x;
	posRect.y = y;

	// Original draw Rect used to draw one frame of the animation strip
	drawRect.x = 0;
	drawRect.y = 0;
	drawRect.w = frameWidth;
	drawRect.h = frameHeight;

}

void Fire::Update(float deltaTime) {

	// Add to frame counter
	frameCounter += deltaTime;

	if (frameCounter > .5f) {

		// Add to the drawRect the W of an individual frame to move the drawing area over one
		drawRect.x += frameWidth;

		// See if we have gone past the edge of the image - W
		if (drawRect.x >= textureWidth) {

			drawRect.y += frameHeight;

			// If so, go back to the first = 0
			drawRect.x = 0;

			if (drawRect.y >= textureHeight){

				drawRect.y = 0;
			}
		}
	}
}



void Fire::Draw(SDL_Renderer *renderer) {

	SDL_RenderCopy(renderer, texture, &drawRect, &posRect);

}
