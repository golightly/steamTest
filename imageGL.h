#pragma once

#include <cstdint>

class ImageGL {
private:
	uint16_t width, height;
	uint8_t* pixels;
public:
	void setupImage(const char* filePath);
	inline ~ImageGL() { delete[] pixels; }
	void writePixels(uint8_t* screen, int x, int y, int screenWidth, int screenHeight);
	inline uint16_t getWidth() { return width; }
	inline uint16_t getHeight() { return height; }
};