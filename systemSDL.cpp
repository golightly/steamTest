#include "systemSDL.h"
#include <SDL.h>
#include <SDL_image.h>
#include "imageSDL.h"

SystemSDL::SystemSDL(int screenWidth, int screenHeight, const char* programName) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow(programName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	IMG_Init(IMG_INIT_PNG);
}

SystemSDL::~SystemSDL() {
	delete[] image;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void SystemSDL::render() {
	SDL_RenderClear(renderer);
	for (int a = 0; a < imageNum; ++a) {
		SDL_RenderCopy(renderer, image[a].getTexture(), NULL, NULL);
	}
	SDL_RenderPresent(renderer);
}