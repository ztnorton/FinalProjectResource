#include"explode.h"

// Creation Method
Explode::Explode(SDL_Renderer *renderer, string filePath, float x, float y) {

	// Set the explode initial state
	active = false;

	// Get the directory path of file
	string explodePath = filePath + "explode.png";

	// Initialize frame counter
	frameCounter = 0.0f;

	// create a SDL texture - title
	texture = IMG_LoadTexture(renderer, explodePath.c_str());

	// Set the width and height of the explosion's rectangle by querying the texture itself
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	// Get the W and H of entire image
	textureWidth = w;
	textureHeight = h;

	// Setting the individual image size
	posRect.w = 128;
	posRect.h = 128;

	//set the x and y positions of the explosion rectangle
	posRect.x = x;
	posRect.y = y;

	// Frame W and H of each anaimation image
	frameWidth = 128;
	frameHeight = 128;

	// Original draw Rect used to draw one frame of the animation strip
	drawRect.x = 128;
	drawRect.y = 0;
	drawRect.w = frameWidth;
	drawRect.h = frameHeight;

}

// Reset Explode
void Explode::Reset() {
	// Deactivate the Explode
	active = false;
}

void Explode::Update(float deltaTime) {

	if (active) {

	

		// Add to frame counter
		frameCounter += deltaTime;

		if (frameCounter >= .05f) {

			// Add to the drawRect the W of an individual fram to move the drawing area over one
			drawRect.x += frameWidth;

			// See if we have gone pase the edge of the image - W
			if (drawRect.x >= textureWidth) {

				drawRect.y += frameHeight;

				// If so, go back to the first = 0
				drawRect.x = 0;

				if (drawRect.y >= textureHeight) {

					drawRect.y = 0;

					// Deactivate the Explode
					active = false;

					// Move offscreen
					posRect.x = -1000;
					posRect.y = -1000;
				}
			}
		}
	}
}

// Explode Draw
void Explode::Draw(SDL_Renderer *renderer) {
	SDL_RenderCopy(renderer, texture, &drawRect, &posRect);
}

void Explode::makeExplosion(int x, int y) {
	
	posRect.x = x;
	posRect.y = y;

	active = true;

}
