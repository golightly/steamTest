#include "imageSDL.h"
#include <SDL.h>
#include <SDL_image.h>

void ImageSDL::setupImage(const char* filePath, SDL_Renderer* renderer) {
	SDL_Surface* surface = IMG_Load(filePath);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}
