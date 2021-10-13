#include "Texture.h"

Texture::Texture(std::string name) {
	std::string file = std::string(DIRECTORY) + name;
	GLuint tex = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	if (!tex) texture_ = -1;
	else texture_ = tex;
}

Texture::~Texture() {

}

GLuint Texture::getTexture() {
	return texture_;
}
