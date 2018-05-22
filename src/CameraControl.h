#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "Entity.h"
#include "Window.h"
#include "Runner.h"

# define M_PI           3.14159265358979323846 

using namespace glm;

class CameraControl : public Entity {
public:
	Window *window = 0;
	Camera *camera = 0;

	float yaw = 0;
	float pitch = -M_PI / 4;

	float mouseSpeed = 0.001f;
	glm::vec3 position = glm::vec3(0, 256, 0);
	float speed = 100;
	bool mouseDown = false;
	float lastMouseX, lastMouseY;

	void start() override {
		if (window == 0) throw std::runtime_error("window cannot be empty");
		if (camera == 0) throw std::runtime_error("camera cannot be empty");
	}

	void update() override {
		updateCamera();
	}

	void updateCamera() {
		if (!window->hasFocus()) {
			return;
		}
		float pi = glm::pi<float>();
		float delta = this->runner->delta;

		double mouseX, mouseY;
		glfwGetCursorPos(window->window, &mouseX, &mouseY);

		float diffX = mouseX - lastMouseX;
		float diffY = mouseY - lastMouseY;

		lastMouseX = mouseX;
		lastMouseY = mouseY;

		if (mouseDown) {
			yaw += mouseSpeed * diffX;
			pitch -= mouseSpeed * diffY;
		}

		pitch = fmin(pi / 2, fmax(-pi / 2, pitch));

		glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
		glm::quat qRoll = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));
		glm::quat quat = qRoll * qPitch * qYaw;

		glm::vec3 direction = glm::vec3(0, 0, 1) * quat;

		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = glm::cross(direction, up);
		glm::vec3 forward = glm::cross(up, right);

		if (glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
			mouseDown = true;
		}
		else {
			mouseDown = false;
		}

		if (glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS) {
			position += forward * delta * speed;
		}
		if (glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS) {
			position -= forward * delta * speed;
		}
		if (glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * delta * speed;
		}
		if (glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * delta * speed;
		}
		if (glfwGetKey(window->window, GLFW_KEY_R) == GLFW_PRESS) {
			position += up * delta * speed;
		}
		if (glfwGetKey(window->window, GLFW_KEY_F) == GLFW_PRESS) {
			position -= up * delta * speed;
		}

		camera->position = position;
		camera->target = position + direction;
	}
};
