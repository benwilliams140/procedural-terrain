#include "App.h"

App::App()
	:	windowTitle_("Procedural Generation - COMP3009 Final"),
		windowWidth_(1024), windowHeight_(768),
		mouseX_(-1), mouseY_(-1), paused_(false) {
	try {
		if (!glfwInit()) {
			throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
		}

		window_ = glfwCreateWindow(windowWidth_, windowHeight_, windowTitle_, NULL, NULL);
		if (!window_) {
			glfwTerminate();
			throw(std::runtime_error(std::string("Could not create GLFW window")));
		}
		glfwMakeContextCurrent(window_);
		glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char*)glewGetErrorString(err))));
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glViewport(0, 0, windowWidth_, windowHeight_);

		shader_ = new Shader("shader");
		camera_ = new Camera(glm::vec3(-15, 25, -15), glm::vec3(10, 0, 10), glm::vec3(0, 1, 0), windowWidth_, windowHeight_);
		world_ = new World();
		gui_ = new GUI(window_, world_);

		worldTexture_ = new Texture("green_grass.jpg");
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

App::~App() { }

void App::render() {
	glm::mat4 projMat = camera_->getProjectionMatrix();
	glm::mat4 viewMat = camera_->getViewMatrix();

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projMat));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(viewMat));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	world_->render(shader_, worldTexture_, viewMat, projMat);

	if (paused_) gui_->render(shader_);

	glfwSwapBuffers(window_);
}

void App::pollEvents() {
	glfwPollEvents();
}

void App::keyInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		if (paused_) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		paused_ = !paused_;
	}
	if (key == GLFW_KEY_W) {
		switch (action) {
		case GLFW_PRESS: camera_->setForwardSpeed(1); break;
		case GLFW_RELEASE: camera_->setForwardSpeed(0); break;
		}
	}
	if (key == GLFW_KEY_S) {
		switch (action) {
		case GLFW_PRESS: camera_->setForwardSpeed(-1); break;
		case GLFW_RELEASE: camera_->setForwardSpeed(0); break;
		}
	}
	if (key == GLFW_KEY_A) {
		switch (action) {
		case GLFW_PRESS: camera_->setSideSpeed(-1); break;
		case GLFW_RELEASE: camera_->setSideSpeed(0); break;
		}
	}
	if (key == GLFW_KEY_D) {
		switch (action) {
		case GLFW_PRESS: camera_->setSideSpeed(1); break;
		case GLFW_RELEASE: camera_->setSideSpeed(0); break;
		}
	}
}

void App::mouseInput(GLFWwindow* window, double x, double y) {
	if (mouseX_ == -1 && mouseY_ == -1) {
		mouseX_ = x;
		mouseY_ = y;
	}

	float xOffset = (x - mouseX_) * 0.25f;
	float yOffset = (y - mouseY_) * 0.25f;
	mouseX_ = x;
	mouseY_ = y;

	if (!paused_) {
		camera_->yaw(xOffset);
		camera_->pitch(yOffset);
	}
}

bool App::isRunning() {
	return !glfwWindowShouldClose(window_);
}

bool App::isPaused() {
	return paused_;
}

Camera* App::getCamera() {
	return camera_;
}

GLFWwindow* App::getWindow() {
	return window_;
}