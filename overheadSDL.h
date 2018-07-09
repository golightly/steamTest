#pragma once

#include <SDL.h>
#include "imageSDL.h"

class Overhead {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	int screenWidth, screenHeight;
public:
	Overhead(int screenWidth, int screenHeight, const char* programName);
	~Overhead();
	void render(int imageNum, Image* image);
	inline SDL_Renderer* getRenderer() { return renderer; }
};