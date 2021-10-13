#pragma once

#define GLEW_STATIC

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>
#include <vector>
#include <string>

class Camera
{
public:
	Camera(glm::vec3, glm::vec3, glm::vec3, int, int);
	~Camera(void);

	void roll(float);
	void pitch(float);
	void yaw(float);

	void move();
	void setForwardSpeed(int);
	void setSideSpeed(int);

	void zoomIn(float);
	void zoomOut(float);

	glm::vec3 getPosition();
	glm::vec3 getLookAtPoint();
	glm::vec3 getSide(void) const;
	glm::vec3 getForward(void) const;
	glm::vec3 getUp(void) const;

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	void setPerspectiveView(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

private:
	glm::mat4 viewMatrix_;
	glm::mat4 projectionMatrix_;
	glm::vec3 position_;
	glm::vec3 upVector_;
	glm::vec3 forwardVector_;

	glm::quat orientation_;

	float fov_, aspect_, nearPlane_, farPlane_;
	float forwardSpeed_, sideSpeed_;
};