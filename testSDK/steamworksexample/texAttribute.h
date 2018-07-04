#pragma once

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

class TexAttribute {
	private:
		GLuint vao, vbo, ebo;
		GLfloat* vertices;
		GLuint* elements;
	public:
		void setupTexAttribute();
		void shutdown();
};
