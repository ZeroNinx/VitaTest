#include "Camera.h"

Camera::Camera()
{
	
}

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

glm::vec3& Camera::GetPosition()
{
	return Positon;
}

void Camera::ProcessMouseMovement(float DeltaX, float DeltaY)
{
	Pitch -= DeltaY * CameraSpeedY;
	
	//防止相机Pitch绝对值大于90度发生视角逆转
	if(Pitch > 89.0f)
	{
  		Pitch =  89.0f;
	}
	if(Pitch < -89.0f)
  	{	
		Pitch = -89.0f;
	}
	
	Yaw -= DeltaX * CameraSpeedX;
	UpdateCameraVectors();
}

void Camera::UpdateCameraPosition()
{
	Positon += Forward * CameraMoveSpeedZ + Right * CameraMoveSpeedX + Up * CameraMoveSpeedY;
}

void Camera::UpdateCameraVectors()
{
	Forward.x = glm::cos(glm::radians(Pitch)) * glm::sin(glm::radians(Yaw));
	Forward.y = glm::sin(glm::radians(Pitch));
	Forward.z = glm::cos(glm::radians(Pitch)) * glm::cos(glm::radians(Yaw));
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right,Forward));
}
