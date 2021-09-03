#include "Camera.h"

Camera::Camera(glm::vec3 InPosition, glm::vec3 InTarget, glm::vec3 InWorldUp)
{
	Positon = InPosition;
	WorldUp = InWorldUp;
	Forward = glm::normalize(InTarget - InPosition);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

Camera::Camera(glm::vec3 InPosition, float InPitch, float InYaw, float InRoll, glm::vec3 InWorldUp)
{
	Positon = InPosition;
	WorldUp = InWorldUp;
	Pitch = InPitch;
	Yaw = InYaw;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Positon, Positon + Forward, WorldUp);
}

void Camera::ProcessMouseMovement(float DeltaX, float DeltaY)
{
	Pitch -= DeltaY * CameraSpeedY;
	Yaw -= DeltaX * CameraSpeedX;
	UpdateCameraVectors();
}

void Camera::UpdateCameraPosition()
{
	Positon += Forward * CameraMoveSpeedZ + Right * CameraMoveSpeedX +Up * CameraMoveSpeedY;
}

void Camera::UpdateCameraVectors()
{
	Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right,Forward));
}
