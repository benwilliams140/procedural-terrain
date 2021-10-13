#include "World.h"

World::World() : numChunksX_(16), numChunksZ_(16), chunkSize_(16) {
	noise = new Noise(0.2, 4, 2);
	generate();
}

World::~World() {

}

void World::generate() {
	vertices_.clear();
	indices_.clear();

	int nVerticesX = numChunksX_* chunkSize_;
	int nVerticesZ = numChunksZ_* chunkSize_;
	int nFaces = (nVerticesX - 1) * (nVerticesZ - 1);
	int nIndices = nFaces * 3 * 2; // 2 triangles per square in the mesh

	vertices_.resize(nVerticesX * nVerticesZ);
	indices_.resize(nIndices);

	for (int z = 0; z < nVerticesZ; ++z) {
		for (int x = 0; x < nVerticesX; ++x) {
			float height = noise->recursivePerlin(x, 1, z);
			glm::vec3 position = glm::vec3(x, height, z);
			vertices_[x + z * nVerticesX].pos = position;
			vertices_[x + z * nVerticesX].color = glm::vec3(1, 0, 0);

			glm::vec3 dxnPos = glm::vec3(x - 1, noise->recursivePerlin(x - 1, 1, z), z) - position;
			glm::vec3 dxpPos = glm::vec3(x + 1, noise->recursivePerlin(x + 1, 1, z), z) - position;
			glm::vec3 dznPos = glm::vec3(x, noise->recursivePerlin(x, 1, z - 1), z - 1) - position;
			glm::vec3 dzpPos = glm::vec3(x, noise->recursivePerlin(x, 1, z + 1), z + 1) - position;

			glm::vec3 normal = glm::cross(dxnPos, dzpPos);
			normal += glm::cross(dxpPos, dznPos);
			vertices_[x + z * nVerticesX].normal = glm::normalize(normal);

			// calculate uv based on height: i.e. if height > 1: rock, > 0.7: dirt, > 0.3: grass, > 0.1: sand, else water
			vertices_[x + z * nVerticesX].uv = glm::vec2((float) (x % nVerticesX) / (nVerticesX / (8 * numChunksX_)), (float) (z % nVerticesZ) / (nVerticesZ / (8 * numChunksZ_)));
		}
	}

	int indexCounter = 0;
	for (int z = 0; z < nVerticesZ - 1; ++z) {
		for (int x = 0; x < nVerticesX - 1; ++x) {
			unsigned int tlIndex = x + z * nVerticesX;
			unsigned int trIndex = tlIndex + 1;
			unsigned int blIndex = x + (z + 1) * nVerticesX;
			unsigned int brIndex = blIndex + 1;

			indices_.push_back(glm::uvec3(tlIndex, blIndex, brIndex));
			indices_.push_back(glm::uvec3(tlIndex, brIndex, trIndex));
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create OpenGL buffer for vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Create OpenGL buffer for faces
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(glm::uvec3), &indices_[0], GL_STATIC_DRAW);
}

void World::render(Shader* shader, Texture* texture, glm::mat4 viewMat, glm::mat4 projMat) {
	glUseProgram(shader->getShader());

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	GLint vertex = glGetAttribLocation(shader->getShader(), "vertex");
	GLint color = glGetAttribLocation(shader->getShader(), "color");
	GLint normal = glGetAttribLocation(shader->getShader(), "normal");
	GLint uv = glGetAttribLocation(shader->getShader(), "uv");

	glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)24);
	glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)36);

	glEnableVertexAttribArray(vertex);
	glEnableVertexAttribArray(color);
	glEnableVertexAttribArray(normal);
	glEnableVertexAttribArray(uv);

	//Create the transform matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0)); //glm::vec3(-(numChunksX_ * chunkSize_) / 2, 0, -(numChunksZ_ * chunkSize_) / 2)
	glm::mat4 rotationMatrix = glm::mat4_cast(glm::angleAxis(0.0f, glm::vec3(0.0, 1.0, 0.0)));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1));
	glm::mat4 transformation = translationMatrix * rotationMatrix * scaleMatrix;

	//Load all the shader matricies into thier uniforms
	shader->loadShaderMatrix(transformation, "world_mat");
	shader->loadShaderMatrix(viewMat, "view_mat");
	shader->loadShaderMatrix(projMat, "projection_mat");

	// load texture into shader
	GLint tex = glGetUniformLocation(shader->getShader(), "texture_map");

	glUniform1i(tex, 0);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, texture->getTexture());

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glDrawElements(GL_TRIANGLES, indices_.size() * 3, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Noise* World::getNoise() {
	return noise;
}

void World::setNumChunks(int n) {
	numChunksX_ = n;
	numChunksZ_ = n;
}