#pragma once

#define DIRECTORY "C:/Users/benwi/Documents/University/Third\ Year/COMP3009A\ -\ Computer\ Graphics/Final\ Project/Source/assets/"

#include <string>
#include <stdexcept>

#include <GL/glew.h>
#include <SOIL/SOIL.h>

class Texture {
public:
	Texture(std::string);
	~Texture();

	GLuint getTexture();

protected:
private:
	GLuint texture_;
};