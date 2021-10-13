#include "Shader.h"

Shader::Shader(std::string name) {
	std::string vsFile = loadTextFile(DIRECTORY + name + ".vert");
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	
	const char* vsConst = vsFile.c_str();
	glShaderSource(vs, 1, &vsConst, NULL);
	glCompileShader(vs);

	GLint status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling vertex shader: ") + std::string(buffer)));
	}

	std::string fsFile = loadTextFile(DIRECTORY + name + ".frag");
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	const char* fsConst = fsFile.c_str();
	glShaderSource(fs, 1, &fsConst, NULL);
	glCompileShader(fs);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling fragment shader: ") + std::string(buffer)));
	}

	shader_ = glCreateProgram();
	glAttachShader(shader_, vs);
	glAttachShader(shader_, fs);
	glLinkProgram(shader_);

	glGetProgramiv(shader_, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shader_, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error linking shaders: ") + std::string(buffer)));
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader() {

}

void Shader::loadShaderMatrix(glm::mat4 matrix, std::string matName) {
	GLint shaderMat = glGetUniformLocation(shader_, matName.c_str());
	glUniformMatrix4fv(shaderMat, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string Shader::loadTextFile(std::string file) {
	std::ifstream stream;
	stream.open(file.c_str());

	if (stream.fail()) {
		throw(std::ios_base::failure(std::string("Error opening file ") + std::string(file)));
	}

	std::string content;
	std::string line;
	while (std::getline(stream, line)) {
		content += line + "\n";
	}

	stream.close();
	return content;
}

GLuint Shader::getShader() {
	return shader_;
}