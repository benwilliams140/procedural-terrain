#include "App.h"

App* app;

void keyInput(GLFWwindow*, int, int, int, int);
void mouseInput(GLFWwindow*, double, double);

int main(int arg, char** argv) {
	app = new App();

	glfwSetKeyCallback(app->getWindow(), keyInput);
	glfwSetCursorPosCallback(app->getWindow(), mouseInput);

	while (app->isRunning()) { 
		app->render();
		app->pollEvents();
		if (!app->isPaused()) {
			app->getCamera()->move();
		}
	}
	delete app;
}

void keyInput(GLFWwindow* window, int key, int scancode, int action, int mods) { app->keyInput(window, key, scancode, action, mods); }
void mouseInput(GLFWwindow* window, double x, double y) { app->mouseInput(window, x, y);  }