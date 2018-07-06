#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "texAttribute.h"

void TexAttribute::setupTexAttribute() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	vertices = new GLfloat[16]{
		-1.0f,  1.0f, 0.0f, 0.0f, // Top-left, texture bottom left
		1.0f,  1.0f, 1.0f, 0.0f, // Top-right, texture bottom right
		1.0f, -1.0f, 1.0f, 1.0f, // Bottom-right, texture top right
		-1.0f, -1.0f, 0.0f, 1.0f  // Bottom-left, texture top right
	};
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &ebo);
	elements = new GLuint[6]{
		0,1,2,
		2,3,0
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, elements, GL_STATIC_DRAW);
}

void TexAttribute::shutdown() {
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	delete[] vertices;
	delete[] elements;
}
