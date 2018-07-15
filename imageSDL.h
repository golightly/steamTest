#pragma once

#include <SDL.h>

class ImageSDL {
private:
	SDL_Texture* texture;
public:
	void setupImage(const char* filePath, SDL_Renderer* renderer);
	inline ~ImageSDL() { SDL_DestroyTexture(texture); }
	inline SDL_Texture* getTexture() { return texture; }
};