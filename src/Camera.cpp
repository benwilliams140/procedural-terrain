#include <Camera.h>


Camera::Camera(glm::vec3 position, glm::vec3 lookAtPosition, glm::vec3 upVector, int windowWidth, int windowHeight) :
	position_(position), forwardVector_(glm::normalize(lookAtPosition - position_)), upVector_(glm::normalize(upVector)),
	fov_(60.0f), nearPlane_(0.1f), farPlane_(1000.0f), aspect_(windowWidth / windowHeight), forwardSpeed_(0), sideSpeed_(0)
{
	viewMatrix_ = glm::lookAt(position, forwardVector_, upVector_);
	float top = tan((fov_ / 2.0) * (glm::pi<float>() / 180.0)) * nearPlane_;
	float right = top * aspect_;
	projectionMatrix_ = glm::perspective(fov_, aspect_, nearPlane_, farPlane_);

	orientation_ = glm::quat();
}

Camera::~Camera(void) {
}

void Camera::roll(float angle) {
	glm::quat rotation = glm::angleAxis(glm::radians(angle), getForward());
	orientation_ = glm::normalize(orientation_ * rotation);
	viewMatrix_ = glm::lookAt(position_, position_ + forwardVector_, upVector_);
}

void Camera::pitch(float angle) {
	glm::quat rotation = glm::angleAxis(glm::radians(angle), getSide());
	orientation_ = glm::normalize(orientation_ * rotation);
	viewMatrix_ = glm::lookAt(position_, position_ + forwardVector_, upVector_);
}

void Camera::yaw(float angle) {
	glm::quat rotation = glm::angleAxis(glm::radians(angle), getUp());
	orientation_ = glm::normalize(orientation_ * rotation);
	viewMatrix_ = glm::lookAt(position_, position_ + forwardVector_, upVector_);
}

void Camera::move() {
	position_ += getForward() * forwardSpeed_;
	position_ += getSide() * sideSpeed_;
}

void Camera::setForwardSpeed(int speed) {
	forwardSpeed_ = speed * 0.15f;
}

void Camera::setSideSpeed(int speed) {
	sideSpeed_ = speed * 0.15f;
}

void Camera::zoomIn(float zoom) {
	fov_ -= zoom;
	if (fov_ < 10) fov_ = 10;

	//DO NOT remove the line below:
	projectionMatrix_ = glm::perspective(fov_ / 2, aspect_, nearPlane_, farPlane_);
}

void Camera::zoomOut(float zoom) {
	fov_ += zoom;
	if (fov_ > 150) fov_ = 150;

	//DO NOT remove the line below:
	projectionMatrix_ = glm::perspective(fov_ / 2, aspect_, nearPlane_, farPlane_);
}

glm::vec3 Camera::getPosition(void) {
	return position_;
}

glm::vec3 Camera::getLookAtPoint(void) {
	return (position_ + getForward());
}

glm::vec3 Camera::getForward(void) const {
	return forwardVector_ * orientation_;
}

glm::vec3 Camera::getSide(void) const {
	glm::vec3 current_side = glm::cross(getForward(), getUp());
	current_side = glm::normalize(current_side);
	return current_side;
}

glm::vec3 Camera::getUp(void) const {
	return upVector_ * orientation_;
}

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 m = glm::lookAt(position_, getLookAtPoint(), getUp());
	return(m);
}

void Camera::setPerspectiveView(float fov, float aspect, float nearPlane, float farPlane) {
	fov_ = fov;
	aspect_ = aspect;
	nearPlane_ = nearPlane;
	farPlane_ = farPlane;

	projectionMatrix_ = glm::perspective(fov_, aspect_, nearPlane_, farPlane_);
}


glm::mat4 Camera::getProjectionMatrix() {
	glm::mat4 m = glm::perspective(fov_ / 2, aspect_, nearPlane_, farPlane_);
	return(m);
}
