#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "util.h"

class Camera
{
public:

	Camera();
	Camera(glm::vec3 InPosition, glm::vec3 InTarget, glm::vec3 InWorldup);
	Camera(glm::vec3 InPosition, float InPitch, float InYaw, float InRoll, glm::vec3 InWorldUp);

	//相机自身的位置
	glm::vec3 Positon;

	//相机的朝向
	glm::vec3 Forward;
	
	//相机向右的方向
	glm::vec3 Right;

	//相机向上的方向
	glm::vec3 Up;

	//世界向上的方向
	glm::vec3 WorldUp;

	float Pitch;
	float Yaw;
	float Roll;
	float CameraSpeedX = 0.02f;
	float CameraSpeedY = 0.02f;
	float CameraMoveSpeedX = 0;
	float CameraMoveSpeedY = 0;
	float CameraMoveSpeedZ = 0;

	//计算LookAt矩阵
	glm::mat4 GetViewMatrix();

	//返回相机位置
	glm::vec3& GetPosition();

	void ProcessMouseMovement(float DeltaX, float DeltaY);

	void UpdateCameraPosition();

	void UpdateCameraVectors();

};

