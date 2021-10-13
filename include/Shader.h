#pragma once

#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#define DIRECTORY "C:/Users/benwi/Documents/University/Third\ Year/COMP3009A\ -\ Computer\ Graphics/Final\ Project/Source/resource/"

class Shader {
public:
	Shader(std::string name);
	~Shader();

	void loadShaderMatrix(glm::mat4 matrix, std::string matName);
	
	GLuint getShader();

protected:
private:
	std::string loadTextFile(std::string file);

	GLuint shader_;
};