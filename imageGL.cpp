#include "imageGL.h"
#include <fstream>
#include <cstdint>

void ImageGL::setupImage(const char* filePath) {
	int count = 0;
	for ( ; filePath[count] != '\0'; ++count) {}
	++count;
	count += 4;
	char* imagePath = new char[count];
	for (int a = 0; a < count - 5; ++a) {
		imagePath[a] = filePath[a];
	}
	imagePath[count - 5] = '.';
	imagePath[count - 4] = 'i';
	imagePath[count - 3] = 'm';
	imagePath[count - 2] = 'g';
	imagePath[count - 1] = '\0';
	std::ifstream readFile;
	readFile.open(imagePath, std::ios::in | std::ios::beg | std::ios::binary);
	readFile.read(reinterpret_cast<char*>(&width), sizeof(uint16_t));
	readFile.read(reinterpret_cast<char*>(&height), sizeof(uint16_t));
	pixels = new uint8_t[width * height * 4];
	readFile.read(reinterpret_cast<char*>(pixels), sizeof(uint8_t) * (width * height * 4));
	readFile.close();
	readFile.clear();
	delete[] imagePath;
}

void ImageGL::writePixels(uint8_t* screen, int x, int y, int screenWidth, int screenHeight) { 
	bool transparent = false;
	size_t startpos = (y * (screenWidth * 4)) + (x * 4);
	for (int a = 0; a < height; ++a) {
		size_t startpos2 = startpos + ((a * (screenWidth * 4)));
		size_t startpos3 = a * (width * 4);
		for (int b = 0; b < width * 4; ++b) {
			transparent = false;
			if ((b == 0 || b % 4 == 0) && pixels[startpos3 + b + 3] == 0) {
				transparent = true;
			}
			if (!transparent)
				screen[startpos2 + b] = pixels[startpos3 + b];
			else {
				b += 3;
			}
		}
	}
}