#pragma once

#include <cstddef>
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>
#include "shader.h"
#include "texAttribute.h"
#include "imageGL.h"

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
	  Overhead(int screenWidth, int screenHeight, const char* programName);
	  ~Overhead();
	  void render(int imageNum, Image* image);
};