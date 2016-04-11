#include"fire.h"

Fire::Fire(SDL_Renderer *renderer, string filePath, float x, float y){

	string flamePath = filePath + "flame_0Test.png";

	// Initialize frame counter
	frameCounter = 1.0f;

	// create a SDL texture - title
	texture = IMG_LoadTexture(renderer, flamePath.c_str());

	textureWidth = 4096;
	textureHeight = 2048;

	// Setting the individual image size
	posRect.w = 344;
	posRect.h = 344;

	//set the x and y positions of the explosion rectangle
	posRect.x = x;
	posRect.y = y;

	// Frame W and H of each animation image
	frameWidth = 344;
	frameHeight = 344;

	// Original draw Rect used to draw one frame of the animation strip
	drawRect.x = 0;
	drawRect.y = 0;
	drawRect.w = frameWidth;
	drawRect.h = frameHeight;

}

void Fire::Update(float deltaTime) {

	// Add to frame counter
	frameCounter += deltaTime;

	if (frameCounter > .0001) {

		// Add to the drawRect the W of an individual frame to move the drawing area over one
		drawRect.x += frameWidth;

		cout << drawRect.x << endl;

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
