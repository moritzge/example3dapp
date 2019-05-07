#pragma once

#include "camera.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
//enum Camera_Movement {
//    FORWARD,
//    BACKWARD,
//    LEFT,
//    RIGHT
//};

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

		Position -= glm::vec3(xoffset, yoffset, 0);

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
