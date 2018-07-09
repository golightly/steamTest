#pragma once

#ifdef OPENGLPROGRAM

#define IMAGE_SETUP image[a].setupImage(imagePath[a]);

#include <cstddef>
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>
#include <fstream>
#include <string>
#include "shader.h"
#include "texAttribute.h"

class Image {
private:
	uint16_t width, height;
	uint8_t* pixels;
public:
	inline void setupImage(const char* filePath) {
		std::ifstream readFile;
		std::string imagePath = filePath;
		imagePath += ".img";
		readFile.open(imagePath, std::ios::in | std::ios::beg | std::ios::binary);
		readFile.read(reinterpret_cast<char*>(&width), sizeof(uint16_t));
		readFile.read(reinterpret_cast<char*>(&height), sizeof(uint16_t));
		pixels = new uint8_t[width * height * 4];
		readFile.read(reinterpret_cast<char*>(pixels), sizeof(uint8_t) * (width * height * 4));
		readFile.close();
		readFile.clear();
	}
	inline ~Image() { delete[] pixels; }
	inline void writePixels(uint8_t* screen, int x, int y, int screenWidth, int screenHeight) {
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
	inline uint16_t getHeight() { return height; }
	inline uint16_t getWidth() { return width; }
};

class Overhead {
private:
	SDL_GLContext context;
	SDL_Window* window;
	TexAttribute texAttribute;
	GLuint texture;
	Shader* shader;
	int screenWidth, screenHeight;
	uint8_t* screen;
public:
	inline Overhead(int screenWidth, int screenHeight, const char* programName) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		screen = new uint8_t[screenWidth * screenHeight * 4];
		SDL_Init(SDL_INIT_VIDEO);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		window = SDL_CreateWindow(programName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		context = SDL_GL_CreateContext(window);
		glewExperimental = GL_TRUE;
		glewInit();
		SDL_GL_SetSwapInterval(1);
		texAttribute.setupTexAttribute();
		shader = new Shader(getVertexSource(), getFragmentSource());
		shader->setupShader();
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, screen);
		shader->uniformFunction();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	inline ~Overhead() {
		glDeleteTextures(1, &texture);
		shader->shutdown();
		delete shader;
		texAttribute.shutdown();
		SDL_DestroyWindow(window);
		window = NULL;
		SDL_Quit();
		delete[] screen;
	}
	inline void render(int imageNum, Image* image) {
		for (int a = 0; a < imageNum; ++a) {
			image[a].writePixels(screen, 0, 0, screenWidth, screenHeight);
		}
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, screen);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		SDL_GL_SwapWindow(window);
	}
};

#endif

#ifdef SDLPROGRAM

#define IMAGE_SETUP image[a].setupImage(imagePath[a], overhead);

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <fstream>
#include <iostream>

class Image;

class Overhead {
private:
	SDL_Window * window;
public:
	int screenWidth, screenHeight;
	SDL_Renderer * renderer;
	Overhead(int screenWidth, int screenHeight, const char* programName);
	~Overhead();
	void render(int imageNum, Image* image);
	//inline SDL_Renderer* getRenderer() { return renderer; }
};

class Image {
private:
public:
	SDL_Texture * texture;
	void setupImage(const char* filePath, Overhead& overhead);
	inline ~Image() { SDL_DestroyTexture(texture); }
	//inline SDL_Texture* getTexture() { return texture; }
};

void Image::setupImage(const char* filePath, Overhead& overhead) {
	texture = NULL;
	if (overhead.renderer == NULL) {
		std::cout << "renderer is null" << std::endl;
		std::ofstream writeFile;
		writeFile.open("errorLog.txt", std::ios::app);
		writeFile << "renderer is null\n";
		writeFile.close();
		writeFile.clear();
	}
	std::cout << "window width = " << overhead.screenWidth << std::endl;
	std::cout << "window height = " << overhead.screenHeight << std::endl;
	std::ofstream writeFile;
	writeFile.open("errorLog.txt", std::ios::app);
	writeFile << "width: " << overhead.screenWidth << '\n';
	writeFile << "height: " << overhead.screenHeight << '\n';
	writeFile.close();
	writeFile.clear();
	std::string imagePath = filePath;
	imagePath += ".png";
	imagePath = "C:\\Users\\Nick\\Desktop\\testSDK\\steamworksexample\\win64\\release\\test.png";
	SDL_Surface* surface = IMG_Load(imagePath.c_str());
	if (surface == NULL) {
		std::cout << "surface is null" << std::endl;
		std::ofstream writeFile;
		writeFile.open("errorLog.txt", std::ios::app);
		writeFile << "surface is null\n";
		writeFile.close();
		writeFile.clear();
	}
	texture = SDL_CreateTextureFromSurface(overhead.renderer, surface);
	if (texture == NULL) {
		std::cout << "texture is null" << std::endl;
		std::ofstream writeFile;
		writeFile.open("errorLog.txt", std::ios::app);
		writeFile << "error creating texture\n";
		writeFile.close();
		writeFile.clear();
	}
	SDL_FreeSurface(surface);
}

void Overhead::render(int imageNum, Image* image) {
	if (renderer == NULL) {
		std::cout << "null renderer in rendering function" << std::endl;
		std::ofstream writeFile;
		writeFile.open("errorLog.txt", std::ios::app);
		writeFile << "null renderer in rendering function\n";
		writeFile.close();
		writeFile.clear();
	}
	else if (renderer != NULL) {
		std::cout << "working renderer in rendering function" << std::endl;
		std::ofstream writeFile;
		writeFile.open("errorLog.txt", std::ios::app);
		writeFile << "working renderer in rendering function\n";
		writeFile.close();
		writeFile.clear();
	}
	SDL_RenderClear(renderer);
	for (int a = 0; a < imageNum; ++a) {
		if ((SDL_RenderCopy(renderer, image[a].texture, NULL, NULL)) < 0) {
			std::cout << "error rendering " << SDL_GetError() << std::endl;
			std::ofstream writeFile;
			writeFile.open("errorLog.txt", std::ios::app);
			writeFile << "error rendering\n";
			writeFile.close();
			writeFile.clear();
		}
		if (image[a].texture == NULL) {
			std::cout << "null texture" << std::endl;
			std::ofstream writeFile;
			writeFile.open("errorLog.txt", std::ios::app);
			writeFile << "null texture\n";
			writeFile.close();
			writeFile.clear();
		}
	}
	SDL_RenderPresent(renderer);
}

Overhead::Overhead(int screenWidth, int screenHeight, const char* programName) {
	renderer = NULL;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	SDL_Window* window = SDL_CreateWindow(programName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer != NULL) {
		std::cout << "created renderer fine" << std::endl;
		std::ofstream writeFile;
		writeFile.open("errorLog.txt", std::ios::app);
		writeFile << "created renderer fine\n";
		writeFile.close();
		writeFile.clear();
	}
	SDL_SetRenderDrawColor(renderer, 100, 0, 0, 0);
	if (renderer != NULL) {
		std::cout << "draw color fine" << std::endl;
		std::ofstream writeFile;
		writeFile.open("errorLog.txt", std::ios::app);
		writeFile << "draw color fine\n";
		writeFile.close();
		writeFile.clear();
	}
}
Overhead::~Overhead() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}
#endif