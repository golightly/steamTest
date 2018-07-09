#pragma once

#include <cstdint>

class Image {
private:
	uint16_t width, height;
	uint8_t* pixels;
public:
	void setupImage(const char* filePath);
	inline ~Image() { delete[] pixels; }
	void writePixels(uint8_t* screen, int x, int y, int screenWidth, int screenHeight);
	inline uint16_t getHeight() { return height; }
	inline uint16_t getWidth() { return width; }
};