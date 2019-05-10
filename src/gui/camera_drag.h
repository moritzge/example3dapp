#pragma once

#include "camera.h"

#include <iostream>

template<int N>
std::ostream& operator<<(std::ostream &o, glm::vec<N, float> v) {
	for (int i = 0; i < N; ++i) {
		o << v[i] << ", ";
	}
	return o;
}

class CameraTracking
{
public:
	// Camera Attributes
	// explicitely keep track of the rotations about the "up" and "horizontal right" directions to avoid singularities in decomposition of large rotations
	float rotAboutUpAxis = 0, rotAboutRightAxis = 0;
	// the location of the camera is determined as a function of its orientation, its target, and how far along the target it is
	float distance = 40;
	// this is the target of the camera, in world coordinates
	glm::vec3 target = {0, 0, 0};
	// we need to keep track of the view direction and up axis
	glm::vec3 direction = {1, 1, 1}, up = {0, 1, 0};

	bool ignoreRotations = false;

	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	glm::mat4 matCam;

	// Constructor with vectors
	CameraTracking(glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), target(target), up(up), Yaw(yaw), Pitch(pitch)
	{
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	 }
	// Constructor with scalar values
//	CameraDrag(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
//    {
//        Position = glm::vec3(posX, posY, posZ);
//        WorldUp = glm::vec3(upX, upY, upZ);
//        Yaw = yaw;
//        Pitch = pitch;
//        updateCameraVectors();
//    }

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		glm::vec3 worldUp = rotation() * glm::vec4(up, 1);

		return glm::lookAt(position(), target, worldUp);
	}

	glm::vec3 position() const {
		return target + glm::vec3(rotation() * glm::vec4(direction, 1)) * distance;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
//		float velocity = MovementSpeed * deltaTime;
//		if (direction == FORWARD)
//			Position += Front * velocity;
//		if (direction == BACKWARD)
//			Position -= Front * velocity;
//		if (direction == LEFT)
//			Position -= Right * velocity;
//		if (direction == RIGHT)
//			Position += Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessLeftMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		rotAboutUpAxis -= xoffset * MouseSensitivity;
		rotAboutRightAxis += yoffset * MouseSensitivity;
	}

	void ProcessMiddleMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		glm::vec3 m = {xoffset, yoffset, 0.f};
		glm::mat4 rot = GetViewMatrix();
		target -= MouseSensitivity*2 * glm::vec3(glm::inverse(rot) * glm::vec4(m, 0.f));
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		distance *= std::pow(1.05f, yoffset);
//		target += yoffset * glm::vec3(rotation() * glm::vec4(direction, 0));
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
//        // Calculate the new Front vector
//        glm::vec3 front;
//        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//        front.y = sin(glm::radians(Pitch));
//        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//        Front = glm::normalize(front);
//        // Also re-calculate the Right and Up vector
//        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
//        Up    = glm::normalize(glm::cross(Right, Front));
	}

	glm::mat4 rotation() const {
		glm::mat4 rot(1.f);
		rot = glm::rotate(rot, rotAboutUpAxis, up);
		rot = glm::rotate(rot, rotAboutRightAxis, glm::cross(up, direction));
		return rot;
	}
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CameraDrag
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

	glm::mat4 matCam;

    // Constructor with vectors
	CameraDrag(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
		Front = glm::vec3(0, 0, 1);
		Right = glm::vec3(0, 1, 0);
        updateCameraVectors();
	 }
    // Constructor with scalar values
//	CameraDrag(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
//    {
//        Position = glm::vec3(posX, posY, posZ);
//        WorldUp = glm::vec3(upX, upY, upZ);
//        Yaw = yaw;
//        Pitch = pitch;
//        updateCameraVectors();
//    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {

		matCam = glm::identity<glm::mat4>();
		matCam = glm::translate(matCam, Position);
		matCam = glm::rotate(matCam, glm::radians(Yaw), glm::vec3(0, 1, 0));
		matCam = glm::rotate(matCam, glm::radians(Pitch), glm::vec3(0, 0, 1));

		return matCam;//glm::lookAt(Position, glm::vec3(0, 0, 0)/*Position + Front*/, Up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessLeftMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
		xoffset *= MouseSensitivity*MouseSensitivity;
		yoffset *= MouseSensitivity*MouseSensitivity;

		Position += glm::vec3(xoffset, yoffset, 0);

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

	void ProcessMiddleMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw   += xoffset;
		Pitch += yoffset;

		updateCameraVectors();
	}

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
//        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
//        if (Zoom <= 1.0f)
//            Zoom = 1.0f;
//        if (Zoom >= 45.0f)
//            Zoom = 45.0f;
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
//        // Calculate the new Front vector
//        glm::vec3 front;
//        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//        front.y = sin(glm::radians(Pitch));
//        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//        Front = glm::normalize(front);
//        // Also re-calculate the Right and Up vector
//        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
//        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
