#include "imageSDL.h"
#include <SDL.h>
#include <SDL_image.h>

void ImageSDL::setupImage(const char* filePath, SDL_Renderer* renderer) {
	int count = 0;
	for (; filePath[count] != '\0'; ++count) {}
	++count;
	count += 4;
	char* imagePath = new char[count];
	for (int a = 0; a < count - 5; ++a) {
		imagePath[a] = filePath[a];
	}
	imagePath[count - 5] = '.';
	imagePath[count - 4] = 'p';
	imagePath[count - 3] = 'n';
	imagePath[count - 2] = 'g';
	imagePath[count - 1] = '\0';
	SDL_Surface* surface = IMG_Load(imagePath);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	delete imagePath;
}
