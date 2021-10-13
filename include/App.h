#pragma once

#include "Camera.h"
#include "World.h"
#include "Shader.h"
#include "Texture.h"
#include "GUI.h"

#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class App {
public:
	App();
	~App();

	void render();

	void pollEvents();
	void keyInput(GLFWwindow*, int, int, int, int);
	void mouseInput(GLFWwindow*, double, double);

	/*
	Getters
	*/
	bool isRunning();
	bool isPaused();
	Camera* getCamera();
	GLFWwindow* getWindow();

	/*
	Setters
	*/

protected:
private:
	GLFWwindow* window_;
	int windowWidth_, windowHeight_;
	const char* windowTitle_;
	bool paused_;

	double mouseX_, mouseY_;

	GUI* gui_;

	Camera* camera_;
	World* world_;
	Shader* shader_;
	Texture* worldTexture_;
};