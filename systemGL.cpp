#include "systemGL.h"
#include <cstddef>
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>
#include "shader.h"
#include "texAttribute.h"
#include "imageGL.h"

SystemGL::SystemGL(int screenWidth, int screenHeight, const char* programName) {
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
	image = NULL;
	imageNum = 0;
}

SystemGL::~SystemGL() {
  glDeleteTextures(1, &texture);
  shader->shutdown();
  delete shader;
  texAttribute.shutdown();
  SDL_DestroyWindow(window);
  window = NULL;
  SDL_Quit();
  delete[] screen;
  delete[] image;
}

void SystemGL::render() {
	for (int a = 0; a < imageNum; ++a) {
		image[a].writePixels(screen, 0, 0, screenWidth, screenHeight);
	}
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, screen);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(window);
}