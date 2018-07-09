#pragma once

#include <SDL.h>

class Image {
private:
	SDL_Texture* texture;
public:
	void setupImage(const char* filePath, SDL_Renderer* renderer);
	inline ~Image() { SDL_DestroyTexture(texture); }
	inline SDL_Texture* getTexture() { return texture; }
};