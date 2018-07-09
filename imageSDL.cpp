#include "definitions.h"
#include <SDL.h>
#include <SDL_image.h>

void Image::setupImage(const char* filePath, SDL_Renderer* renderer) {
	SDL_Surface* surface = IMG_Load(filePath);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}
