#pragma once

#include "World.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <string>

class GUI {
public:
	GUI(GLFWwindow*, World*);
	~GUI();

	void render(Shader*);
protected:
private:
	World* world_;

	float frequency_, amplitude_, lacunarity_;
	int octaves_, numChunks_;
	float lightPosition_[3] = { 0, 100, 0 };
};