#pragma once

#include <cstddef>
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>
#include "shader.h"
#include "texAttribute.h"
#include "imageGL.h"

class SystemGL {
  private:
    SDL_GLContext context;
    SDL_Window* window;
    TexAttribute texAttribute;
    GLuint texture;
    Shader* shader;
    int screenWidth, screenHeight;
	uint8_t* screen;
	ImageGL* image;
	int imageNum;
  public:
	  SystemGL(int screenWidth, int screenHeight, const char* programName);
	  ~SystemGL();
	  void render();
	  inline void initImage(int imageNum) { this->imageNum = imageNum; image = new ImageGL[imageNum]; }
	  inline void setupImage(const char* filePath, int index) { image[index].setupImage(filePath); }
	  inline void writePixels(int x, int y, int index) { image[index].writePixels(screen, x, y, screenWidth, screenHeight); }
	  inline uint16_t getImageHeight(int index) { return image[index].getHeight(); }
	  inline uint16_t getImageWidth(int index) { return image[index].getWidth(); }
};