#ifndef CAMERA_H
#define CAMERA_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<vector>


enum camera_movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(glm::u8vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW
		, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVector();
	}

	Camera(float Xpos, float Ypos, float Zpos, float Xup, float Yup, float Zup, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f))
		, movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		Position = glm::vec3(Xpos, Ypos, Zpos);
		WorldUp = glm::vec3(Xup, Yup, Zup);
		Pitch = pitch;
		Yaw = yaw;
		updateCameraVector();
	}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	void processKeyboard(camera_movement direction, float deltaTime) {
		float displacement = movementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * displacement;
		else if (direction == BACKWARD)
			Position -= Front * displacement;
		else if (direction == RIGHT)
			Position += Right * displacement;
		else if (direction == LEFT)
			Position -= Right * displacement;
		else if (direction == UP)
			Position += Up * displacement;
		else if (direction == DOWN)
			Position -= Up * displacement;
	}

	void processMouseMovement(float Xoffset, float Yoffset, GLboolean constrainPitch = true) {
		Xoffset *= mouseSensitivity;
		Yoffset *= mouseSensitivity;

		Yaw += Xoffset;
		Pitch += Yoffset;

		if (constrainPitch) {
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		updateCameraVector();

	}

	void processMouseScroll(float Yoffset) {
		zoom -= (float)Yoffset;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 45.0f)
			zoom = 45.0f;
	}

private:
	void updateCameraVector() {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif // !CAMERA_H

