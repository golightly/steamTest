#pragma once

#include <SDL.h>
#include "imageSDL.h"

class SystemSDL {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	int screenWidth, screenHeight;
	ImageSDL* image;
	int imageNum;
public:
	SystemSDL(int screenWidth, int screenHeight, const char* programName);
	~SystemSDL();
	void render();
	inline SDL_Renderer* getRenderer() { return renderer; }
	inline void initImage(int imageNum) { this->imageNum = imageNum; image = new ImageSDL[imageNum]; }
	inline void setupImage(const char* filePath, int index) { image[index].setupImage(filePath, renderer); }
};