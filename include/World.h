#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <windows.h>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/glew.h>

#include <iostream>
#include <vector>
#include <thread>

#include "Noise.h"
#include "Shader.h"
#include "Texture.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 uv;
};

class World {
public:
	World();
	~World();

	/*
	Generates the terrain using the 3D Marching Squares algorithm
	*/
	void generate();

	/*
	Renders the terrain
	*/
	void render(Shader*, Texture*, glm::mat4, glm::mat4);

	Noise* getNoise();

	void setNumChunks(int);
protected:
private:
	Noise* noise;

	int numChunksX_, numChunksZ_;
	int chunkSize_;

	std::vector<Vertex> vertices_;
	std::vector<glm::uvec3> indices_;

	GLuint vbo, ibo, vao;
};